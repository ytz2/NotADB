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
	v["signature"] = client->name();
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
	auto socket1 = std::make_shared<lib::mock::MockTcp>("connection one");
	auto socket2 = std::make_shared<lib::mock::MockTcp>("connection two");
	tcpMgr.produce(socket1, lib::mock::ADMIN);
	tcpMgr.produce(socket2, lib::mock::ADMIN);
	auto client1 = std::make_shared<lib::mock::MockConnection>(*socket1, io,
			lib::mock::CLIENT);
	auto client2 = std::make_shared<lib::mock::MockConnection>(*socket2, io,
			lib::mock::CLIENT);
	auto listener = std::make_shared<lib::mock::MockListener>(lib::mock::CLIENT);
	client1->registerListner(listener);
	client2->registerListner(listener);
	auto server = std::make_shared<lib::mock::MockAcceptor>(tcpMgr, io);
	server->start();
	client1->init();
	client2->init();
	boost::asio::deadline_timer t1(io, boost::posix_time::seconds(1));
	t1.async_wait(std::bind(heartbeat, std::placeholders::_1, &t1, client1));
	boost::asio::deadline_timer t2(io, boost::posix_time::seconds(1));
	t2.async_wait(std::bind(heartbeat, std::placeholders::_1, &t2, client2));
	io.run();
	return 0;
}
