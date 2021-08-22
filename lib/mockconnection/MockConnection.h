/* Copyright 2020 Yanhua Liu */
#pragma once

#include <glog/logging.h>

#include <boost/asio.hpp>
#include <exception>      // NOLINT
#include <iostream>       // NOLINT
#include <memory>         // NOLINT
#include <queue>          // NOLINT
#include <string>         // NOLINT
#include <unordered_map>  // NOLINT
#include <utility>        // NOLINT

#include "interface/connection.h"
#include "interface/message.h"
#include "message/commonjson/CommonJsonMessage.h"

namespace lib {
    namespace mock {
        typedef lib::message::commonjson::CommonJsonMessage MockMessage;

        enum MockSide {
            ADMIN, SERVER, CLIENT, NA
        };

        template<class T>
        class MockIO {
        public:
            typedef std::queue <T> MockQueue;

            explicit MockIO(const std::string &name) : name_(name), ready_(true) {
                qs_.emplace(ADMIN, std::move(std::queue<T>()));
                qs_.emplace(SERVER, std::move(std::queue<T>()));
                qs_.emplace(CLIENT, std::move(std::queue<T>()));
            }

            MockIO(const MockIO &rhs) = delete;

            bool closed() const { return !ready_; }

            void close() { ready_ = false; }

            std::string getName() { return name_; }

            void produce(T msg, MockSide t) { produce(msg, getq(t)); }

            MockQueue consume(MockSide t) { return consume(getq(t)); }

        private:
            MockQueue &getq(MockSide t) {
                std::lock_guard <std::mutex> gd(lock_);
                if (t >= NA) {
                    throw std::runtime_error("unknown mock queue type is passed");
                }
                return qs_[t];
            }

            void produce(T msg, MockQueue &q) {  // NOLINT
                std::lock_guard <std::mutex> gd(lock_);
                q.push(msg);
            }

            MockQueue consume(MockQueue &q) {  // NOLINT
                std::lock_guard <std::mutex> gd(lock_);
                MockQueue tmp;
                q.swap(tmp);
                return tmp;
            }

        private:
            std::string name_;
            mutable std::mutex lock_;
            std::atomic_bool ready_;
            std::unordered_map <MockSide, MockQueue> qs_;
        };

        typedef MockIO<interface::IMessagePtr> MockTcp;
        typedef std::shared_ptr <MockTcp> MockTcpPtr;
        typedef MockIO<MockTcpPtr> MockTcpMgr;

        class MockAcceptor : public interface::IAcceptor,
                             public interface::IConnectionCallback,
                             public std::enable_shared_from_this<MockAcceptor> {
        public:  // MockAcceptors
            MockAcceptor() = delete;

            MockAcceptor(MockTcpMgr &tcpMgr, boost::asio::io_service &io);  // NOLINT

        public:  // IAcceptor
            bool start() override;

            void stop() override;

        public:  // IConnectionCallback
            void onDisconnect(interface::ISessionPtr session) override;

            void onMessageSent(interface::ISessionPtr,
                               const interface::IMessagePtr) override;

            void onConnect(interface::ISessionPtr) override {}  // noop
        public:
            void accept();

        private:
            MockTcpMgr &mgr_;
            boost::asio::io_service &io_;
            std::shared_ptr <boost::asio::deadline_timer> t_;
        };

        class MockConnection : public interface::ISession {
        public:
            explicit MockConnection(MockTcp &tcp, boost::asio::io_service &io,  // NOLINT
                                    MockSide side);

            virtual ~MockConnection() {}

            void disconnect() override;

            bool send(const interface::IMessagePtr msg) override;

            bool start() override;

        public:
            void poll();

        private:
            MockTcp &tcp_;
            MockSide side_;
            std::shared_ptr <boost::asio::deadline_timer> t_;
        };

        class MockListener : public interface::IListener {
        public:
            explicit MockListener(MockSide side) : side_(side) {}

            virtual bool start() { return true; }

            virtual bool shouldProcess(const interface::IMessagePtr) { return true; }

            virtual bool onMessage(interface::ISessionPtr session,
                                   const interface::IMessagePtr msg) {
                std::string buf;
                if (msg && msg->ToString(buf)) {
                    LOG(INFO) << (side_ == SERVER ? "server" : "client")
                              << " with session: " << session->name() << " listener received"
                              << buf << std::endl;
                    if (side_ == SERVER && session && session->connected()) {
                        auto m = std::static_pointer_cast<MockMessage>(msg);
                        if (!m) return false;
                        (*m)["signature"] = "server";
                        return session->send(msg);
                    }
                } else {
                    return false;
                }
                return true;
            }

            virtual ~MockListener() {}

        private:
            MockSide side_;
        };

    }  // namespace mock
}  // namespace lib
