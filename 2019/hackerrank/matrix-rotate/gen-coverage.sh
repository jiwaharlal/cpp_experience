cd build
lcov --coverage --directory . --output-file main_coverage.info
genhtml main_coverage.info --output-directory out
