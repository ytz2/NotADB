/* Copyright 2020 Yanhua Liu */
#include "lib/tcpconnection/TcpConnection.h"
#include "message/commonjson/CommonJsonMessage.h"

using namespace std;       // NOLINT
using namespace lib::tcp;  // NOLINT
using boost::asio::ip::tcp;

class SimpleClientListener : public interface::IListener,
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
            return true;
        }
        return false;
    }

public:
    explicit SimpleClientListener(boost::asio::io_service &io)  // NOLINT
            : timer_(std::make_shared<boost::asio::deadline_timer>(io)),
              session_(nullptr) {}

    virtual ~SimpleClientListener() {}

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
        session_ = nullptr;
        timer_->cancel();
        LOG(INFO) << session->name() << " disconnected from listener";
    }

    // handle connected
    void onConnect(interface::ISessionPtr session) override {
        session_ = session;
        LOG(INFO) << session_->name() << " established, start to hearbeat";
        boost::system::error_code e;
        heartbeat(e);
        LOG(INFO) << session->name() << " connected from listener";
    }

private:
    void heartbeat(const boost::system::error_code &e) {
        if (session_ == nullptr) return;
        if (session_->connected()) {
            auto msg =
                    std::make_shared<lib::message::commonjson::CommonJsonMessage>();
            auto &v = *msg;
            v["msg"] = "heartbeat";
            session_->send(msg);
        }
        timer_->expires_from_now(boost::posix_time::seconds(3));
        auto func = std::bind(&SimpleClientListener::heartbeat, this,
                              std::placeholders::_1);
        timer_->async_wait(func);
    }

private:
    std::shared_ptr <boost::asio::deadline_timer> timer_;
    interface::ISessionPtr session_;
};

int main() {
    auto codec = std::make_shared<FlatMessageDecoder>("CommonJsonMessage");
    boost::asio::io_service service;
    auto client = std::make_shared<TcpClientSession>(service, "SimpleClient",
                                                     "0.0.0.0", "45679", codec);
    client->setRetryIntervalMill(3000);
    client->setTimeoutIntervalMill(100);
    auto listner = std::make_shared<SimpleClientListener>(service);
    client->registerListner(listner);
    client->registerSessionCallback(listner);
    if (!client->start()) {
        LOG(ERROR) << "Failed to start the tcp client";
        return -1;
    }
    service.run();
    return 0;
}
