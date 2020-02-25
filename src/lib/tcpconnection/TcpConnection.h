/*
 * TcpConnection.h
 *
 *  Created on: Feb 9, 2020
 *      Author: yanhualiu
 */

#ifndef SRC_LIB_TCPCONNECTION_TCPCONNECTION_H_
#define SRC_LIB_TCPCONNECTION_TCPCONNECTION_H_
#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include  <interface/connection.h>
#include <interface/message.h>
#include <lib/tcpconnection/MessageCodec.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <glog/logging.h>
namespace lib {
namespace tcp {

class TcpSession: public interface::ISession {
public: // ISession
	TcpSession(boost::asio::ip::tcp::socket soccket, MessageCodecPtr codec,
			const std::string &name);
	virtual ~TcpSession() {
		if (socket_.is_open()) {
			socket_.close();
		}
	}
	virtual void disconnect() override {
		if (socket_.is_open())
			socket_.close();
		interface::ISession::disconnect();
	}
	virtual bool start() override;
	virtual bool send(const interface::IMessagePtr msg) override;

protected:
	void handle_read(const boost::system::error_code &error,
			std::size_t bytes_transferred);
protected:
	boost::asio::ip::tcp::socket socket_;
	MessageCodecPtr codec_;
};

class TcpClientSession: public TcpSession {
public:
	TcpClientSession(boost::asio::io_service &io, const std::string &name,
			const std::string &host, const std::string &port,
			MessageCodecPtr codec) :
			TcpSession(std::move(boost::asio::ip::tcp::socket(io)), codec,
					name), io_(io), host_(host), port_(port), retryTimer_(
					std::make_shared<boost::asio::deadline_timer>(io)), timeouter_(
					std::make_shared<boost::asio::deadline_timer>(io)), retryInterval_(
					100), timeoutInterval_(100), manualllyStopped_(false) {

	}

	virtual bool start() override;
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
		retryTimer_->async_wait([this](const boost::system::error_code &e) {
			this->start();
		});
	}
	virtual void onDisconnect() override {
		LOG(ERROR) << "Disconnected from remote";
		TcpSession::onDisconnect();
		// as a client, we will reconnect
		if (!manualllyStopped_)
			onConnectionFail();
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

class TcpServer: public std::enable_shared_from_this<TcpServer>,
		public interface::IAcceptor,
		public interface::IConnectionCallback,
		public interface::ICallbackContainer {
public: // MockAcceptors
	TcpServer() = delete;
	TcpServer(boost::asio::io_context &io_context, int port,
			MessageCodecPtr codec) :
			codec_(codec), acceptor_(io_context,
					boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
							port)) {
		LOG(INFO) << "Tcp Server start will listen on " << port;
	}
public: // IAcceptor
	virtual bool start() override;
	virtual void stop() override;
public: // IConnectionCallback
	virtual void onDisconnect(interface::ISessionPtr session) override;
	virtual void onMessageSent(interface::ISessionPtr,
			const interface::IMessagePtr) override;
	virtual void onConnect(interface::ISessionPtr session) override;
public: // ICallbackContainer
	virtual void onMessage(const interface::IMessagePtr msg) override {
		//noop
	}
	virtual void onConnected() override {
		//noop
	}
	virtual void onDisconnect() override {
		// noop
	}
protected:
	void accept();

protected:
	boost::asio::ip::tcp::acceptor acceptor_;
	MessageCodecPtr codec_;
};

} // namesapce tcp
} /* namespace lib */

#endif /* SRC_LIB_TCPCONNECTION_TCPCONNECTION_H_ */
