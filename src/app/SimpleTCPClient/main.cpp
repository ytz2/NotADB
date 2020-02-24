/*
 * TcpClient.cpp
 *
 *  Created on: Feb 17, 2020
 *      Author: yanhualiu
 */

#include <lib/tcpconnection/TcpConnection.h>
#include <message/commonjson/CommonJsonMessage.h>
using namespace std;
using namespace lib::tcp;
using boost::asio::ip::tcp;

void handleClient(const boost::system::error_code &e,
		std::shared_ptr<boost::asio::deadline_timer> t,
		std::shared_ptr<TcpClientSession> client) {
	if (client->connected()) {
		auto msg =
				std::make_shared<lib::message::commonjson::CommonJsonMessage>();
		auto &v = *msg;
		v["msg"] = "heartbeat";
		client->send(msg);
	}
	t->expires_from_now(boost::posix_time::seconds(3));
	auto func = std::bind(handleClient, std::placeholders::_1, t, client);
	t->async_wait(func);
}

int main() {
	auto codec = std::make_shared<FlatMessageDecoder>("CommonJsonMessage");
	boost::asio::io_service service;
	auto client = std::make_shared<TcpClientSession>(service, "SimpleClient",
			"0.0.0.0", "45679", codec);
	client->setRetryIntervalMill(3000);
	client->setTimeoutIntervalMill(100);
	if (!client->start()) {
		LOG(ERROR) << "Failed to start the tcp client";
		return -1;
	}
	std::shared_ptr<boost::asio::deadline_timer> t = std::make_shared<
			boost::asio::deadline_timer>(service);
	boost::system::error_code e;
	handleClient(e, t, client);
	service.run();
	return 0;
}
