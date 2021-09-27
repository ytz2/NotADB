#pragma once
#include <string>
#include <memory>
#include "service.h"

namespace interface {

enum class TransportType {
  GRPC = 0,
  HTTP = 1,
  UNKNOWN = 2
};

class ITransport {
 public:
  virtual bool start() = 0;
  virtual void stop() = 0;
  virtual bool initialize() = 0;
  virtual const ServingStatus status() const = 0;
  virtual bool registerSupervisedService(IService* svc) = 0;
  //TODO stats api
 public:
  const TransportType getTransportType() const {
    return transportType_;
  }

  const std::string transportType() const {
    switch (transportType_) {
      case TransportType::GRPC:
        return "GRPC";
      case TransportType::HTTP:
        return "HTTP";
      default:
        return "UNKNOWN";
    }
    return "UNKNOWN";
  }
 protected:
  TransportType transportType_ = TransportType::UNKNOWN; // default
};

}