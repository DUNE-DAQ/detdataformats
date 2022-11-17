#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TOADFRAME_HPP_

namespace dunedaq {
namespace detdataformats {
namespace toad {

class TOADFrame
{
  public:
  unsigned fec: 10;
  unsigned channel: 5;
  unsigned tstmp: 32;
  bool hdr_par_check: 1;
  bool data_par_check: 1;
  std::vector<unsigned short> toadsamples;
  //unsigned short toadsamples[];

  //toadpacket toadpkt;
  /*uint64_t get_timestamp() const {
    return (uint64_t)tstmp;
  }
  void set_timestamp(uint32_t new_tstmp){
    tstmp = new_tstmp;
  }
  std::size_t get_size() {
    //printf("size class %d\n", sizeof(*this));
    //int size = 40;
    return sizeof(*this);
  }
*/
  //toadpacket* get_toad_packet(void* msg) const {
  //  return static_cast<toadpacket*>(msg);
  //} 
};

} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
#endif
