#pragma once

#include <ftest/test_logging.hpp>

#include "string.hpp"
#include "utility.hpp"

class TestLwPair {
   public:
    static TestLogging::test_result run() {
        lw_std::pair<int, lw_std::string> pair{0, "test"};

        if (pair.first != 0 || pair.second != "test") return {"initializer"};

        auto copy = pair;

        if (copy.first != 0 || copy.second != "test") return {"copy"};

        return {};
    }
};