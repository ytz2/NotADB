/* Copyright 2020 Yanhua Liu */
#include "TcpConnection.h"

#include <glog/logging.h>

#include <boost/lexical_cast.hpp>
#include <vector>  // NOLINT

namespace lib {
namespace tcp {
using boost::asio::ip::tcp;

static std::size_t chunk_read_size = 1024;
TcpSession::TcpSession(boost::asio::ip::tcp::socket skt, MessageCodecPtr codec,
                       const std::string &name)
    : interface::ISession(name, false), socket_(std::move(skt)), codec_(codec) {
  LOG(INFO) << "server session " << name << " is created";
}

bool TcpSession::start() {
  connected_ = true;
  LOG(INFO) << "session " << name_ << " started";
  socket_.async_read_some(*codec_->readBuffer(chunk_read_size),
                          [this](const boost::system::error_code &e,
                                 std::size_t bytes_transferred) {
                            this->handle_read(e, bytes_transferred);
                          });
  return true;
}

void TcpSession::handle_read(const boost::system::error_code &error,
                             std::size_t bytes_transferred) {
  if (!error && bytes_transferred != 0) {
    codec_->produce(bytes_transferred);
    std::vector<interface::IMessagePtr> msgs;
    if (!codec_->getMessages(msgs)) {
      LOG(ERROR) << "Connection error happened";
      disconnect();
      return;
    }
    for (const auto each : msgs) {
      onMessage(each);
    }
    socket_.async_read_some(
        *codec_->readBuffer(chunk_read_size),
        [this](const boost::system::error_code &err, std::size_t transferred) {
          this->handle_read(err, transferred);
        });
  } else {
    LOG(ERROR) << error.message();
    disconnect();
  }
}

bool TcpSession::send(const interface::IMessagePtr msg) {
  auto buf = codec_->writeBuffer(msg);
  boost::asio::async_write(socket_, *buf,
                           [this, msg](const boost::system::error_code &err,
                                       std::size_t transferred) {
                             if (!err) {
                               for (auto each : this->sessionCallbacks_) {
                                 each->onMessageSent(shared_from_this(), msg);
                               }
                             } else {
                               disconnect();
                             }
                           });
  return true;
}

bool TcpClientSession::start() {
  try {
    manualllyStopped_ = false;
    LOG(INFO) << "Starting the tcp session to " << host_ << ":" << port_;
    tcp::resolver::query q{host_, port_};
    tcp::resolver resolver(io_);
    auto endpoint = resolver.resolve(q);
    LOG(INFO) << "Destination resolved";
    boost::asio::async_connect(
        socket_, endpoint,
        [this](const boost::system::error_code &ec,
               const boost::asio::ip::tcp::endpoint &endpoint) {
          if (!ec) {
            onConnected();
            TcpSession::start();
          } else {
            LOG(ERROR) << "failed to connect due to " << ec.message();
            onConnectionFail();
          }
        });
    timeouter_->expires_from_now(timeoutInterval_);
    timeouter_->async_wait([this](const boost::system::error_code &e) {
      if (!connected()) {
        LOG(ERROR) << "Have not connected, timing it out";
        if (socket_.is_open()) socket_.close();
      }
    });
  } catch (const std::exception &e) {
    LOG(ERROR) << e.what();
    return false;
  } catch (...) {
    LOG(ERROR) << "tcp session start failure for unknown reason";
    return false;
  }
  return true;
}

bool TcpServer::start() {
  if (!IAcceptor::start()) {
    return false;
  }
  accept();
  return true;
}

void TcpServer::stop() {
  IAcceptor::stop();
  for (auto &each : this->sessions_) {
    each.second->disconnect();
  }
  sessions_.clear();
}

void TcpServer::onDisconnect(interface::ISessionPtr session) {
  auto n = session->name();
  LOG(INFO) << n << " has disconnected" << std::endl;
  removeSession(n);
}

void TcpServer::onMessageSent(interface::ISessionPtr s,
                              const interface::IMessagePtr msg) {
  std::string res;
  if (msg->ToString(res)) {
    LOG(INFO) << s->name() << " sent msg " << res << std::endl;
  }
}

void TcpServer::onConnect(interface::ISessionPtr session) {
  auto n = session->name();
  LOG(INFO) << n << " has connected" << std::endl;
  addSession(n, session);
}

void TcpServer::accept() {
  LOG(INFO) << "start to accept connection";
  acceptor_.async_accept([this](boost::system::error_code ec,
                                tcp::socket socket) {
    if (!ec) {
      boost::uuids::uuid uuid = boost::uuids::random_generator()();
      auto name = boost::lexical_cast<std::string>(uuid);
      auto session =
          std::make_shared<TcpSession>(std::move(socket), codec_, name);
      LOG(INFO) << "session " << name << " has been established";
      sessions_[name] = session;
      // give listners to child
      for (auto s : sessionCallbacks_) {
        session->registerSessionCallback(s);
      }
      // also register myself
      session->registerSessionCallback(shared_from_this());
      for (auto l : appCallbacks_) {
        session->registerListner(l);
      }
      if (!session->start()) {
        throw std::runtime_error("Failed to read the socket for tcp server");
      }
    } else {
      LOG(ERROR) << "failed to accept connection, reason: " << ec.message();
    }
    accept();
  });
}

}  // namespace tcp
}  // namespace lib
