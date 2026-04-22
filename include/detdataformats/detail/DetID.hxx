
namespace dunedaq::detdataformats {

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

inline std::string
DetID::subdetector_to_string(const Subdetector& type)
{
  switch (type) {
    case Subdetector::kDAQ:
      return "DAQ";
    case Subdetector::kHD_PDS:
      return "HD_PDS";
    case Subdetector::kHD_TPC:
      return "HD_TPC";
    case Subdetector::kHD_CRT:
      return "HD_CRT";
    case Subdetector::kVD_CathodePDS:
      return "VD_CathodePDS";
    case Subdetector::kVD_MembranePDS:
      return "VD_MembranePDS";
    case Subdetector::kVD_BottomTPC:
      return "VD_BottomTPC";
    case Subdetector::kVD_TopTPC:
      return "VD_TopTPC";
    case Subdetector::kVD_BernCRT:
      return "VD_BernCRT";
    case Subdetector::kVD_GrenobleCRT:
      return "VD_GrenobleCRT";
    case Subdetector::kNDLAr_TPC:
      return "NDLAr_TPC";
    case Subdetector::kNDLAr_PDS:
      return "NDLAr_PDS";
    case Subdetector::kND_GAr:
      return "ND_GAr";
    default:
      return "Unknown";
  }
}

inline DetID::Subdetector
DetID::string_to_subdetector(const std::string& typestring)
{
  if (typestring == "DAQ")
    return Subdetector::kDAQ;
  if (typestring == "HD_PDS")
    return Subdetector::kHD_PDS;
  if (typestring == "HD_TPC")
    return Subdetector::kHD_TPC;
  if (typestring == "HD_CRT")
    return Subdetector::kHD_CRT;
  if (typestring == "VD_CathodePDS")
    return Subdetector::kVD_CathodePDS;
  if (typestring == "VD_MembranePDS")
    return Subdetector::kVD_MembranePDS;
  if (typestring == "VD_BottomTPC")
    return Subdetector::kVD_BottomTPC;
  if (typestring == "VD_TopTPC")
    return Subdetector::kVD_TopTPC;
  if (typestring == "VD_BernCRT")
    return Subdetector::kVD_BernCRT;
  if (typestring == "VD_GrenobleCRT")
    return Subdetector::kVD_GrenobleCRT;
  if (typestring == "NDLAr_TPC")
    return Subdetector::kNDLAr_TPC;
  if (typestring == "NDLAr_PDS")
    return Subdetector::kNDLAr_PDS;
  if (typestring == "ND_GAr")
    return Subdetector::kND_GAr;

  return Subdetector::kUnknown;
}

} // namespace dunedaq::detdataformats
