include(FetchContent)
FetchContent_Declare(gtest
  QUIET
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

#################### Add test files and libraries hear ####################
add_executable(
    unit_test
    aim/associative_index_matching_unit_test.cpp
    aim/comparator_array_test.cpp
    aim/priority_encoder_test.cpp
    utils.h
)

target_link_libraries(unit_test
PRIVATE
  AIM
  gtest_main
)

target_include_directories(unit_test
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)
###########################################################################

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(unit_test
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
)
