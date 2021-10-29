cmake_minimum_required(VERSION 3.12)
project(detdataformats VERSION 3.1.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()


##############################################################################
# Main library

daq_add_library(*.cpp LINK_LIBRARIES)

##############################################################################
# Plugins

##############################################################################
# Integration tests

##############################################################################
# Unit Tests
daq_add_unit_test(WIBFrame_test                LINK_LIBRARIES dataformats)
daq_add_unit_test(WIB2Frame_test                LINK_LIBRARIES dataformats)

##############################################################################

daq_install()
