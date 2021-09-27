#include <chrono>
#include <thread>
#include <glog/logging.h>
#include "lib/grpc/GrpcTransport.h"

int main() {
  lib::grpc::GrpcTransport transport({"0.0.0.0:30555", "0.0.0.0:30556"});
  if (!transport.initialize()){
    LOG(ERROR) << "failed to initialize";
    return -1;
  }
  if (!transport.start()) {
    LOG(ERROR) << "failed to start";
    return -2;
  }
  while(true)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}

