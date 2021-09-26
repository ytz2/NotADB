#pragma once
#include <string>

namespace interface {

enum class ServingStatus {
  UNKNOWN = 0,
  SERVING = 1,
  NOT_SERVING = 2,
  SERVICE_UNKNOWN = 3
};

class IService {
 public:
  // init the service, must explicitly init
  virtual bool initialize() = 0;
  // service name
  virtual const std::string& name() = 0;
  // readiness probe
  virtual const ServingStatus status() const = 0;
};
}