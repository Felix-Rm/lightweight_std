#pragma once

#include <time.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>

#include "ftest/test_logging.hpp"

class NonTrivial {
   public:
    NonTrivial() = delete;

    NonTrivial(unsigned a)
        : m_data(a){};

    bool operator==(const NonTrivial& rhs) const {
        return m_data == rhs.m_data;
    };

    bool operator!=(const NonTrivial& rhs) const {
        return !operator==(rhs);
    };

    auto data() const {
        return m_data;
    };

   private:
    unsigned m_data{0};
};

template <typename TestContainer, typename VerifyContainer, typename ContainerValueType = typename VerifyContainer::value_type>
class ContainerTester {
   public:
    typedef TestContainer test_container_t;
    typedef VerifyContainer verify_container_t;

    typedef std::function<size_t(const TestContainer&)> size_getter_t;
    typedef std::function<ContainerValueType()> value_generator_t;

    typedef std::function<std::string(TestContainer&, VerifyContainer&, const value_generator_t&)> grow_modifier_t;
    typedef std::function<std::string(TestContainer&, VerifyContainer&)> shrink_modifier_t;
    typedef std::function<TestLogging::test_result_t(const TestContainer&, const VerifyContainer&)> verifier_t;

    typedef std::function<void(const TestContainer&)> tc_printer_t;
    typedef std::function<void(const VerifyContainer&)> tv_printer_t;

    void set_median_size(size_t size) {
        m_median_size = size;
    };

    void add_grow_modifier(std::string name, const grow_modifier_t& mod) {
        m_grow_modifiers.emplace(move(name), mod);
    };

    void add_shrink_modifier(std::string name, const shrink_modifier_t& mod) {
        m_shrink_modifiers.emplace(move(name), mod);
    };

    void add_verifier(std::string name, const verifier_t& verifier) {
        m_verifiers.emplace(move(name), verifier);
    };

    void set_value_generator(const value_generator_t& generator) {
        m_generator = generator;
    };

    void set_size_getter(const size_getter_t& getter) {
        m_size_getter = getter;
    };

    void set_test_container_printer(const tc_printer_t& printer) {
        m_tc_printer = printer;
    };

    void set_verify_container_printer(const tv_printer_t& printer) {
        m_tv_printer = printer;
    };

    TestLogging::test_result_t run_operations(size_t num) {
        if (!m_size_getter || !m_generator || !m_tc_printer || !m_tv_printer || m_grow_modifiers.empty() || m_shrink_modifiers.empty() || m_verifiers.empty()) {
            return {"one of the following is empty or unset: grow or shrink modifiers, validators, generator, size_getter, tc or tv printer "};
        }

        auto seed = time(NULL);
        std::srand(0);

        TestLogging::test_printf("Random seed for container test is %ld", seed);

        for (size_t i = 0; i < num; ++i) {
            operation_t op = operations::GROW;

            auto curent_size = m_size_getter(m_test_container);

            if (curent_size > 0) {
                ssize_t tolerance = (rand() % (m_median_size / 2)) - m_median_size / 4;
                ssize_t current_size_with_tolerance = (ssize_t) curent_size + tolerance;

                op = current_size_with_tolerance > m_median_size ? operations::SHRINK : operations::GROW;
            }

            // TestLogging::test_printf("running %s operation, size = %ld", op == operations::GROW ? "grow" : "shrink", curent_size);

            std::string last_mod_name = "";
            std::string last_mod_operation = "";

            if (op == operations::GROW) {
                size_t idx = rand() % m_grow_modifiers.size();
                auto it = m_grow_modifiers.begin();
                std::advance(it, idx);
                last_mod_name = it->first;
                last_mod_operation = it->second(m_test_container, m_verify_container, m_generator);
            } else {
                size_t idx = rand() % m_shrink_modifiers.size();
                auto it = m_shrink_modifiers.begin();
                std::advance(it, idx);
                last_mod_name = it->first;
                last_mod_operation = it->second(m_test_container, m_verify_container);
            }

            for (auto& verifier : m_verifiers) {
                TestLogging::test_result_t s = verifier.second(m_test_container, m_verify_container);
                if (!s.passed) {
                    TestLogging::test_printf("Test Container:");
                    m_tc_printer(m_test_container);
                    TestLogging::test_printf("Verify Container:");
                    m_tv_printer(m_verify_container);
                    return {"Test '" + verifier.first + "' failed after applying '" + last_mod_name + "' [" + last_mod_operation + "] with reason: " + s.fail_reason};
                }
            }
        }

        return {};
    }

