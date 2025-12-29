include(GenerateExportHeader)

add_library(combat_status CombatStatus.cpp)

add_library(battleTest::combat_status ALIAS combat_status)

target_link_libraries(combat_status
  PRIVATE battleTest_options battleTest_warnings
  PUBLIC battleTest::stat_component)

target_include_directories(combat_status ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(combat_status PUBLIC cxx_std_23)

set_target_properties(
  combat_status
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(combat_status EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/battleTest/combat_status_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(combat_status PUBLIC COMBAT_STATUS_STATIC_DEFINE)
endif()
