include(GenerateExportHeader)

add_library(stat_component StatComponent.cpp)

add_library(battleTest::stat_component ALIAS stat_component)

target_link_libraries(stat_component PRIVATE battleTest_options battleTest_warnings)

target_include_directories(stat_component ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(stat_component PUBLIC cxx_std_23)

set_target_properties(
  stat_component
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(stat_component EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/battleTest/stat_component_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(stat_component PUBLIC STAT_COMPONENT_STATIC_DEFINE)
endif()
