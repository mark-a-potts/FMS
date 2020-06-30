# (C) Copyright 2018-2020 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

# Standard FMS compiler definitions
add_definitions( -Duse_libMPI -DSPMD -Duse_netCDF -Duse_LARGEFILE )

# Add platform specific compiler definitions
if( APPLE )
  add_definitions( -D__APPLE__ )
endif()

if (FV3_PRECISION=SINGLE)
  add_definitions( -OVERLOAD_R4 )
  add_definitions( -OVERLOAD_R8 )
endif()

#######################################################################################
# Fortran
#######################################################################################

if( CMAKE_Fortran_COMPILER_ID MATCHES "GNU" )
  include( compiler_flags_GNU_Fortran )
elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Intel" )
  include( compiler_flags_Intel_Fortran )
elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Clang" )
  include( compiler_flags_Clang_Fortran )
else()
  message( STATUS "Fortran compiler with ID ${CMAKE_CXX_COMPILER_ID} will be used with CMake default options")
endif()

#######################################################################################
# C
#######################################################################################

if( CMAKE_C_COMPILER_ID MATCHES "GNU" )
  include( compiler_flags_GNU_C )
elseif( CMAKE_C_COMPILER_ID MATCHES "Intel" )
  include( compiler_flags_Intel_C )
elseif( CMAKE_C_COMPILER_ID MATCHES "Clang" )
  include( compiler_flags_Clang_C )
else()
  message( STATUS "C compiler with ID ${CMAKE_CXX_COMPILER_ID} will be used with CMake default options")
endif()
