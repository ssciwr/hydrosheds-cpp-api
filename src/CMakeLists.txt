add_library(hydrosheds-cpp-api hydrosheds-cpp-api.cpp)
target_include_directories(hydrosheds-cpp-api PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Add an alias for use if this project is included as a subproject in another project
add_library(hydrosheds-cpp-api::hydrosheds-cpp-api ALIAS hydrosheds-cpp-api)
