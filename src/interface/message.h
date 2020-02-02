#include <string>
#include <memory>s
namespace interface {

	class IMessage : public iSerializable, public iDeserializable {
		virtual bool ToString(std::string& output) = 0;
		virtual bool FromString(const std::string& input) = 0;
		virtual ~IMessage() = 0;
	};

	typedef std::shared_ptr<IMessage> IMessagePtr;
}