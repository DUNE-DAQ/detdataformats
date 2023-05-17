#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAMEOVERLAY_HPP_

#include <cstdint>
#include <cstddef>
#include <cstdio>

namespace dunedaq {
namespace detdataformats {
namespace toad {

struct TOADFrameOverlay {
  uint64_t tstmp;
  size_t n_samples;
  size_t n_bytes;
  uint64_t fec : 16;
  uint64_t hdr_par_check : 1;
  uint64_t padding1 : 15;
  uint64_t padding2 : 32;
  //char toadsample_test[100];
  uint16_t toadsamples[];

  uint64_t get_timestamp() const {
    return tstmp;
  }
  
  void set_timestamp(uint64_t new_tstmp){
    tstmp = new_tstmp;
  }
 
  size_t get_size() const {
    return n_bytes;
  }

  int get_n_samples() const {
    return n_samples;
  }

  int get_first_sample() const {
    printf("%p, %p, %p", (void*)&n_bytes, (void*)&toadsamples[0], (void*)&toadsamples[n_samples]); 
    return toadsamples[0];
  }

  unsigned short get_samples(int j)  const {
    return toadsamples[j];
  } 
 
};
 
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif                           

