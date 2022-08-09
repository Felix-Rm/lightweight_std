#include "container_tester_helpers.hpp"

namespace container_tester {

unsigned int urand() {
    return static_cast<unsigned>(rand());
}

unsigned int default_uint_generator() {
    return urand() % 100;
}

double default_double_generator() {
    return (urand() % 10000) / 100.0;
}

NonTrivial default_non_trivial_generator() {
    return {default_uint_generator()};
}

void print_item(const std::string& t) {
    TestLogging::test_printf_append("'%s'", t.c_str());
}

void print_item(const NonTrivial& t) {
    TestLogging::test_printf_append("{%3d}", t.data());
}

std::string to_string(const NonTrivial& t) {
    return "{" + to_string(t.data()) + "}";
}

std::string to_string(const std::string& t) {
    return t;
}

}  // namespace container_tester