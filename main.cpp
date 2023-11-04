#define BOOST_TEST_MODULE ALL TESTS
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <thread>

#include "chroniclock/seqlock.hpp"

BOOST_AUTO_TEST_CASE(basic_test) {
    int data = 30;
    chroniclock::seqlock<int> s;
    s.store(data);
    auto func = [&s]() {
        int tmp = 0;
        while (true) {
            if (s.load(tmp) && tmp == 100) {
                return;
            }
        }
    };
    std::thread thread1(func);
    s.store(100);
    thread1.join(); // test passes if thread1 doesn't run forever
}

int main(int argc, char **argv) {
    int error_code = boost::unit_test::unit_test_main(init_unit_test, argc, argv);
    if (error_code != 0) return error_code;
    return 0;
}