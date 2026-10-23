/**
 * @file geoid.cpp Python bindings for the GeoID struct
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/GeoID.hpp"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <functional>
#include <sstream>

namespace py = pybind11;

namespace dunedaq::detdataformats::python {

void
register_geoid(py::module& m)
{
  py::class_<GeoID>(m, "GeoID")
    .def(py::init())
    .def(py::init<DetID::Subdetector, uint16_t, uint16_t, uint16_t>(), // NOLINT(build/unsigned)
         py::arg("subdetector"),
         py::arg("crate_id"),
         py::arg("slot_id"),
         py::arg("stream_id"))
    .def_readwrite("detector_id", &GeoID::detector_id)
    .def_readwrite("crate_id", &GeoID::crate_id)
    .def_readwrite("slot_id", &GeoID::slot_id)
    .def_readwrite("stream_id", &GeoID::stream_id)
    .def("subdetector", &GeoID::subdetector)
    .def("encode", &GeoID::encode)
    .def_static("decode", &GeoID::decode, py::arg("value"))
    .def("to_string", &GeoID::to_string)
    .def(py::self == py::self)
    .def(py::self < py::self)
    .def("__hash__", [](const GeoID& geo_id) { return std::hash<GeoID::encoded_t>{}(geo_id.encode()); })
    .def("__repr__", [](const GeoID& geo_id) {
      std::ostringstream oss;
      oss << "<detdataformats::GeoID " << geo_id << ">";
      return oss.str();
    });
}

} // namespace dunedaq::detdataformats::python
