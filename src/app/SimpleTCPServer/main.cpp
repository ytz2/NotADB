/*
 * main.cpp
 *
 *  Created on: Feb 23, 2020
 *      Author: yanhualiu
 */

#include <lib/tcpconnection/TcpConnection.h>
#include <message/commonjson/CommonJsonMessage.h>

using namespace std;
using namespace lib::tcp;

int main() {
	boost::asio::io_service service;
	auto codec = std::make_shared<FlatMessageDecoder>("CommonJsonMessage");
	auto server = std::make_shared<TcpServer>(service, 45679, codec);
	if (!server->start())
		return -1;
	service.run();
	return 0;
}

