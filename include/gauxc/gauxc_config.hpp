#pragma once

#ifdef __CUDACC__
  #define HOST_DEVICE_ACCESSIBLE __host__ __device__
#else
  #define HOST_DEVICE_ACCESSIBLE
#endif
  
#ifdef GAUXC_ENABLE_CEREAL
  #include <cereal/types/vector.hpp>
  #include <cereal/types/array.hpp>
#endif
