# (C) Copyright 2017 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

################################################################################
# FMS
################################################################################

cmake_minimum_required( VERSION 3.3.2 FATAL_ERROR )

project( fms C Fortran )

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH} )

set( ECBUILD_DEFAULT_BUILD_TYPE Release )
set( ENABLE_OS_TESTS           OFF CACHE BOOL "Disable OS tests" FORCE )
set( ENABLE_LARGE_FILE_SUPPORT OFF CACHE BOOL "Disable testing of large file support" FORCE )
set( ENABLE_MPI ON CACHE BOOL "Compile with MPI" )

include( ecbuild_system NO_POLICY_SCOPE )

ecbuild_requires_macro_version( 2.7 )

################################################################################
# Project
################################################################################

ecbuild_declare_project()

ecbuild_enable_fortran( REQUIRED )

set( FMS_LINKER_LANGUAGE Fortran )

################################################################################
# Dependencies
################################################################################

# MPI
ecbuild_add_option( FEATURE MPI DEFAULT ON
                    DESCRIPTION "Support for MPI distributed parallelism"
                    REQUIRED_PACKAGES "MPI COMPONENTS Fortran" )
ecbuild_find_mpi( COMPONENTS Fortran REQUIRED )
ecbuild_include_mpi()
link_libraries(${MPI_Fortran_LIBRARIES})

# OpenMP
if( HAVE_OMP )
  ecbuild_enable_omp()
else()
  ecbuild_enable_ompstubs()
endif()

# NetCDF
find_package( NetCDF REQUIRED COMPONENTS Fortran )
include_directories( ${NETCDF_INCLUDE_DIR} )

################################################################################
# Definitions
################################################################################

################################################################################
# Export package info
################################################################################

set( FMS_INCLUDE_DIRS ${CMAKE_Fortran_MODULE_DIRECTORY}
                      ${CMAKE_CURRENT_SOURCE_DIR}/include
                      ${CMAKE_CURRENT_SOURCE_DIR}/mpp/include
                      ${CMAKE_CURRENT_SOURCE_DIR}/fms)
set( FMS_LIBRARIES fms )

get_directory_property( FMS_DEFINITIONS COMPILE_DEFINITIONS )

list( APPEND FMS_TPLS NetCDF )

foreach( _tpl ${FMS_TPLS} )
  string( TOUPPER ${_tpl} TPL )
  list( APPEND FMS_EXTRA_DEFINITIONS   ${${TPL}_DEFINITIONS}  ${${TPL}_TPL_DEFINITIONS}  )
  list( APPEND FMS_EXTRA_INCLUDE_DIRS  ${${TPL}_INCLUDE_DIRS} ${${TPL}_TPL_INCLUDE_DIRS} )
  list( APPEND FMS_EXTRA_LIBRARIES     ${${TPL}_LIBRARIES}    ${${TPL}_TPL_LIBRARIES}    )
endforeach()

################################################################################
# Sources
################################################################################

# FMS compiler flags are set here (e.g. -Duse_libMPI, etc.)
include( fms_compiler_flags )

include_directories( ${FMS_INCLUDE_DIRS} ${FMS_EXTRA_INCLUDE_DIRS} )

################################################################################
list( APPEND fms_hdr_files
include/file_version.h
include/fms_platform.h
)

