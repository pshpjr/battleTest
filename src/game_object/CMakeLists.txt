include(GenerateExportHeader)

add_library(game_object GameObject.cpp GameObjectFactory.cpp)

add_library(battleTest::game_object ALIAS game_object)

target_link_libraries(game_object PRIVATE battleTest_options battleTest_warnings)

target_include_directories(game_object ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(game_object PUBLIC cxx_std_23)

set_target_properties(
  game_object
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(game_object EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/battleTest/game_object_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(game_object PUBLIC GAME_OBJECT_STATIC_DEFINE)
endif()
