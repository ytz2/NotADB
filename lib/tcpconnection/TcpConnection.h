/* Copyright 2020 Yanhua Liu */
#pragma once

#include <glog/logging.h>
#include "interface/connection.h"
#include "interface/message.h"
#include "MessageCodec.h"

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>   //NOLINT
#include <iostream>  //NOLINT
#include <memory>    //NOLINT
#include <string>    //NOLINT
#include <utility>   //NOLINT

namespace lib {
namespace tcp {

class TcpSession : public interface::ISession {
 public:  // ISession
  TcpSession(boost::asio::ip::tcp::socket soccket, MessageCodecPtr codec,
             const std::string &name);

  virtual ~TcpSession() {
    if (socket_.is_open()) {
      socket_.close();
    }
  }

  void disconnect() override {
    if (socket_.is_open()) socket_.close();
    interface::ISession::disconnect();
  }

  bool start() override;

  bool send(const interface::IMessagePtr msg) override;

 protected:
  void handle_read(const boost::system::error_code &error,
                   std::size_t bytes_transferred);

 protected:
  boost::asio::ip::tcp::socket socket_;
  MessageCodecPtr codec_;
};

class TcpClientSession : public TcpSession {
 public:
  explicit TcpClientSession(boost::asio::io_service &io,  // NOLINT
                            const std::string &name, const std::string &host,
                            const std::string &port, MessageCodecPtr codec)
      : TcpSession(std::move(boost::asio::ip::tcp::socket(io)), codec, name),
        io_(io),
        host_(host),
        port_(port),
        retryTimer_(std::make_shared<boost::asio::deadline_timer>(io)),
        timeouter_(std::make_shared<boost::asio::deadline_timer>(io)),
        retryInterval_(100),
        timeoutInterval_(100),
        manualllyStopped_(false) {}

  bool start() override;

  void stop() {
    manualllyStopped_ = true;
    disconnect();
  }

  void setTimeoutIntervalMill(int t) {
    timeoutInterval_ = boost::posix_time::millisec(t);
  }

  void setRetryIntervalMill(int t) {
    retryInterval_ = boost::posix_time::millisec(t);
  }

 protected:
  virtual void onConnectionFail() {
    LOG(ERROR) << "Connection failed, will rety in "
               << retryInterval_.total_milliseconds() << " mill seconds";
    retryTimer_->expires_from_now(retryInterval_);
    retryTimer_->async_wait(
        [this](const boost::system::error_code &e) { this->start(); });
  }

  void onDisconnect() override {
    LOG(ERROR) << "Disconnected from remote";
    TcpSession::onDisconnect();
    // as a client, we will reconnect
    if (!manualllyStopped_) onConnectionFail();
  }

 protected:
  std::atomic<bool> manualllyStopped_;
  boost::asio::io_service &io_;
  std::string host_;
  std::string port_;
  std::shared_ptr<boost::asio::deadline_timer> retryTimer_;
  std::shared_ptr<boost::asio::deadline_timer> timeouter_;
  boost::posix_time::millisec retryInterval_;
  boost::posix_time::millisec timeoutInterval_;
};

class TcpServer : public std::enable_shared_from_this<TcpServer>,
                  public interface::IAcceptor,
                  public interface::IConnectionCallback,
                  public interface::ICallbackContainer {
 public:  // MockAcceptors
  TcpServer() = delete;

  explicit TcpServer(boost::asio::io_context &io_context, int port,  // NOLINT
                     MessageCodecPtr codec)
      : codec_(codec),
        acceptor_(io_context, boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(), port)) {
    LOG(INFO) << "Tcp Server start will listen on " << port;
  }

 public:  // IAcceptor
  bool start() override;

  void stop() override;

 public:  // IConnectionCallback
  void onDisconnect(interface::ISessionPtr session) override;

  void onMessageSent(interface::ISessionPtr,
                     const interface::IMessagePtr) override;

  void onConnect(interface::ISessionPtr session) override;

 public:  // ICallbackContainer
  void onMessage(const interface::IMessagePtr msg) override {
    // noop
  }

  void onConnected() override {
    // noop
  }

  void onDisconnect() override {
    // noop
  }

 protected:
  void accept();

 protected:
  boost::asio::ip::tcp::acceptor acceptor_;
  MessageCodecPtr codec_;
};

}  // namespace tcp
} /* namespace lib */
