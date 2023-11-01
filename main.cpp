#define BOOST_TEST_MODULE ALL TESTS
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>

#include <iostream>

#include "chroniclock/seqlock.hpp"

BOOST_AUTO_TEST_CASE(test) {
    std::cout << "Hello wurld!\n";
}

int main(int argc, char **argv) {
    int error_code = boost::unit_test::unit_test_main(init_unit_test, argc, argv);
    if (error_code != 0) return error_code;
    return 0;
}