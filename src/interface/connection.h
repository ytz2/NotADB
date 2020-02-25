#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <json/json.h>
#include <interface/message.h>
namespace interface {

class IMessage;
class ISession;
typedef std::shared_ptr<ISession> ISessionPtr;
class IListener {
public:
	virtual bool start() = 0;
	virtual bool shouldProcess(const IMessagePtr) = 0;
	virtual bool onMessage(ISessionPtr session, const IMessagePtr) = 0;
	virtual ~IListener() {
	}
};
typedef std::shared_ptr<IListener> IListnerPtr;

class IConnectionCallback {
public:
	// handles session layer disconnect
	virtual void onDisconnect(ISessionPtr) = 0;
	// hanldes message sent for logging/audit etc
	virtual void onMessageSent(ISessionPtr, const IMessagePtr) = 0;

	// handle connected
	virtual void onConnect(ISessionPtr) = 0;
	virtual ~IConnectionCallback() {
	}
};

typedef std::shared_ptr<IConnectionCallback> IConnectionCallbackPtr;

class ICallbackContainer {
public:
	ICallbackContainer() = default;
	void registerSessionCallback(IConnectionCallbackPtr cb) {
		sessionCallbacks_.push_back(cb);
	}
	void registerListner(IListnerPtr cb) {
		appCallbacks_.push_back(cb);
	}
	virtual void onMessage(const IMessagePtr msg) = 0;
	virtual void onConnected() = 0;
	virtual void onDisconnect() = 0;
	virtual ~ICallbackContainer() {
	}
protected:
	// when session is disconnected, do sessionCallbacks each by each
	std::vector<IConnectionCallbackPtr> sessionCallbacks_;
	std::vector<IListnerPtr> appCallbacks_;
};

class ISession: public ICallbackContainer, public std::enable_shared_from_this<
		ISession> {
public:
	ISession(const std::string &name) :
			name_(name), connected_(true) {
	}
	ISession(const std::string &name, bool connected) :
			name_(name), connected_(connected) {
	}
	virtual void disconnect() {
		connected_ = false;
		onDisconnect();
	}
	virtual bool send(const IMessagePtr msg) = 0;
	virtual bool start() = 0;
	virtual bool connected() {
		return connected_;
	}
	const std::string& name() {
		return name_;
	}
	virtual ~ISession() {
	}
protected:
	// ICallbackContainer
	virtual void onMessage(const IMessagePtr msg) override {
		for (auto l : appCallbacks_) {
			if (l->shouldProcess(msg)) {
				l->onMessage(shared_from_this(), msg);
			}
		}
	}
	virtual void onDisconnect() override {
		connected_ = false;
		for (auto l : sessionCallbacks_) {
			l->onDisconnect(shared_from_this());
		}
	}

	virtual void onConnected() override {
		connected_ = true;
		for (auto l : sessionCallbacks_) {
			l->onConnect(shared_from_this());
		}
	}
protected:
	// uuid
	std::string name_;
	// state
	std::atomic<bool> connected_;
};

typedef std::shared_ptr<ISession> ISessionPtr;

class IAcceptor {
public:
	// managment acceptor state
	virtual bool start() {
		ready_ = true;
		return true;
	}
	virtual void stop() {
		ready_ = false;
		std::lock_guard<std::mutex> lk(lock_);
		for (auto &session : sessions_) {
			if (session.second->connected()) {
				session.second->disconnect();
			}
		}
	}
	bool isReady() const {
		return ready_;
	}
	// for broadcast
	virtual bool send(const IMessagePtr msg) {
		if (!ready_)
			return false;
		bool res = true;
		std::lock_guard<std::mutex> lk(lock_);
		for (auto &session : sessions_) {
			if (session.second->connected()) {
				res = res && session.second->send(msg);
			}
		}
		return res;
	}
	virtual ~IAcceptor() {
		stop();
		sessions_.clear();
	}
	void addSession(const std::string &name, ISessionPtr s) {
		std::lock_guard<std::mutex> lk(lock_);
		sessions_[name] = s;
	}
	bool removeSession(const std::string &name) {
		std::lock_guard<std::mutex> lk(lock_);
		if (!sessions_.count(name))
			return false;
		sessions_.erase(name);
		return true;
	}
protected:
	std::atomic<bool> ready_;
	std::unordered_map<std::string, ISessionPtr> sessions_;
	std::mutex lock_;
};
typedef std::shared_ptr<IAcceptor> IAcceptorPtr;

}
