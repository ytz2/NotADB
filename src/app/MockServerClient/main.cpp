/*
 * main.cpp
 *
 *  Created on: Feb 4, 2020
 *      Author: yanhualiu
 */

#include <lib/MockConnection.h>
#include <glog/logging.h>
#include <stdlib.h>
#include <functional>
static int count = 1;

void heartbeat(const boost::system::error_code &e,
		boost::asio::deadline_timer *t,
		std::shared_ptr<lib::mock::MockConnection> client) {
	auto msg = std::make_shared<lib::mock::MockMessage>();
	auto &v = *msg;
	v["msg"] = "heartbeat";
	v["seq"] = count++;
	client->send(msg);
	t->expires_from_now(boost::posix_time::seconds(1));
	t->async_wait(std::bind(heartbeat, std::placeholders::_1, t, client));
}

int main(int argc, char *argv[]) {
	google::InitGoogleLogging(argv[0]);
	google::LogToStderr();
	google::InstallFailureSignalHandler();
	boost::asio::io_service io;
	lib::mock::MockTcpMgr tcpMgr("manager");
	auto socket1 = std::make_shared<lib::mock::MockTcp>("connection1");
	tcpMgr.produce(socket1, lib::mock::ADMIN);
	auto client1 = std::make_shared<lib::mock::MockConnection>(*socket1, io,
			lib::mock::CLIENT);
	auto listener1 = std::make_shared<lib::mock::MockListener>(client1,
			lib::mock::CLIENT);
	client1->registerListner(listener1);
	auto server = std::make_shared<lib::mock::MockAcceptor>(tcpMgr, io);
	server->start();
	client1->init();
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
	t.async_wait(std::bind(heartbeat, std::placeholders::_1, &t, client1));
	io.run();
	return 0;
}
