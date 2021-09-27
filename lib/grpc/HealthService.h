#include "rpc/HealthCheck/health_check.pb.h"
#include "rpc/HealthCheck/health_check.grpc.pb.h"
#include "interface/transport.h"

namespace lib {
namespace grpc {

class HealthService : public ::grpc::health::v1::Health::Service {
 public:
  HealthService(interface::ITransport *monitorTransport) {
    transport_ = monitorTransport;
  }
  HealthService() = delete;
  virtual ::grpc::Status Check(::grpc::ServerContext *context,
                               const ::grpc::health::v1::HealthCheckRequest *request,
                               ::grpc::health::v1::HealthCheckResponse *response) override {
    response->set_status(getStatus());
    return ::grpc::Status::OK;
  }

  virtual ::grpc::Status Watch(::grpc::ServerContext *context,
                               const ::grpc::health::v1::HealthCheckRequest *request,
                               ::grpc::ServerWriter<::grpc::health::v1::HealthCheckResponse> *writer) override {
    ::grpc::health::v1::HealthCheckResponse resp;
    resp.set_status(getStatus());
    writer->Write(resp);
    return ::grpc::Status::OK;
  }

 private:
  ::grpc::health::v1::HealthCheckResponse_ServingStatus getStatus() {
    if (!transport_) {
      return ::grpc::health::v1::HealthCheckResponse_ServingStatus_SERVICE_UNKNOWN;
    }
    auto status = transport_->status();
    switch (status) {
      case interface::ServingStatus::NOT_SERVING:return ::grpc::health::v1::HealthCheckResponse_ServingStatus_NOT_SERVING;
      case interface::ServingStatus::SERVING:return ::grpc::health::v1::HealthCheckResponse_ServingStatus_SERVING;
      default:return ::grpc::health::v1::HealthCheckResponse_ServingStatus_SERVICE_UNKNOWN;
    }
    return ::grpc::health::v1::HealthCheckResponse_ServingStatus_SERVICE_UNKNOWN;
  }

 private:
  interface::ITransport *transport_;
};

}
}