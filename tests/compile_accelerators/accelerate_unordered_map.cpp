#include <unordered_map>

#include "../test_lw_unordered_map.hpp"
#include "unordered_map.hpp"

LWSTD_TEST_ACCELERATE(TestLwUnorderedMap, unordered_map, int, int);
LWSTD_TEST_ACCELERATE(TestLwUnorderedMap, unordered_map, int, NonTrivial);