list( APPEND fms_src_files
amip_interp/amip_interp.F90
astronomy/astronomy.F90
axis_utils/axis_utils.F90
block_control/block_control.F90
column_diagnostics/column_diagnostics.F90
constants/constants.F90
coupler/atmos_ocean_fluxes.F90
coupler/coupler_types.F90
coupler/ensemble_manager.F90
data_override/data_override.F90
diag_manager/diag_axis.F90
diag_manager/diag_data.F90
diag_manager/diag_grid.F90
diag_manager/diag_manager.F90
diag_manager/diag_manifest.F90
diag_manager/diag_output.F90
diag_manager/diag_table.F90
diag_manager/diag_util.F90
drifters/cloud_interpolator.F90
drifters/drifters.F90
drifters/drifters_comm.F90
drifters/drifters_compute_k.h
drifters/drifters_core.F90
drifters/drifters_input.F90
drifters/drifters_io.F90
drifters/drifters_push.h
drifters/drifters_set_field.h
drifters/fms_switches.h
drifters/quicksort.F90
exchange/stock_constants.F90
exchange/test_xgrid.F90
exchange/xgrid.F90
#fft/fft99.F90
#fft/fft.F90
field_manager/field_manager.F90
field_manager/fm_util.F90
field_manager/parse.inc
fms/fms.F90
fms/fms_io.F90
fms/fms_io_unstructured_field_exist.inc
fms/fms_io_unstructured_file_unit.inc
fms/fms_io_unstructured_get_field_size.inc
fms/fms_io_unstructured_get_file_name.inc
fms/fms_io_unstructured_get_file_unit.inc
fms/fms_io_unstructured_read.inc
fms/fms_io_unstructured_register_restart_axis.inc
fms/fms_io_unstructured_register_restart_field.inc
fms/fms_io_unstructured_save_restart.inc
fms/fms_io_unstructured_setup_one_field.inc
fms/read_data_2d.inc
fms/read_data_3d.inc
fms/read_data_4d.inc
fms/test_fms_io.F90
fms/test_unstructured_fms_io.F90
fms/write_data.inc
horiz_interp/horiz_interp.F90
horiz_interp/horiz_interp_bicubic.F90
horiz_interp/horiz_interp_bilinear.F90
horiz_interp/horiz_interp_conserve.F90
horiz_interp/horiz_interp_spherical.F90
horiz_interp/horiz_interp_type.F90
horiz_interp/test_horiz_interp.F90
interpolator/interpolator.F90
memutils/memuse.c
memutils/memutils.F90
mosaic/constant.h
mosaic/create_xgrid.c
mosaic/create_xgrid.h
mosaic/gradient.F90
mosaic/gradient_c2l.c
mosaic/gradient_c2l.h
mosaic/grid.F90
mosaic/interp.c
mosaic/interp.h
mosaic/mosaic.F90
mosaic/mosaic_util.c
mosaic/mosaic_util.h
mosaic/read_mosaic.c
mosaic/read_mosaic.h
mpp/affinity.c
mpp/mpp.F90
mpp/mpp_data.F90
mpp/mpp_domains.F90
mpp/mpp_efp.F90
mpp/mpp_io.F90
mpp/mpp_memutils.F90
mpp/mpp_parameter.F90
mpp/mpp_pset.F90
mpp/mpp_utilities.F90
mpp/nsclock.c
mpp/test_mpp.F90
mpp/test_mpp_domains.F90
mpp/test_mpp_io.F90
mpp/test_mpp_pset.F90
mpp/threadloc.c
mpp/include/group_update_pack.inc
mpp/include/group_update_unpack.inc
mpp/include/mpp_alltoall_mpi.h
mpp/include/mpp_alltoall_nocomm.h
mpp/include/mpp_alltoall_sma.h
mpp/include/mpp_chksum.h
mpp/include/mpp_chksum_int.h
mpp/include/mpp_chksum_scalar.h
mpp/include/mpp_comm.inc
mpp/include/mpp_comm_mpi.inc
mpp/include/mpp_comm_nocomm.inc
mpp/include/mpp_comm_sma.inc
mpp/include/mpp_data_mpi.inc
mpp/include/mpp_data_nocomm.inc
mpp/include/mpp_data_sma.inc
mpp/include/mpp_define_nest_domains.inc
mpp/include/mpp_do_check.h
mpp/include/mpp_do_checkV.h
mpp/include/mpp_do_get_boundary.h
mpp/include/mpp_do_global_field.h
mpp/include/mpp_do_redistribute.h
mpp/include/mpp_do_update.h
mpp/include/mpp_do_updateV.h
mpp/include/mpp_do_updateV_ad.h
mpp/include/mpp_do_updateV_nonblock.h
mpp/include/mpp_do_update_ad.h
mpp/include/mpp_do_update_nest.h
mpp/include/mpp_do_update_nonblock.h
mpp/include/mpp_domains_comm.inc
mpp/include/mpp_domains_define.inc
mpp/include/mpp_domains_misc.inc
mpp/include/mpp_domains_reduce.inc
mpp/include/mpp_domains_util.inc
mpp/include/mpp_error_a_a.h
mpp/include/mpp_error_a_s.h
mpp/include/mpp_error_s_a.h
mpp/include/mpp_error_s_s.h
mpp/include/mpp_gather.h
mpp/include/mpp_get_boundary.h
mpp/include/mpp_global_field.h
mpp/include/mpp_global_field_ug.h
mpp/include/mpp_global_reduce.h
mpp/include/mpp_global_sum.h
mpp/include/mpp_global_sum_ad.h
mpp/include/mpp_global_sum_tl.h
mpp/include/mpp_group_update.h
mpp/include/mpp_io_connect.inc
mpp/include/mpp_io_misc.inc
mpp/include/mpp_io_read.inc
mpp/include/mpp_io_unstructured_read.inc
mpp/include/mpp_io_unstructured_write.inc
mpp/include/mpp_io_util.inc
mpp/include/mpp_io_write.inc
mpp/include/mpp_read_2Ddecomp.h
mpp/include/mpp_read_compressed.h
mpp/include/mpp_read_distributed_ascii.h
mpp/include/mpp_read_distributed_ascii.inc
mpp/include/mpp_reduce_mpi.h
mpp/include/mpp_reduce_nocomm.h
mpp/include/mpp_reduce_sma.h
mpp/include/mpp_scatter.h
mpp/include/mpp_sum.inc
mpp/include/mpp_sum_mpi.h
mpp/include/mpp_sum_nocomm.h
mpp/include/mpp_sum_sma.h
mpp/include/mpp_transmit.inc
mpp/include/mpp_transmit_mpi.h
mpp/include/mpp_transmit_nocomm.h
mpp/include/mpp_transmit_sma.h
mpp/include/mpp_unstruct_domain.inc
mpp/include/mpp_unstruct_pass_data.h
mpp/include/mpp_update_domains2D.h
mpp/include/mpp_update_domains2D_ad.h
mpp/include/mpp_update_domains2D_nonblock.h
mpp/include/mpp_update_nest_domains.h
mpp/include/mpp_util.inc
mpp/include/mpp_util_mpi.inc
mpp/include/mpp_util_nocomm.inc
mpp/include/mpp_util_sma.inc
mpp/include/mpp_write.h
mpp/include/mpp_write_2Ddecomp.h
mpp/include/mpp_write_compressed.h
mpp/include/mpp_write_unlimited_axis.h
mpp/include/system_clock.h
oda_tools/oda_core.F90
oda_tools/oda_core_ecda.F90
oda_tools/oda_types.F90
oda_tools/write_ocean_data.F90
oda_tools/xbt_drop_rate_adjust.f90
platform/platform.F90
random_numbers/MersenneTwister.F90
random_numbers/random_numbers.F90
sat_vapor_pres/sat_vapor_pres.F90
sat_vapor_pres/sat_vapor_pres_k.F90
station_data/station_data.F90
time_interp/time_interp.F90
time_interp/time_interp_external.F90
time_manager/get_cal_time.F90
time_manager/time_manager.F90
topography/gaussian_topog.F90
topography/topography.F90
tracer_manager/tracer_manager.F90
tridiagonal/tridiagonal.F90
)
################################################################################

################################################################################
ecbuild_add_library( TARGET   fms
                     SOURCES  ${fms_hdr_files} ${fms_src_files}
                     LIBS     ${NETCDF_LIBRARIES}
                     INSTALL_HEADERS_LIST ${fms_hdr_files}
                     LINKER_LANGUAGE ${FMS_LINKER_LANGUAGE}
                    )
################################################################################

# glibc>=2.30 includes gettid in unistd.h if _GNU_SOURCE is set
include(CheckSymbolExists)
set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE=1")
check_symbol_exists("gettid" "unistd.h" HAVE_GETTID)
unset(CMAKE_REQUIRED_DEFINITIONS)
if(HAVE_GETTID)
    target_compile_definitions(fms PRIVATE HAVE_GETTID)
endif()


if(ECBUILD_INSTALL_FORTRAN_MODULES)
  install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/${CMAKE_CFG_INTDIR} DESTINATION ${INSTALL_INCLUDE_DIR} )
endif()

################################################################################
# Finalise configuration
################################################################################

# prepares a tar.gz of the sources and/or binaries
ecbuild_install_project( NAME fms )

# print the summary of the configuration
ecbuild_print_summary()
