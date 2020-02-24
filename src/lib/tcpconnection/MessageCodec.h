/*
 * MessageIO.h
 *
 *  Created on: Feb 9, 2020
 *      Author: yanhualiu
 */

#ifndef SRC_LIB_TCPCONNECTION_MESSAGECODEC_H_
#define SRC_LIB_TCPCONNECTION_MESSAGECODEC_H_

#include <interface/message.h>
#include <boost/asio.hpp>
#include <list>
#include <iostream>
#include <string>
#include "MessageBuffer.h"

namespace lib {
namespace tcp {

enum TCP_PROTOCOL_TYPE {
	FLAT, // header {msgType, size}[body]
	STREAM, // eg. stream style, no header at all
	NONE
};

typedef std::shared_ptr<boost::asio::mutable_buffer> BufferPtr;
class MessageCodec {
public:
	MessageCodec(const std::string &protocol) :
			protocol(protocol) {
	}
	virtual ~MessageCodec() {
	}
	;
	BufferPtr readBuffer(std::size_t size) {
		return std::make_shared<boost::asio::mutable_buffer>(
				buffer_.getBufferForRead(size), size);
	}

	void produce(std::size_t s) {
		buffer_.produce(s);
	}
	virtual BufferPtr writeBuffer(interface::IMessagePtr msg) = 0;
	virtual bool getMessages(std::vector<interface::IMessagePtr> &msgs) = 0;
protected:
	std::string protocol;
	std::list<std::string> writeMsgs_;
	MessageBuffer buffer_;
};
typedef std::shared_ptr<MessageCodec> MessageCodecPtr;
class FlatMessageDecoder: public MessageCodec {
public:
	FlatMessageDecoder(const std::string &protocol) :
			MessageCodec(protocol) {
	}

	virtual BufferPtr writeBuffer(interface::IMessagePtr msg) override;
	virtual bool getMessages(std::vector<interface::IMessagePtr> &msgs)
			override;
};

} /* namespace tcp */
} /* namespace lib */

#endif /* SRC_LIB_TCPCONNECTION_MESSAGECODEC_H_ */