    static size_t default_size_getter(const test_container_t& c) {
        return default_size_getter_templated(c);
    };

    static unsigned int default_uint_generator() {
        return rand() % 100;
    };

    static double default_double_generator() {
        return (rand() % 10000) / 100.0;
    };

    static NonTrivial default_non_trivial_generator() {
        return {default_uint_generator()};
    };

    static void default_test_container_printer(const test_container_t& tc) {
        default_printer_templated(tc);
    }

    static void default_verify_container_printer(const verify_container_t& vc) {
        default_printer_templated(vc);
    }

    static std::string default_grow_by_push(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_push_templated(tc, vc, gen);
    }

    static std::string default_grow_by_push_back(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_push_back_templated(tc, vc, gen);
    }

    static std::string default_grow_by_push_front(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_push_front_templated(tc, vc, gen);
    }

    static std::string default_grow_by_emplace_back(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_emplace_back_templated(tc, vc, gen);
    }

    static std::string default_grow_by_emplace_front(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_emplace_front_templated(tc, vc, gen);
    }

    static std::string default_grow_by_emplace_no_iterator(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_emplace_no_iterator_templated(tc, vc, gen);
    }

    static std::string default_grow_by_emplace(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_emplace_templated(tc, vc, gen, default_size_getter);
    }

    static std::string default_grow_by_insert_no_iterator(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_insert_no_iterator_templated(tc, vc, gen);
    }

    static std::string default_grow_by_insert(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return default_grow_by_insert_templated(tc, vc, gen, default_size_getter);
    }

    static std::string default_shrink_by_pop(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_pop_templated(tc, vc);
    }

    static std::string default_shrink_by_pop_back(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_pop_back_templated(tc, vc);
    }

    static std::string default_shrink_by_pop_front(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_pop_front_templated(tc, vc);
    }

    static std::string default_shrink_by_erase_by_iterator(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_erase_by_iterator_templated(tc, vc, default_size_getter);
    }

    static std::string default_shrink_by_erase_by_value(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_erase_by_value_templated(tc, vc, default_size_getter);
    }

