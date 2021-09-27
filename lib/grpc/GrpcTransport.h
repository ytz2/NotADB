#pragma once
#include "interface/transport.h"
#include "grpcpp/grpcpp.h"
#include <vector>
#include <string>
#include <thread>

namespace lib {
namespace grpc {

class GrpcTransport : public interface::ITransport {
 public:
  // implement proxy logic colo within one transport, this is important to the deploy model
  GrpcTransport(const std::vector<std::string> &addresses);
  GrpcTransport() = delete;
  virtual ~GrpcTransport() = default;
  virtual bool start() override;
  virtual void stop() override;
  virtual bool initialize() override;
  virtual const interface::ServingStatus status() const override;
  // a service which needs to be initialized and monitored
  virtual bool registerSupervisedService(interface::IService *svc) override; // status related
 public:
  // a grpc service must be registered here
  virtual void registerGRPCService(::grpc::Service *svc);
 private:
  std::vector<interface::IService *> pullableServices_;
  std::vector<std::string> addresses_;
  ::grpc::ServerBuilder builder_;
  bool initialized_ = false;
  std::unique_ptr<::grpc::Server> server_ = nullptr;
  std::thread thread_;
};

}
}