add_executable(tests tests.cc hydrosheds_t.cc)
target_link_libraries(tests PUBLIC hydrosheds Catch2::Catch2)

# allow user to run tests with `make test` or `ctest`
catch_discover_tests(tests)