    static std::string default_shrink_by_map_erase_by_value(test_container_t& tc, verify_container_t& vc) {
        return default_shrink_by_map_erase_by_value_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result_t default_verify_size(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_size_templated(tc, vc, default_size_getter, default_size_getter_templated<verify_container_t>);
    }

    static TestLogging::test_result_t default_verify_element_position_with_at(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_element_position_with_at_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result_t default_verify_element_position(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_element_position_templated(tc, vc);
    }

    static TestLogging::test_result_t default_verify_front_element_position(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_front_element_position_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result_t default_verify_back_element_position(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_back_element_position_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result_t default_verify_find_existing_element(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_find_existing_element_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result_t default_verify_element_inclusion(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_element_inclusion_templated(tc, vc);
    }

    static TestLogging::test_result_t default_verify_map_element_inclusion(const test_container_t& tc, const verify_container_t& vc) {
        return default_verify_map_element_inclusion_templated(tc, vc);
    }

    template <typename Container>
    static size_t default_size_getter_templated(const Container& c) {
        return c.size();
    };

    template <typename Container>
    static void default_printer_templated(const Container& c) {
        TestLogging::test_printf_start("[");
        for (const auto& item : c) {
            TestLogging::test_printf_append(" ");
            print_item(item);
            TestLogging::test_printf_append(",");
        }
        TestLogging::test_printf_end("]");
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_push_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push(value);
        vc.push(value);

        return "push value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_push_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push_back(value);
        vc.push_back(value);

        return "push_back value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_push_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push_front(value);
        vc.push_front(value);

        return "push_front value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_emplace_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.emplace_back(value);
        vc.emplace_back(value);

        return "emplace_back value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_emplace_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.emplace_front(value);
        vc.emplace_front(value);

        return "emplace_front value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_emplace_no_iterator_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.emplace(value);
        vc.emplace(value);

        return "emplace value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static std::string default_grow_by_emplace_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        int idx = size_getter(tc) ? rand() % size_getter(tc) : 0;

        tc.emplace(advance_copy(tc.begin(), idx), value);
        vc.emplace(advance_copy(vc.begin(), idx), value);

        return "emplace value " + to_string(value) + " at index " + to_string(idx);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static std::string default_grow_by_insert_no_iterator_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();

        tc.insert(value);
        vc.insert(value);

        return "insert value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static std::string default_grow_by_insert_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        int idx = size_getter(tc) ? rand() % size_getter(tc) : 0;

        tc.insert(advance_copy(tc.begin(), idx), value);
        vc.insert(advance_copy(vc.begin(), idx), value);

        return "insert value " + to_string(value) + " at index " + to_string(idx);
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static std::string default_shrink_by_pop_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop();
        vc.pop();

        return "pop";
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static std::string default_shrink_by_pop_back_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop_back();
        vc.pop_back();

        return "pop_back";
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static std::string default_shrink_by_pop_front_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop_front();
        vc.pop_front();

        return "pop_front";
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static std::string default_shrink_by_erase_by_iterator_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        int idx = size_getter(tc) ? rand() % size_getter(tc) : 0;

        tc.erase(advance_copy(tc.begin(), idx));
        vc.erase(advance_copy(vc.begin(), idx));

        return "erase at idx " + to_string(idx);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static std::string default_shrink_by_erase_by_value_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        int idx = size_getter(tc) ? rand() % size_getter(tc) : 0;
        auto value = *advance_copy(tc.begin(), idx);

        tc.erase(value);
        vc.erase(value);

        return "erase value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static std::string default_shrink_by_map_erase_by_value_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        int idx = size_getter(tc) ? rand() % size_getter(tc) : 0;
        auto value = *advance_copy(tc.begin(), idx);

        tc.erase(value.first);
        vc.erase(value.first);

        return "erase value " + to_string(value);
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter, typename VerifyContainerSizeGetter>
    static TestLogging::test_result_t default_verify_size_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter, const VerifyContainerSizeGetter& vc_size_getter) {
        if (tc_size_getter(tc) != vc_size_getter(vc))
            return {"sizes dont match: TestContainer: " + to_string(tc_size_getter(tc)) + " VerifyContainer: " + to_string(vc_size_getter(vc))};

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result_t default_verify_element_position_with_at_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        for (size_t idx = 0; idx < tc_size_getter(tc); ++idx)
            if (tc.at(idx) != vc.at(idx))
                return {"values dont match at idx " + to_string(idx) + " TestContainer: " + to_string(tc.at(idx)) + " VerifyContainer: " + to_string(vc.at(idx))};

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result_t default_verify_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        auto tc_it = tc.begin();
        auto vc_it = vc.begin();
        size_t pos = 0;

        while (tc_it != tc.end()) {
            if (*tc_it != *vc_it)
                return {"values dont match at position " + to_string(pos) + " TestContainer: " + to_string(*tc_it) + " VerifyContainer: " + to_string(*vc_it)};
            tc_it++;
            vc_it++;
            pos++;
        }

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result_t default_verify_front_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        if (tc.front() != vc.front())
            return {"front values dont match TestContainer: " + to_string(tc.front()) + " VerifyContainer: " + to_string(vc.front())};

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result_t default_verify_back_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        if (tc.back() != vc.back())
            return {"back values dont match TestContainer: " + to_string(tc.back()) + " VerifyContainer: " + to_string(vc.back())};

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result_t default_verify_find_existing_element_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        if (tc_size_getter(tc) == 0) return {};

        int idx = rand() % tc_size_getter(tc);

        auto find_value = *(tc.begin() + idx);
        auto res = find(tc.begin(), tc.end(), find_value);

        if (res == tc.end())
            return {"find failed on existing value " + to_string(find_value)};

        if (*res != find_value) {
            return {"find returned wrong iterator to value " + to_string(find_value)};
        }

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result_t default_verify_element_inclusion_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        for (auto& elt : tc) {
            if (vc.find(elt) == vc.end())
                return {"element " + to_string(elt) + " exists in TestContainer, but not in VerifyContainer"};
        }

        for (auto& elt : vc) {
            if (tc.find(elt) == tc.end())
                return {"element " + to_string(elt) + " exists in VerifyContainer, but not in TestContainer"};
        }

        return {};
    };

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result_t default_verify_map_element_inclusion_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        for (const auto& elt : tc) {
            auto res = vc.find(elt.first);
            if (res == vc.end())
                return {"element " + to_string(elt) + " exists in TestContainer, but not in VerifyContainer"};
            if (*res != elt)
                return {"element " + to_string(elt) + " has deviating value in VerifyContainer " + to_string(*res)};
        }

        for (auto& elt : vc) {
            auto res = vc.find(elt.first);
            if (res == vc.end())
                return {"element " + to_string(elt) + " exists in VerifyContainer, but not in TestContainer"};
            if (*res != elt)
                return {"element " + to_string(elt) + " has deviating value in TestContainer " + to_string(*res)};
        }

        return {};
    };

   private:
    template <typename Iterator>
    static Iterator advance_copy(const Iterator& cit, size_t n) {
        Iterator it = cit;
        for (size_t i = 0; i < n; i++)
            it++;
        return it;
    }

    template <typename T>
    static void print_item(const T& t) {
        TestLogging::test_printf_append("%3d", t);
    }

    static void print_item(const std::string& t) {
        TestLogging::test_printf_append("'%s'", t.c_str());
    }

    static void print_item(const NonTrivial& t) {
        TestLogging::test_printf_append("{%3d}", t.data());
    }

    template <template <typename, typename> class S, typename A, typename B>
    static void print_item(const S<A, B>& t) {
        TestLogging::test_printf_append("(");
        print_item(t.first);
        TestLogging::test_printf_append(", ");
        print_item(t.second);
        TestLogging::test_printf_append(")");
    }

    template <typename T>
    static std::string to_string(const T& t) {
        return std::to_string(t);
    }

    static std::string to_string(const NonTrivial& t) {
        return "{" + to_string(t.data()) + "}";
    }

    static std::string to_string(const std::string& t) {
        return t;
    }

    template <template <typename, typename> class S, typename A, typename B>
    static std::string to_string(const S<A, B>& t) {
        return "(" + to_string(t.first) + ", " + to_string(t.second) + ")";
    }

    typedef enum operations {
        SHRINK,
        GROW
    } operation_t;

    ssize_t m_median_size = 25;

    TestContainer m_test_container;
    VerifyContainer m_verify_container;

    std::unordered_map<std::string, grow_modifier_t> m_grow_modifiers;
    std::unordered_map<std::string, shrink_modifier_t> m_shrink_modifiers;
    std::unordered_map<std::string, verifier_t> m_verifiers;

    value_generator_t m_generator;
    size_getter_t m_size_getter;

    tc_printer_t m_tc_printer;
    tv_printer_t m_tv_printer;
};

template <typename Derived, template <typename...> class TestContainer, template <typename...> class VerifyContainer>
class ContainerTestDefaultMixin {
   public:
    static TestLogging::test_result_t run_with_int(size_t operation_count) {
        typedef ContainerTester<TestContainer<int>, VerifyContainer<int>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_uint_generator);

        return Derived::run_templated(tester, operation_count);
    };

    static TestLogging::test_result_t run_with_non_trivial(size_t operation_count) {
        typedef ContainerTester<TestContainer<NonTrivial>, VerifyContainer<NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_non_trivial_generator);

        return Derived::run_templated(tester, operation_count);
    };

    static TestLogging::test_result_t run_with_int_int(size_t operation_count) {
        typedef ContainerTester<TestContainer<int, int>, VerifyContainer<int, int>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, int> {
            return {tester_t::default_uint_generator(), tester_t::default_uint_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    };

    static TestLogging::test_result_t run_with_int_non_trivial(size_t operation_count) {
        typedef ContainerTester<TestContainer<int, NonTrivial>, VerifyContainer<int, NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, NonTrivial> {
            return {tester_t::default_uint_generator(), tester_t::default_non_trivial_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    };
};