
#include <sstream>
#include <string>

namespace dunedaq::detdataformats {

/**
 * @brief Stream a GeoID instance in a human-readable form
 * @param o Stream to output to
 * @param geo_id GeoID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, GeoID const& geo_id)
{
  return o << "subdetector: " << geo_id.subdetector() << ", crate: " << geo_id.crate_id << ", slot: " << geo_id.slot_id
           << ", stream: " << geo_id.stream_id;
}

inline std::string
GeoID::to_string() const
{
  std::ostringstream oss;
  oss << *this;
  return oss.str();
}

} // namespace dunedaq::detdataformats
