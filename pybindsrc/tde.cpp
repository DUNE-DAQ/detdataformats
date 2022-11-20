/**
 * @file wib2.cpp Python bindings for the TDE16Frame format
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/tde/TDE16Frame.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace tde {
namespace python {

void
register_tde(py::module& m)
{

  py::class_<TDE16Frame>(m, "TDE16Frame", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<TDE16Frame*>(capsule.get_pointer());
        return wfp;
    } ))
    .def(py::init([](py::bytes bytes){
      py::buffer_info info(py::buffer(bytes).request());
      auto wfp = *static_cast<TDE16Frame*>(info.ptr);
      return wfp;
    }))
    .def("get_timestamp", &TDE16Frame::get_timestamp)
    .def("set_timestamp", &TDE16Frame::set_timestamp)
    .def("get_tde_header", [](TDE16Frame& self) -> TDE16Header* {return self.get_tde_header();}, py::return_value_policy::reference_internal)
    .def_static("sizeof", [](){ return sizeof(TDE16Frame); })
    .def("get_bytes",
         [](TDE16Frame* fr) -> py::bytes {
           return py::bytes(reinterpret_cast<char*>(fr), sizeof(TDE16Frame));
        }
    )
  ;

  py::class_<TDE16Header>(m, "TDE16Header")
    .def_property_readonly("version", [](TDE16Header& self) -> uint32_t {return self.version;})
    .def_property_readonly("det_id", [](TDE16Header& self) -> uint32_t {return self.det_id;})
    .def_property_readonly("crate", [](TDE16Header& self) -> uint32_t {return self.crate;})
    .def_property_readonly("slot", [](TDE16Header& self) -> uint32_t {return self.slot;})
    .def_property_readonly("link", [](TDE16Header& self) -> uint32_t {return self.link;})
    .def_property_readonly("timestamp_1", [](TDE16Header& self) -> uint32_t {return self.timestamp_1;})
    .def_property_readonly("timestamp_2", [](TDE16Header& self) -> uint32_t {return self.timestamp_2;})
  ;
}

} // namespace python
} // namespace wib2
} // namespace detdataformats
} // namespace dunedaq
