//
// Created by Danni Song on 9/12/21.
//

#ifndef DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_
#define DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_

// resolve the conflict due to modern cpp kafka conflict

#ifdef DEBUG
#undef DEBUG
#endif

#include "kafka/KafkaProducer.h"
#include <glog/logging.h>
#include "interface/message.h"
#include "lib/config/Configuration.h"

#include <iostream>
#include <string>

namespace lib {
namespace kafka {

class Producer {
 public:
  Producer() {
    LOG(INFO) << producer_;
  };
  virtual ~Producer() {
    if (producer_)
      delete producer_;
  };

 private:
  ::kafka::KafkaProducer *producer_ = nullptr;
};

}
}

#endif //DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_
