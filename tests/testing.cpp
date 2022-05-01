#include "ftest/test_logging.hpp"
#include "test_lw_list.hpp"
#include "test_lw_pair.hpp"
#include "test_lw_queue.hpp"
#include "test_lw_unordered_map.hpp"
#include "test_lw_unordered_set.hpp"
#include "test_lw_vector.hpp"

int main() {
    static constexpr size_t num_operations = 10000;

    TestLogging::start_suite("lw_std");

    TestLogging::run("vector<int>", TestLwVector::run_with_int, num_operations);
    TestLogging::run("vector<NonTrivial>", TestLwVector::run_with_non_trivial, num_operations);

    TestLogging::run("list<int>", TestLwList::run_with_int, num_operations);
    TestLogging::run("list<NonTrivial", TestLwList::run_with_non_trivial, num_operations);

    TestLogging::run("pair", TestLwPair::run);

    TestLogging::run("queue<int>", TestLwQueue::run_with_int, num_operations);
    TestLogging::run("queue<NonTrivial>", TestLwQueue::run_with_non_trivial, num_operations);

    TestLogging::run("unordered_set<int>", TestLwUnorderedSet::run_with_int, num_operations);

    TestLogging::run("unordered_map<int, int>", TestLwUnorderedMap::run_with_int_int, num_operations);
    TestLogging::run("unordered_map<int, NonTrivial>", TestLwUnorderedMap::run_with_int_non_trivial, num_operations);

    return TestLogging::results();
}