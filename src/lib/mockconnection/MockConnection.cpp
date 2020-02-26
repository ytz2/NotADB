/* Copyright 2020 Yanhua Liu */
#include "MockConnection.h"

#include <iostream>

namespace lib {
namespace mock {
MockAcceptor::MockAcceptor(MockTcpMgr &tcpMgr, boost::asio::io_service &io)
    : io_(io), mgr_(tcpMgr) {
  t_ = std::make_shared<boost::asio::deadline_timer>(io);
}

bool MockAcceptor::start() {
  if (!IAcceptor::start()) {
    return false;
  }
  accept();
  return true;
}

void MockAcceptor::stop() {
  t_->cancel();
  IAcceptor::stop();
}

void MockAcceptor::accept() {
  if (!isReady()) {
    t_->cancel();
  }
  auto sessions = mgr_.consume(ADMIN);
  if (!sessions.empty()) {
    while (!sessions.empty()) {
      auto s = sessions.front();
      sessions.pop();
      auto news = std::make_shared<MockConnection>(*s, io_, SERVER);
      addSession(news->name(), news);
      news->registerSessionCallback(shared_from_this());
      news->registerListner(std::make_shared<MockListener>(SERVER));
      news->start();
      LOG(INFO) << news->name() << " has been accepted for connection"
                << std::endl;
    }
  }

  t_->expires_from_now(boost::posix_time::millisec(10));
  t_->async_wait(
      [this](const boost::system::error_code &e) { this->accept(); });
}

void MockAcceptor::onDisconnect(interface::ISessionPtr session) {
  auto n = session->name();
  LOG(INFO) << n << " has disconnected" << std::endl;
  removeSession(n);
}

void MockAcceptor::onMessageSent(interface::ISessionPtr s,
                                 const interface::IMessagePtr msg) {
  std::string res;
  if (msg->ToString(res)) {
    LOG(INFO) << s->name() << " sent msg " << res << std::endl;
  }
}

MockConnection::MockConnection(MockTcp &tcp, boost::asio::io_service &io,
                               MockSide side)
    : interface::ISession(tcp.getName(), true),
      tcp_(tcp),
      io_(io),
      side_(side) {
  t_ = std::make_shared<boost::asio::deadline_timer>(io);
}

void MockConnection::disconnect() {
  connected_ = false;
  t_->cancel();
  tcp_.close();
}

bool MockConnection::send(const interface::IMessagePtr msg) {
  if (side_ == SERVER) {
    tcp_.produce(msg, SERVER);
    return true;
  }
  tcp_.produce(msg, CLIENT);
  return true;
}

bool MockConnection::start() {
  connected_ = true;
  poll();
  return true;
}

void MockConnection::poll() {
  if (tcp_.closed()) {
    onDisconnect();
    return;
  }
  MockIO<interface::IMessagePtr>::MockQueue q;
  if (side_ == SERVER)
    q = tcp_.consume(CLIENT);
  else
    q = tcp_.consume(SERVER);
  while (!q.empty()) {
    auto m = q.front();
    q.pop();
    onMessage(m);
  }
  boost::system::error_code err;
  t_->expires_from_now(boost::posix_time::millisec(10));
  t_->async_wait([this](const boost::system::error_code &e) { this->poll(); });
}

}  // namespace mock
} /* namespace lib */
