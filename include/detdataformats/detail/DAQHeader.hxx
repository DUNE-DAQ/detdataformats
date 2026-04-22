
#include <type_traits>
#include <bit>
#include <cstddef>

namespace dunedaq::detdataformats {

static_assert(std::is_trivially_copyable_v<DAQHeader>, "DAQHeader isn't trivially copyable and can't be safely std::memcpy'd");
static_assert(std::is_standard_layout_v<DAQHeader>, "DAQHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");
static_assert(std::endian::native == std::endian::little,
              "The DAQHeader bitfield layout assumes little-endian architecture");

static_assert(sizeof(DAQHeader) == 12, "DAQHeader struct size different than expected!");
static_assert(offsetof(DAQHeader, timestamp_1) == 4, "DAQHeader timestamp_1 field not at expected offset");
static_assert(offsetof(DAQHeader, timestamp_2) == 8, "DAQHeader timestamp_2 field not at expected offset");


inline std::ostream&
operator<<(std::ostream& o, DAQHeader const& h)
{
  return o << "Version:" << unsigned(h.version) << " DetID:" << unsigned(h.det_id) << " CrateID:" << unsigned(h.crate_id)
           << " SlotID:" << unsigned(h.slot_id) << " LinkID:" << unsigned(h.link_id)
           << " Timestamp: " << h.get_timestamp() << '\n';
}

} // namespace dunedaq::detdataformats
