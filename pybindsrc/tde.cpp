/**
 * @file TDE16.cpp Python bindings for the TDE16Frame format
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
    .def("get_tde_header", static_cast<const TDE16Header* (TDE16Frame::*)() const >(&TDE16Frame::get_tde_header), py::return_value_policy::reference_internal)
    .def("get_timestamp", &TDE16Frame::get_timestamp, py::return_value_policy::reference_internal)
    .def("get_adc_samples", &TDE16Frame::get_adc_samples, py::return_value_policy::reference_internal)
    .def("set_timestamp", &TDE16Frame::set_timestamp)
    .def("set_TAItime", &TDE16Frame::set_TAItime)
    .def("set_tde_errors", &TDE16Frame::set_tde_errors)
    .def("set_adc_samples", &TDE16Frame::set_adc_samples)
    .def_static("sizeof", [](){ return sizeof(TDE16Frame); })
  ;

  py::class_<ADC16Data>(m, "ADC16Data", py::buffer_protocol())
    .def("get_adc_samples", &ADC16Data::get_adc_samples)
  ;

  py::class_<TDE16Header>(m, "TDE16Header")
    .def_property_readonly("version", [](TDE16Header& self) -> uint32_t {return self.version;})
    .def_property_readonly("det_id", [](TDE16Header& self) -> uint32_t {return self.det_id;})
    .def_property_readonly("crate", [](TDE16Header& self) -> uint32_t {return self.crate;})
    .def_property_readonly("slot", [](TDE16Header& self) -> uint32_t {return self.slot;})
    .def_property_readonly("link", [](TDE16Header& self) -> uint32_t {return self.link;})
    .def_property_readonly("tde_header", [](TDE16Header& self) -> uint32_t {return self.tde_header;})
    .def_property_readonly("tde_errors", [](TDE16Header& self) -> uint32_t {return self.tde_errors;})
    .def("get_timestamp", &TDE16Header::get_timestamp)
    .def("get_TAItime", &TDE16Header::get_TAItime)
  ;

}

} // namespace python
} // namespace tde
} // namespace timing
} // namespace dunedaq