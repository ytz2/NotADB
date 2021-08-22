/* Copyright 2020 Yanhua Liu */
#include "lib/tcpconnection/TcpConnection.h"
#include "message/commonjson/CommonJsonMessage.h"

using namespace std;       // NOLINT
using namespace lib::tcp;  // NOLINT

class SimplServerListener : public interface::IListener,
                            public interface::IConnectionCallback {
public:  // IListener
    bool start() override { return true; }

    bool shouldProcess(const interface::IMessagePtr) override { return true; }

    virtual bool onMessage(interface::ISessionPtr session,
                           const interface::IMessagePtr msg) {
        string body;
        if (msg->ToString(body)) {
            LOG(INFO) << "Received message from connection " << session->name()
                      << ": " << body;
            // heartbeat back
            if (!session->send(msg)) {
                LOG(ERROR) << "Failed to send back msg";
            }
            return true;
        }
        return false;
    }

public:
    SimplServerListener() = default;

    virtual ~SimplServerListener() {}

public:
    // hanldes message sent for logging/audit etc
    void onMessageSent(interface::ISessionPtr session,
                       const interface::IMessagePtr msg) override {
        string body;
        if (msg->ToString(body)) {
            LOG(INFO) << session->name() << " sent: " << body;
        } else {
            LOG(ERROR) << session->name() << " failed to deserialize msg sent "
                       << msg;
        }
    }

    void onDisconnect(interface::ISessionPtr session) override {
        LOG(INFO) << session->name() << " disconnected from server";
    }

    // handle connected
    void onConnect(interface::ISessionPtr session) override {
        LOG(INFO) << session->name() << " connected from server";
    }
};

int main() {
    boost::asio::io_service service;
    auto codec = std::make_shared<FlatMessageDecoder>("CommonJsonMessage");
    auto callback = std::make_shared<SimplServerListener>();
    auto server = std::make_shared<TcpServer>(service, 45679, codec);
    server->registerSessionCallback(callback);
    server->registerListner(callback);
    if (!server->start()) return -1;
    service.run();
    return 0;
}
