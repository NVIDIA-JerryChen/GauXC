#include <gauxc/xc_integrator/local_work_driver.hpp>
#include "host/reference_local_host_work_driver.hpp"
#include "device/cuda/cuda_aos_scheme1.hpp"
#include "device/hip/hip_aos_scheme1.hpp"

namespace GauXC {

LocalWorkDriverFactory::ptr_return_t
  LocalWorkDriverFactory::make_local_work_driver( ExecutionSpace ex, 
    std::string name, LocalWorkSettings settings ) {

  std::transform( name.begin(), name.end(), name.begin(), ::toupper );
  (void)(settings);

  switch(ex) {

  case ExecutionSpace::Host:
    if( name == "DEFAULT" ) name = "REFERENCE";

    if( name == "REFERENCE" )
      return std::make_unique<LocalHostWorkDriver>(
        std::make_unique<ReferenceLocalHostWorkDriver>()
      );
    else
      throw std::runtime_error("LWD Not Recognized");

  case ExecutionSpace::Device:
    if( name == "DEFAULT" ) name = "SCHEME1";

#ifdef GAUXC_ENABLE_DEVICE
    if( name == "SCHEME1" )
#ifdef GAUXC_ENABLE_CUDA
      return std::make_unique<LocalDeviceWorkDriver>(
        std::make_unique<CudaAoSScheme1>()
      );
#elif defined(GAUXC_ENABLE_HIP)
      return std::make_unique<LocalDeviceWorkDriver>(
        std::make_unique<HipAoSScheme1>()
      );
#endif
    else
#endif
      throw std::runtime_error("LWD Not Recognized");


  default:
    throw std::runtime_error("Execution Space Not Regognized");

  }

  return nullptr;

}

}
