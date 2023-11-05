#define BOOST_TEST_MODULE ALL TESTS
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>
#include <cassert>

#include <iostream>
#include <thread>
#include <unistd.h>

#include "chroniclock/seqlock.hpp"
#include "chroniclock/Timer.h"
#include "rigtorp/seqlock.hpp"

#define STATS 1

BOOST_AUTO_TEST_CASE(does_it_work) {
    int data = 50;
    chroniclock::seqlock<int> s;
    s.store(data);
    int tmp = 10;
    BOOST_TEST(s.load(tmp));
    BOOST_TEST(tmp == 50);
    data = 60;
    s.store(data);
    BOOST_TEST(s.load(tmp));
    BOOST_TEST(tmp == 60);
}

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

    if (STATS) {
        Timer t("../stats/seqlock.txt");
        //Timer t("../stats/seqlock_nopadding.txt");

        struct Info {
            int x, y, z;
        };
        
        const int NTHREADS = 50;
        const int NTESTS = 5;

        for (int nums = 0; nums < NTESTS; nums++) {
            // Testing starts: initializing variables
            chroniclock::seqlock<Info> s;
            std::atomic<uint32_t> ct(0);
            std::vector<std::thread> threads;

            auto func = [&s, &ct]() {
                while (ct.load(std::memory_order_acquire) == 0);
                struct Info tmp;
                for (int i = 0; i < (int)1e7; i++) {
                    while (!s.load(tmp));
                    assert(tmp.x == tmp.z/2 && tmp.y == tmp.x + tmp.z);
                }
                ct.fetch_sub(1, std::memory_order_release);
            };

            // Add function to NTHREADS threads, which will try to load
            // 1e7 instances of struct Info
            for (int i = 0; i < NTHREADS; i++) {
                threads.push_back(std::thread(func));
            }

            int start = 10;
            bool begin = false;
            t.start();
            // Will keep pushing Info until 1e7 has been read
            while (true) {
                struct Info tmp = { start/2, start/2 + start, start };
                start++;
                s.store(tmp);
                if (!begin) {
                    begin = true;
                    ct.fetch_add(threads.size(), std::memory_order_release);
                }
                if (ct.load(std::memory_order_acquire) == 0) {
                    break;
                }
            }
            for (auto &thr : threads) thr.join();
            t.stop();
            std::cout << "start: " << start << '\n';
        }
        t.printStats(); // Test complete, printing stats


        // RIGTORP TEST
        t.reset("../stats/seqlock_rigtorp.txt");
        for (int nums = 0; nums < NTESTS; nums++) {
            // Testing starts: initializing variables
            rigtorp::Seqlock<Info> s;
            std::atomic<uint32_t> ct(0);
            std::vector<std::thread> threads;

            auto func = [&s, &ct]() {
                while (ct.load(std::memory_order_acquire) == 0);
                struct Info tmp;
                for (int i = 0; i < (int)1e7; i++) {
                    tmp = s.load();
                    assert(tmp.x == tmp.z/2 && tmp.y == tmp.x + tmp.z);
                }
                ct.fetch_sub(1, std::memory_order_release);
            };

            // Add function to NTHREADS threads, which will try to load
            // 1e7 instances of struct Info
            for (int i = 0; i < NTHREADS; i++) {
                threads.push_back(std::thread(func));
            }

            int start = 10;
            bool begin = false;
            t.start();
            // Will keep pushing Info until 1e7 has been read
            while (true) {
                struct Info tmp = { start/2, start/2 + start, start };
                start++;
                s.store(tmp);
                if (!begin) {
                    begin = true;
                    ct.fetch_add(threads.size(), std::memory_order_release);
                }
                if (ct.load(std::memory_order_acquire) == 0) {
                    break;
                }
            }
            for (auto &thr : threads) thr.join();
            t.stop();
            std::cout << "start: " << start << '\n';
        }
        t.printStats(); // Test complete, printing stats
    }
    return 0;
}