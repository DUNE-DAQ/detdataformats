
namespace dunedaq::detdataformats {

static_assert(DetID::s_det_id_version == 1,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");
static_assert(sizeof(DetID) == 4, "DetID struct size different than expected!");
static_assert(offsetof(DetID, version) == 0, "DetID version field not at expected offset");
static_assert(offsetof(DetID, subdetector) == 2, "DetID subdetector field not at expected offset");

/**
 * @brief Stream a Subdetector instance in a human-readable form
 * @param o Stream to output to
 * @param id Subdetector to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, DetID::Subdetector const& type)
{
  return o << DetID::subdetector_to_string(type);
}

/**
 * @brief Stream a DetID instance in a human-readable form
 * @param o Stream to output to
 * @param id DetID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, DetID const& det_id)
{
  return o << "subdetector: " << det_id.subdetector ;
}

/**
 * @brief Read a DetID::Subdetector from a string stream
 * @param is Stream to read from
 * @param id Subdetector to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, DetID::Subdetector& t)
{
  std::string tmp;
  is >> tmp;

  t = DetID::string_to_subdetector(tmp);

  return is;
}
/**
 * @brief Read a DetID from a string stream
 * @param is Stream to read from
 * @param id DetID to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, DetID& det_id)
{
  std::string tmp;
  is >> tmp >> det_id.subdetector; 

  return is;
}

std::string
DetID::subdetector_to_string(const Subdetector& type)
{
  switch (type) {
    case Subdetector::kHDPDS:
      return "HDPDS";
    case Subdetector::kHDTPC:
      return "HDTPC";
    case Subdetector::kHDCRT:
      return "HDCRT";
    case Subdetector::kVDCPDS:
      return "VDCPDS";
    case Subdetector::kVDMPDS:
      return "VDMPDS";
    case Subdetector::kVDBTPC:
      return "VDBTPC";
    case Subdetector::kVDTTPC:
      return "VDTTPC";
    case Subdetector::kNDLAR:
      return "NDLAR";
    case Subdetector::kNDGAR:
      return "NDGAR";
    default:
      return "UNDEFINED";
  }
}

DetID::Subdetector
DetID::string_to_subdetector(const std::string& typestring)
{
  if (typestring == "HDPDS")
    return Subdetector::kHDPDS;
  if (typestring == "HDTPC")
    return Subdetector::kHDTPC;
  if (typestring == "HDCRT")
    return Subdetector::kHDTPC;
  if (typestring == "VDCPDS")
    return Subdetector::kVDCPDS;
  if (typestring == "VDMPDS")
    return Subdetector::kVDMPDS;
  if (typestring == "VDBTPC")
    return Subdetector::kVDBTPC;
  if (typestring == "VDTTPC")
    return Subdetector::kVDTTPC;
  if (typestring == "NDLAR")
    return Subdetector::kNDLAR;
  if (typestring == "NDGAR")
    return Subdetector::kNDGAR;

  return Subdetector::kUNDEFINED;
}

} // namespace dunedaq::detdataformats
