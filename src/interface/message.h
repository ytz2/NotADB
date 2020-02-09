#include <string>
#include <memory>
namespace interface {

class IMessage {
public:
	virtual bool ToString(std::string &output) = 0;
	virtual bool FromString(const std::string &input) = 0;
	virtual ~IMessage() {
	}
};

typedef std::shared_ptr<IMessage> IMessagePtr;
}
