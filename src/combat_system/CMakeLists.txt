include(GenerateExportHeader)

add_library(combat_system CombatSystem.cpp)

add_library(battleTest::combat_system ALIAS combat_system)

target_link_libraries(combat_system
  PRIVATE battleTest_options battleTest_warnings
  PUBLIC battleTest::combat_status battleTest::stat_component)

target_include_directories(combat_system ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(combat_system PUBLIC cxx_std_23)

set_target_properties(
  combat_system
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(combat_system EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/battleTest/combat_system_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(combat_system PUBLIC COMBAT_SYSTEM_STATIC_DEFINE)
endif()
