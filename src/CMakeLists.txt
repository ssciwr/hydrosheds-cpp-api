add_library(hydrosheds hydrosheds.cc)
target_include_directories(hydrosheds PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(hydrosheds PUBLIC GDAL::GDAL)

# Add an alias for use if this project is included as a subproject in another project
add_library(hydrosheds::hydrosheds ALIAS hydrosheds)

