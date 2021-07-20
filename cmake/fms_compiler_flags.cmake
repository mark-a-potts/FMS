# (C) Copyright 2018-2020 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.


# Standard FMS compiler definitions
# ---------------------------------
add_definitions( -Duse_libMPI -DSPMD -Duse_netCDF -Duse_LARGEFILE -DGFS_PHYS -DINTERNAL_FILE_NML )

# Special cases
# -------------
if( CMAKE_Fortran_COMPILER_ID MATCHES "GNU" OR CMAKE_Fortran_COMPILER_ID MATCHES "Clang")

  set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffree-line-length-none -fdec -fno-range-check ")

elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Intel" )

  include( compiler_flags_Intel_Fortran )

endif()

if( APPLE )
  add_definitions( -D__APPLE__ )
endif()

# Option to compile FMS in single or double precision
# --------------------------------------------------------
if (FV3LM_PRECISION MATCHES "DOUBLE" OR NOT FV3LM_PRECISION)

  # Add double precision compilation flags
  if( CMAKE_Fortran_COMPILER_ID MATCHES "Clang" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fdefault-real-8 -fdefault-double-8 ")

  elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Cray" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} --sreal64 ")

  elseif( CMAKE_Fortran_COMPILER_ID MATCHES "GNU" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fdefault-real-8 -fdefault-double-8 ")

  elseif( CMAKE_Fortran_COMPILER_ID MATCHES "Intel" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -r8")

  elseif( CMAKE_Fortran_COMPILER_ID MATCHES "PGI" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -r8")

  elseif( CMAKE_Fortran_COMPILER_ID MATCHES "XL" )

    set( CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -qdpc")

  else()

    message( FATAL "Fortran compiler with ID ${CMAKE_CXX_COMPILER_ID} does not have double precision flags set")

  endif()

else()

  add_definitions( -DOVERLOAD_R4 -DOVERLOAD_R8 )

endif()
