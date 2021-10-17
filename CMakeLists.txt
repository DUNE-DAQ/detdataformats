cmake_minimum_required(VERSION 3.12)
project(detdataformats VERSION 1.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

#find_package(cetlib REQUIRED)
find_package(ers REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)

##############################################################################
# Main library

daq_add_library(*.cpp LINK_LIBRARIES ers::ers)

##############################################################################
# Plugins

#daq_add_plugin(... ... LINK_LIBRARIES daq-rawdata)

##############################################################################
# Integration tests



##############################################################################
# Unit Tests

daq_add_unit_test(WIBFrame_test LINK_LIBRARIES detdataformats)
daq_add_unit_test(WIB2Frame_test LINK_LIBRARIES detdataformats)

##############################################################################

daq_install()
