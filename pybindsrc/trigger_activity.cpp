/**
 * @file trigger_primitive.cpp Python bindings for the trigger primitive formats
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/trigger/TriggerActivity.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace trigger {
namespace python {

void
register_trigger_activity(py::module& m)
{


  py::enum_<TriggerActivityData::Type>(m, "TriggerActivityData::Type")
    .value("kUnknown", TriggerActivityData::Type::kUnknown)
    .value("kTPC", TriggerActivityData::Type::kTPC)
    .value("kPDS", TriggerActivityData::Type::kPDS)
    .export_values();

  py::enum_<TriggerActivityData::Algorithm>(m, "TriggerActivityData::Algorithm")
    .value("kUnknown", TriggerActivityData::Algorithm::kUnknown)
    .value("kSupernova", TriggerActivityData::Algorithm::kSupernova)
    .value("kPrescale", TriggerActivityData::Algorithm::kPrescale)
    .value("kADCSimpleWindow", TriggerActivityData::Algorithm::kADCSimpleWindow)
    .value("kHorizontalMuon", TriggerActivityData::Algorithm::kHorizontalMuon)
    .value("kMichelElectron", TriggerActivityData::Algorithm::kMichelElectron)
    .value("kDBSCAN", TriggerActivityData::Algorithm::kDBSCAN)
    .value("kLowEnergyEvent", TriggerActivityData::Algorithm::kLowEnergyEvent)
    .export_values();

  py::class_<TriggerActivityData>(m, "TriggerActivityData", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto tp = *static_cast<TriggerActivityData*>(capsule.get_pointer());
        return tp;
		  } ))
    .def(py::init([](py::bytes bytes){
      py::buffer_info info(py::buffer(bytes).request());
      auto tp = *static_cast<TriggerActivityData*>(info.ptr);
      return tp;
    }))
    .def_property_readonly("version", [](TriggerActivityData& self) -> uint16_t {return self.version;})
    .def_property_readonly("time_start", [](TriggerActivityData& self) -> uint64_t {return self.time_start;})
    .def_property_readonly("time_end", [](TriggerActivityData& self) -> uint64_t {return self.time_end;})
    .def_property_readonly("time_peak", [](TriggerActivityData& self) -> uint64_t {return self.time_peak;})
    .def_property_readonly("time_activity", [](TriggerActivityData& self) -> uint64_t {return self.time_activity;})
    .def_property_readonly("channel_start", [](TriggerActivityData& self) -> int32_t {return self.channel_start;})
    .def_property_readonly("channel_end", [](TriggerActivityData& self) -> int32_t {return self.channel_end;})
    .def_property_readonly("channel_peak", [](TriggerActivityData& self) -> int32_t {return self.channel_peak;})
    .def_property_readonly("adc_integral", [](TriggerActivityData& self) -> uint32_t {return self.adc_integral;})
    .def_property_readonly("adc_peak", [](TriggerActivityData& self) -> uint32_t {return self.adc_peak;})
    .def_property_readonly("detid", [](TriggerActivityData& self) -> uint16_t {return self.detid;})
    .def_property_readonly("type", [](TriggerActivityData& self) -> TriggerActivityData::Type {return self.type;})
    .def_property_readonly("algorithm", [](TriggerActivityData& self) -> TriggerActivityData::Algorithm {return self.algorithm;})
    .def_static("sizeof", [](){ return sizeof(TriggerActivityData); })
    ;

    py::class_<TriggerActivity>(m, "TriggerActivity", py::buffer_protocol())
      .def(py::init())
      .def(py::init([](py::capsule capsule) {
          auto tp = *static_cast<TriggerActivity*>(capsule.get_pointer());
          return tp;
        } ))
      .def(py::init([](py::bytes bytes){
        py::buffer_info info(py::buffer(bytes).request());
        auto tp = *static_cast<TriggerActivity*>(info.ptr);
        return tp;
      }))
      .def_property_readonly("data", [](TriggerActivity& self) -> TriggerActivityData& {return self.data;})
      .def("__len__", [](TriggerActivity& self){ return self.n_inputs; })
      .def("__getitem__",
            [](const TriggerActivity &self, size_t i) {
                if (i >= self.n_inputs) {
                    throw py::index_error();
                }
                return self.inputs[i];
            })
      .def("sizeof", [](TriggerActivity& self){ return sizeof(TriggerActivityData)+sizeof(uint64_t)+self.n_inputs*sizeof(TriggerPrimitive); })
      
      ;
}

} // namespace python
} // namespace trigger
} // namespace timing
} // namespace dunedaq
