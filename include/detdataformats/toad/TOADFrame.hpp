#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAME_HPP_

namespace dunedaq {
namespace detdataformats {
namespace toad {

class TOADFrame
{
  public:
  unsigned fec: 16;
  uint64_t tstmp : 64;
  bool hdr_par_check: 1;
  std::vector<unsigned short> toadsamples;
};
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif
