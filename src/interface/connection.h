#include <vector>
#include <unordered_map>
#include <memory>
#include <atomic>
#include "message.h"
namespace interface {

	class IListener {
		public:
			virtual bool start() = 0;
			virtual bool shouldProcess(const IMessagePtr) = 0;
			virtual bool onMessage(const IMessagePtr) = 0;
			virtual ~IListener() = 0;
	};
	typedef std::shared_ptr<IListner> IListnerPtr;

	class IConnectionCallback;
	typedef std::shared_ptr< IConnectionCallback > IConnectionCallbackPtr

	class ISession {
	public:
		virtual void disconnect() = 0;
		virtual bool send(const IMessagePtr msg) = 0;
		virtual bool connected() {
			return connected_.load();
		}
		const std::string& name() {
			return name_;
		}
		void registerSessionCallback(IConnectionCallbackPtr cb) {
			sessionCallbacks_.push_back(cb);
		}
		void registerListner(IListnerPtr cb) {
			appCallbacks_.push_back(cb);
		}
		virtual ~ISession() = 0;
	protected:
		// uuid
		std::string name_;
		// state
		std::atomic<bool> connected_;
		// when session is disconnected, do sessionCallbacks each by each
		std::vector< IConnectionCallbackPtr > sessionCallbacks_;
		std::vector< IListnerPtr > appCallbacks_;
	};

	typedef std::shared_ptr<ISession> ISessionPtr;

	class IConnectionCallback {
	public:
		// handles session layer disconnect
	typedef std::shared_ptr<ISession> ISessionPtr;
		virtual void onDisconnect(ISessionPtr) = 0;
		// hanldes message sent for logging/audit etc
		virtual void onMessageSent(ISessionPtr, const IMessagePtr) = 0;
		virtual ~IConnectionCallback() = 0;
	};

	class IAcceptor : public IConnectionCallback {
	public:
		// managment acceptor state
		virtual bool start() = 0;
		virtual void stop() {
			for (auto& session : sessions_) {
				if (session.second->connected()) {
					session.second->disconnect();
				}	
			}
		}
		// for broadcast
		virtual bool send(const IMessagePtr msg) {
			bool res = true;
			for (auto& session : sessions_) {
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
	protected:
		// manage sessions here
		std::unordered_map<std::string, ISessionPtr > sessions_;
	};
	typedef std::shared_ptr< IAcceptor > IAcceptorPtr;

}