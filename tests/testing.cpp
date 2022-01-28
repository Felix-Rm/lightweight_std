#include "ftest/test_logging.hpp"
#include "test_lw_list.hpp"
#include "test_lw_pair.hpp"
#include "test_lw_queue.hpp"
#include "test_lw_unordered_map.hpp"
#include "test_lw_unordered_set.hpp"
#include "test_lw_vector.hpp"

int main() {
    TestLogging::start_suite("lw_std");

    TestLogging::run<TestLwVector>();
    TestLogging::run<TestLwList>();
    TestLogging::run<TestLwPair>();
    TestLogging::run<TestLwQueue>();
    TestLogging::run<TestLwUnorderedSet>();
    TestLogging::run<TestLwUnorderedMap>();

    TestLogging::results();

    return 0;
}