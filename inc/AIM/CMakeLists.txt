add_library(AIM)

set(AIM_SRC_DIR ${PROJECT_SRC_DIR}/aim)

target_sources(AIM
  PRIVATE
    ${AIM_SRC_DIR}/associative_index_matching_unit.cpp
    ${AIM_SRC_DIR}/comparator_array.cpp
    ${AIM_SRC_DIR}/priority_encoder_set.cpp
    ${AIM_SRC_DIR}/priority_encoder.cpp
    comparator_array.h
    priority_encoder_set.h
    priority_encoder.h
  PUBLIC
    associative_index_matching_unit.h
)

target_include_directories(AIM
  PUBLIC
    ${PROJECT_INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

