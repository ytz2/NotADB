#include "GrpcTransport.h"
#include <stdexcept>
#include <glog/logging.h>
#include "HealthService.h"

namespace lib {
namespace grpc {

GrpcTransport::GrpcTransport(const std::vector<std::string> &addresses) {
  if (addresses.empty()) {
    throw std::runtime_error("cannot pass empty addresses to transport");
  }
  // todo: validate address
  addresses_ = addresses;
  for (const auto &each: addresses) {
    builder_.AddListeningPort(each, ::grpc::InsecureServerCredentials());
    LOG(INFO) << "transport added " << each << " as listening address";
  }
}

bool GrpcTransport::start() {
  thread_ = std::thread([this]() {
    server_ = builder_.BuildAndStart();
    LOG(INFO) << "GRPC Started: ";
    for (const auto& addr : addresses_) {
      LOG(INFO) <<"\tlisten on " << addr <<" now";
    }
    server_->Wait();
  });
  return true;
}

void GrpcTransport::stop() {
  if (server_) server_->Shutdown();
}

bool GrpcTransport::initialize() {
  if (initialized_) return false;
  initialized_ = true;
  for (auto each: pullableServices_) {
    if (!each->initialize()) {
      LOG(ERROR) << each->name() << " failed to initialize";
      return false;
    }
  }
  // always start health service here
  builder_.RegisterService(new HealthService(this));
  LOG(INFO) << "GRPC transport initialized";
  return true;
}

const interface::ServingStatus GrpcTransport::status() const {
  interface::ServingStatus status = interface::ServingStatus::NOT_SERVING;
  for (auto each: pullableServices_) {
    if (each->status() != interface::ServingStatus::SERVING)
      return status;
  }
  return interface::ServingStatus::SERVING;
}

bool GrpcTransport::registerSupervisedService(interface::IService *svc) {
  if (svc == nullptr)
    return false;
  pullableServices_.push_back(svc);
  return true;
}

void GrpcTransport::registerGRPCService(::grpc::Service *svc) {
  builder_.RegisterService(svc);
}

}
}