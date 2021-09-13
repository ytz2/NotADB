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

#include <iostream>
#include <string>

namespace lib {
namespace tcp {
class Producer {
 public:
  Producer() {
    if (producer_)
      return;
    std::cout << producer_;
  }

 private:
  kafka::KafkaProducer *producer_;
};

}
}

#endif //DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_
