#pragma once

#include <time.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>

#include "ftest/test_logging.hpp"
#include "impl/pair.hpp"

/*
    This is a non default-constructable object with heap allocations to check if destructor is called aproproately
*/

class NonTrivial {
   public:
    NonTrivial() {
        m_data = new unsigned{-1u};
    }

    NonTrivial(unsigned a) {
        m_data = new unsigned{a};
    }

    // NonTrivial(const NonTrivial&) = delete;
    NonTrivial(const NonTrivial& other) {
        operator=(other);
    }

    NonTrivial(NonTrivial&& other) {
        operator=(std::move(other));
    }

    ~NonTrivial() {
        clear();
    }

    NonTrivial& operator=(const NonTrivial& other) {
        if (this != &other) {
            clear();
            if (other.m_data)
                m_data = new unsigned{*other.m_data};
        }
        return *this;
    }

    NonTrivial& operator=(NonTrivial&& other) {
        if (this != &other) {
            clear();
            m_data = other.m_data;
            other.m_data = nullptr;
        }
        return *this;
    }

    bool operator==(const NonTrivial& rhs) const {
        if (m_data == nullptr || rhs.m_data == nullptr) return false;
        return *m_data == *rhs.m_data;
    }

    bool operator!=(const NonTrivial& rhs) const {
        return !operator==(rhs);
    }

    unsigned data() const {
        return *m_data;
    }

    void clear() {
        if (m_data) {
            delete m_data;
            m_data = nullptr;
        }
    }

   private:
    unsigned* m_data{nullptr};
};

template <typename TestContainer, typename VerifyContainer, typename ContainerValueType = typename VerifyContainer::value_type>
class ContainerTester {
   public:
    typedef TestContainer test_container_t;
    typedef VerifyContainer verify_container_t;

    typedef std::function<size_t(const test_container_t&)> size_getter_t;
    typedef std::function<ContainerValueType()> value_generator_t;

    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&, const value_generator_t&)> neutral_modifier_t;
    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&, const value_generator_t&)> grow_modifier_t;
    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&)> shrink_modifier_t;
    typedef std::function<TestLogging::test_result(const test_container_t&, const verify_container_t&)> verifier_t;

    typedef std::function<void(const test_container_t&)> tc_printer_t;
    typedef std::function<void(const verify_container_t&)> tv_printer_t;

    void set_median_size(size_t size) {
        m_median_size = size;
    }

    void add_neutral_modifier(std::string name, const neutral_modifier_t& mod) {
        m_neutral_modifiers.emplace(move(name), mod);
    }

    void add_grow_modifier(std::string name, const grow_modifier_t& mod) {
        m_grow_modifiers.emplace(move(name), mod);
    }

    void add_shrink_modifier(std::string name, const shrink_modifier_t& mod) {
        m_shrink_modifiers.emplace(move(name), mod);
    }

    void add_verifier(std::string name, const verifier_t& verifier) {
        m_verifiers.emplace(move(name), verifier);
    }

    void set_value_generator(const value_generator_t& generator) {
        m_generator = generator;
    }

    void set_size_getter(const size_getter_t& getter) {
        m_size_getter = getter;
    }

    void set_test_container_printer(const tc_printer_t& printer) {
        m_tc_printer = printer;
    }

    void set_verify_container_printer(const tv_printer_t& printer) {
        m_tv_printer = printer;
    }

    TestLogging::test_result run_operations(size_t num) {
        if (!m_size_getter || !m_generator || !m_tc_printer || !m_tv_printer || m_grow_modifiers.empty() || m_shrink_modifiers.empty() || m_verifiers.empty()) {
            return {"one of the following is empty or unset: grow or shrink modifiers, validators, generator, size_getter, tc or tv printer "};
        }

        auto seed = time(NULL);
        std::srand(0);  // NOTE: if there is an error it is repeatable with constant seed

        TestLogging::test_printf("Random seed for container test is %ld", seed);

        for (size_t i = 0; i < num; ++i) {
            TestLogging::test_result step_result;
            operation op = operation::GROW;

            size_t curent_size = m_size_getter(m_test_container);

            if (curent_size > 0) {
                ssize_t tolerance = (rand() % (m_median_size / 2)) - m_median_size / 4;
                ssize_t current_size_with_tolerance = static_cast<ssize_t>(curent_size) + tolerance;

                op = current_size_with_tolerance > m_median_size ? operation::SHRINK : operation::GROW;

                if (rand() % 100 < 5)
                    op = operation::NEUTRAL;
            }

            // TestLogging::test_printf("running %s operation, size = %ld", op == operations::GROW ? "grow" : "shrink", curent_size);

            std::string last_mod_name = "";

            if (op == operation::GROW && m_grow_modifiers.size()) {
                auto it = random_choice(m_grow_modifiers);
                last_mod_name = it->first;
                step_result = it->second(m_test_container, m_verify_container, m_generator);
            } else if (op == operation::NEUTRAL && m_neutral_modifiers.size()) {
                auto it = random_choice(m_neutral_modifiers);
                last_mod_name = it->first;
                step_result = it->second(m_test_container, m_verify_container, m_generator);
            } else if (m_shrink_modifiers.size()) {
                auto it = random_choice(m_shrink_modifiers);
                last_mod_name = it->first;
                step_result = it->second(m_test_container, m_verify_container);
            }

            // TestLogging::test_printf("%s", step_result.message.c_str());
            // m_tc_printer(m_test_container);
            // TestLogging::test_printf("");

            if (step_result.passed) {
                auto last_mod_description = step_result.message;
                for (auto& verifier : m_verifiers) {
                    step_result = verifier.second(m_test_container, m_verify_container);
                    if (!step_result.passed) {
                        step_result.message = "Test '" + verifier.first + "' failed after applying '" + last_mod_name + "' [" + last_mod_description + "] with reason: " + step_result.message;
                        break;
                    }
                }
            }

            if (!step_result.passed) {
                TestLogging::test_printf("Test Container:");
                m_tc_printer(m_test_container);
                TestLogging::test_printf("Verify Container:");
                m_tv_printer(m_verify_container);
                return step_result;
            }
        }

        return {};
    }

    static size_t default_size_getter(const test_container_t& c) {
        return default_size_getter_templated(c);
    }

    static constexpr unsigned int urand() {
        return static_cast<unsigned>(rand());
    }

    static unsigned int default_uint_generator() {
        return urand() % 100;
    }

    static double default_double_generator() {
        return (urand() % 10000) / 100.0;
    }

    static NonTrivial default_non_trivial_generator() {
        return {default_uint_generator()};
    }

    static void default_test_container_printer(const test_container_t& tc) {
        default_printer_templated(tc);
    }

    static void default_verify_container_printer(const verify_container_t& vc) {
        default_printer_templated(vc);
    }

    static TestLogging::test_result modify_by_copy(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_copy_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_move(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_move_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_assign(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return modify_by_assign_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result modify_by_assign_range(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return modify_by_assign_range_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result modify_by_reserve(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_reserve_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result modify_by_resize(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_resize_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result modify_by_resize_with_value(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return modify_by_resize_with_value_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result modify_by_swap(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_swap_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_shrink_to_fit(test_container_t& tc, verify_container_t& vc, const value_generator_t&) {
        return modify_by_shrink_to_fit_templated(tc, vc);
    }

    static TestLogging::test_result grow_by_push(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_push_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_back(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_push_back_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_back_rvalue(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_push_back_rvalue_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_front(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_push_front_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_front_rvalue(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_push_front_rvalue_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_back(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_emplace_back_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_front(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_emplace_front_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_no_pos(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_emplace_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_emplace_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result grow_by_insert_no_pos(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result grow_by_insert_rvalue_no_pos(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_rvalue_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert_rvalue(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_rvalue_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result grow_by_insert_count(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_count_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result grow_by_insert_range_no_pos(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_range_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert_range(test_container_t& tc, verify_container_t& vc, const value_generator_t& gen) {
        return grow_by_insert_range_templated(tc, vc, gen, default_size_getter);
    }

    static TestLogging::test_result shrink_by_clear(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_clear_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_pop_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop_back(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_pop_back_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop_front(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_pop_front_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator_no_pos(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_iterator_no_pos_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator_for_map(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_iterator_for_map_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_iterator_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result shrink_by_erase_by_value_for_map(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_value_for_map_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result shrink_by_erase_by_value(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_value_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result shrink_by_erase_by_range(test_container_t& tc, verify_container_t& vc) {
        return shrink_by_erase_by_range_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_size(const test_container_t& tc, const verify_container_t& vc) {
        return verify_size_templated(tc, vc, default_size_getter, default_size_getter_templated<verify_container_t>);
    }

    static TestLogging::test_result verify_front(const test_container_t& tc, const verify_container_t& vc) {
        return verify_front_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_back(const test_container_t& tc, const verify_container_t& vc) {
        return verify_back_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_element_position_with_at_for_map(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_position_with_at_for_map_templated(tc, vc);
    }

    static TestLogging::test_result verify_element_position_with_at(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_position_with_at_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_element_position_with_operator_brackets_for_map(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_position_with_operator_brackets_for_map_templated(const_cast<test_container_t&>(tc), const_cast<verify_container_t&>(vc));
    }

    static TestLogging::test_result verify_element_position_with_operator_brackets(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_position_with_operator_brackets_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_element_position_with_iterator(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_position_with_iterator_templated(tc, vc);
    }

    static TestLogging::test_result verify_front_element_position(const test_container_t& tc, const verify_container_t& vc) {
        return verify_front_element_position_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_back_element_position(const test_container_t& tc, const verify_container_t& vc) {
        return verify_back_element_position_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_find_existing_element_for_map(const test_container_t& tc, const verify_container_t& vc) {
        return verify_find_existing_element_for_map_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_find_existing_element(const test_container_t& tc, const verify_container_t& vc) {
        return verify_find_existing_element_templated(tc, vc, default_size_getter);
    }

    static TestLogging::test_result verify_element_inclusion_for_map(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_inclusion_for_map_templated(tc, vc);
    }

    static TestLogging::test_result verify_element_inclusion(const test_container_t& tc, const verify_container_t& vc) {
        return verify_element_inclusion_templated(tc, vc);
    }

    template <typename Container>
    static size_t default_size_getter_templated(const Container& c) {
        return c.size();
    }

    template <typename Container>
    static void default_printer_templated(const Container& c) {
        TestLogging::test_printf_start("[");
        for (const auto& item : c) {
            TestLogging::test_printf_append(" ");
            print_item(item);
            TestLogging::test_printf_append(",");
        }
        TestLogging::test_printf_end("]");
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result modify_by_copy_templated(TestContainer_& tc, VerifyContainer_& vc) {
        auto tc_copy = TestContainer_{tc};
        auto vc_copy = VerifyContainer_{vc};

        tc = tc_copy;
        vc = vc_copy;

        return {"copy", true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result modify_by_move_templated(TestContainer_& tc, VerifyContainer_& vc) {
        auto tc_moved = TestContainer_{std::move(tc)};
        auto vc_moved = VerifyContainer_{std::move(vc)};

        tc = std::move(tc_moved);
        vc = std::move(vc_moved);

        return {"move", true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result modify_by_assign_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto size = size_getter(tc);
        auto value = gen();

        tc.assign(size, value);
        vc.assign(size, value);

        return {"assign " + to_string(value) + " x " + to_string(size), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result modify_by_assign_range_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto size = size_getter(tc);
        std::vector<typename TestContainer_::value_type> range;

        for (size_t i = 0; i < size; ++i)
            range.push_back(gen());

        tc.assign(range.begin(), range.end());
        vc.assign(range.begin(), range.end());

        return {"assign range", true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result modify_by_reserve_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto new_size = random_new_size(tc, size_getter, 10, 10);

        tc.reserve(new_size);
        vc.reserve(new_size);

        return {"reserve " + to_string(new_size), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result modify_by_resize_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto new_size = random_new_size(tc, size_getter, 10, 10);

        tc.resize(new_size);
        vc.resize(new_size);

        return {"resize to " + to_string(new_size), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result modify_by_resize_with_value_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        auto new_size = random_new_size(tc, size_getter, 10, 10);

        tc.resize(new_size, value);
        vc.resize(new_size, value);

        return {"resize to " + to_string(new_size) + " with " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result modify_by_swap_templated(TestContainer_& tc, VerifyContainer_& vc) {
        auto tc_copy = TestContainer_{tc};
        auto vc_copy = VerifyContainer_{vc};

        tc.swap(tc_copy);
        vc.swap(vc_copy);

        return {"swap", true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result modify_by_shrink_to_fit_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.shrink_to_fit();
        vc.shrink_to_fit();

        return {"shrink_to_fit", true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_push_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push(value);
        vc.push(value);

        return {"push value " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_push_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push_back(value);
        vc.push_back(value);

        return {"push_back value " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_push_back_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        auto value_copy = value;
        auto value_copy2 = value;

        tc.push_back(std::move(value));
        vc.push_back(std::move(value_copy));

        return {"push_back rvalue " + to_string(value_copy2), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_push_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        tc.push_front(value);
        vc.push_front(value);

        return {"push_front value " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_push_front_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        auto value_copy = value;
        auto value_copy2 = value;

        tc.push_front(std::move(value));
        vc.push_front(std::move(value_copy));

        return {"push_front rvalue " + to_string(value_copy2), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_emplace_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();

        return return_result_check(tc, vc,
                                   tc.emplace_back(value),
                                   vc.emplace_back(value),
                                   "emplace_back value " + to_string(value));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_emplace_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();

        return return_result_check(tc, vc,
                                   tc.emplace_front(value),
                                   vc.emplace_front(value),
                                   "emplace_front value " + to_string(value));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_emplace_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();

        tc.emplace(value);
        vc.emplace(value);
        return {"emplace value " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result grow_by_emplace_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        auto idx = random_index(tc, size_getter);

        return return_result_check(tc, vc,
                                   tc.emplace(advance_copy(tc.begin(), idx), value),
                                   vc.emplace(advance_copy(vc.begin(), idx), value),
                                   "emplace value " + to_string(value) + " at index " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_insert_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();

        tc.insert(value);
        vc.insert(value);
        return {"insert value " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result grow_by_insert_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        auto idx = random_index(tc, size_getter);

        tc.insert(advance_copy(tc.begin(), idx), value);
        vc.insert(advance_copy(vc.begin(), idx), value);

        return return_result_check(tc, vc,
                                   tc.insert(advance_copy(tc.begin(), idx), value),
                                   vc.insert(advance_copy(vc.begin(), idx), value),
                                   "insert value " + to_string(value) + " at index " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_insert_rvalue_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        auto value = gen();
        auto value_copy = value;
        auto value_copy2 = value;

        tc.insert(std::move(value));
        vc.insert(std::move(value_copy));
        return {"insert rvalue " + to_string(value_copy2), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result grow_by_insert_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        auto value_copy = value;
        auto value_copy2 = value;
        auto idx = random_index(tc, size_getter);

        return return_result_check(tc, vc,
                                   tc.insert(advance_copy(tc.begin(), idx), std::move(value)),
                                   vc.insert(advance_copy(vc.begin(), idx), std::move(value_copy)),
                                   "insert rvalue " + to_string(value_copy2) + " at index " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result grow_by_insert_count_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        auto value = gen();
        size_t count = urand() % 10;
        auto idx = random_index(tc, size_getter);

        return return_result_check(tc, vc,
                                   tc.insert(advance_copy(tc.begin(), idx), count, value),
                                   vc.insert(advance_copy(vc.begin(), idx), count, value),
                                   "insert count " + to_string(count) + " of " + to_string(value) + " at index " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator>
    static TestLogging::test_result grow_by_insert_range_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
        size_t size = urand() % 10;
        std::vector<typename TestContainer_::value_type> range;

        for (size_t i = 0; i < size; ++i)
            range.push_back(gen());

        tc.insert(range.begin(), range.end());
        vc.insert(range.begin(), range.end());
        return {"insert range of size " + to_string(size), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
    static TestLogging::test_result grow_by_insert_range_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
        size_t size = urand() % 10;
        std::vector<typename TestContainer_::value_type> range;

        for (size_t i = 0; i < size; ++i)
            range.push_back(gen());

        auto idx = random_index(tc, size_getter);

        return return_result_check(tc, vc,
                                   tc.insert(advance_copy(tc.begin(), idx), range.begin(), range.end()),
                                   vc.insert(advance_copy(vc.begin(), idx), range.begin(), range.end()),
                                   "insert range of size " + to_string(size) + " at index " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result shrink_by_clear_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.clear();
        vc.clear();

        return {"clear", true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result shrink_by_pop_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop();
        vc.pop();

        return {"pop", true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result shrink_by_pop_back_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop_back();
        vc.pop_back();

        return {"pop_back", true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result shrink_by_pop_front_templated(TestContainer_& tc, VerifyContainer_& vc) {
        tc.pop_front();
        vc.pop_front();

        return {"pop_front", true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_iterator_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx = random_index(tc, size_getter);

        auto tc_it = advance_copy(tc.begin(), idx);

        auto value = *tc_it;
        auto vc_it = vc.find(value);

        tc.erase(tc_it);
        vc.erase(vc_it);

        return {"erase at idx " + to_string(idx) + " with value: " + to_string(value), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_iterator_for_map_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx = random_index(tc, size_getter);

        auto tc_it = advance_copy(tc.begin(), idx);

        auto value = *tc_it;
        auto vc_it = vc.find(value.first);

        tc.erase(tc_it);
        vc.erase(vc_it);

        return {"erase at idx " + to_string(idx) + " with value: " + to_string(value.first), true};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_iterator_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx = random_index(tc, size_getter);

        return return_result_check(tc, vc,
                                   tc.erase(advance_copy(tc.begin(), idx)),
                                   vc.erase(advance_copy(vc.begin(), idx)),
                                   "erase at idx " + to_string(idx));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_value_for_map_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx = random_index(tc, size_getter);
        auto value = *advance_copy(tc.begin(), idx);

        return return_result_check(tc, vc,
                                   tc.erase(value.first),
                                   vc.erase(value.first),
                                   "erase value " + to_string(value));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_value_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx = random_index(tc, size_getter);
        auto value = *advance_copy(tc.begin(), idx);

        return return_result_check(tc, vc,
                                   tc.erase(value),
                                   vc.erase(value),
                                   "erase value " + to_string(value));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result shrink_by_erase_by_range_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
        auto idx1 = random_index(tc, size_getter);
        auto idx2 = random_index(tc, size_getter);

        auto min = std::min(idx1, idx2);
        auto max = std::max(idx1, idx2);

        return return_result_check(tc, vc,
                                   tc.erase(advance_copy(tc.begin(), min), advance_copy(tc.begin(), max)),
                                   vc.erase(advance_copy(vc.begin(), min), advance_copy(vc.begin(), max)),
                                   "erase range from " + to_string(min) + " to " + to_string(max));
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter, typename VerifyContainerSizeGetter>
    static TestLogging::test_result verify_size_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter, const VerifyContainerSizeGetter& vc_size_getter) {
        if (tc_size_getter(tc) != vc_size_getter(vc))
            return {"sizes don't match: TestContainer: " + to_string(tc_size_getter(tc)) + " VerifyContainer: " + to_string(vc_size_getter(vc))};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_front_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        if (tc_size_getter(tc) == 0) return {};
        if (tc.front() != vc.front())
            return {"fronts don't match: TestContainer: " + to_string(tc.front()) + " VerifyContainer: " + to_string(vc.front())};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_back_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        if (tc_size_getter(tc) == 0) return {};
        if (tc.back() != vc.back())
            return {"backs don't match: TestContainer: " + to_string(tc.back()) + " VerifyContainer: " + to_string(vc.back())};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result verify_element_position_with_at_for_map_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        for (auto& elt : vc)
            if (tc.at(elt.first) != vc.at(elt.first))
                return {"values don't match at idx " + to_string(elt.first) + " TestContainer: " + to_string(tc.at(elt.first)) + " VerifyContainer: " + to_string(vc.at(elt.first))};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_element_position_with_at_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        for (size_t idx = 0; idx < tc_size_getter(tc); ++idx)
            if (tc.at(idx) != vc.at(idx))
                return {"values don't match at idx " + to_string(idx) + " TestContainer: " + to_string(tc.at(idx)) + " VerifyContainer: " + to_string(vc.at(idx))};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result verify_element_position_with_operator_brackets_for_map_templated(TestContainer_& tc, VerifyContainer_& vc) {
        for (auto& elt : vc)
            if (tc[elt.first] != vc[elt.first])
                return {"values don't match at idx " + to_string(elt.first) + " TestContainer: " + to_string(tc[elt.first]) + " VerifyContainer: " + to_string(vc[elt.first])};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_element_position_with_operator_brackets_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
        for (size_t idx = 0; idx < tc_size_getter(tc); ++idx)
            if (tc[idx] != vc[idx])
                return {"values don't match at idx " + to_string(idx) + " TestContainer: " + to_string(tc[idx]) + " VerifyContainer: " + to_string(vc[idx])};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result verify_element_position_with_iterator_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        auto tc_it = tc.begin();
        auto vc_it = vc.begin();
        size_t pos = 0;

        while (true) {
            auto tc_is_end = tc_it == tc.end();
            auto vc_is_end = vc_it == vc.end();

            if (tc_is_end and vc_is_end)
                return {};

            if (tc_is_end != vc_is_end)
                return {"one iterator indicated end before the other"};

            if (*tc_it != *vc_it)
                return {"values don't match at position " + to_string(pos) + " TestContainer: " + to_string(*tc_it) + " VerifyContainer: " + to_string(*vc_it)};

            tc_it++;
            vc_it++;
            pos++;
        }
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_front_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter&) {
        if (tc.front() != vc.front())
            return {"front values don't match TestContainer: " + to_string(tc.front()) + " VerifyContainer: " + to_string(vc.front())};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_back_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter&) {
        if (tc.back() != vc.back())
            return {"back values don't match TestContainer: " + to_string(tc.back()) + " VerifyContainer: " + to_string(vc.back())};

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_find_existing_element_for_map_templated(const TestContainer_& tc, const VerifyContainer_&, const TestContainerSizeGetter& tc_size_getter) {
        if (tc_size_getter(tc) == 0) return {};

        auto idx = random_index(tc, tc_size_getter);

        auto find_value = *advance_copy(tc.begin(), idx);
        auto res = tc.find(find_value.first);

        if (res == tc.end())
            return {"find failed on existing value " + to_string(find_value.first)};

        if (*res != find_value) {
            return {"find returned wrong iterator to value " + to_string(find_value.first)};
        }

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
    static TestLogging::test_result verify_find_existing_element_templated(const TestContainer_& tc, const VerifyContainer_&, const TestContainerSizeGetter& tc_size_getter) {
        if (tc_size_getter(tc) == 0) return {};

        auto idx = random_index(tc, tc_size_getter);

        auto find_value = *advance_copy(tc.begin(), idx);
        auto res = find(tc.begin(), tc.end(), find_value);

        if (res == tc.end())
            return {"find failed on existing value " + to_string(find_value)};

        if (*res != find_value) {
            return {"find returned wrong iterator to value " + to_string(find_value)};
        }

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result verify_element_inclusion_for_map_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        for (const auto& elt : tc) {
            auto res = vc.find(elt.first);
            if (res == vc.end())
                return {"element " + to_string(elt) + " exists in TestContainer, but not in VerifyContainer"};
            if (*res != elt)
                return {"element " + to_string(elt) + " has deviating value in VerifyContainer " + to_string(*res)};
        }

        for (auto& elt : vc) {
            auto res = tc.find(elt.first);
            if (res == tc.end())
                return {"element " + to_string(elt) + " exists in VerifyContainer, but not in TestContainer"};
            if (*res != elt)
                return {"element " + to_string(elt) + " has deviating value in TestContainer " + to_string(*res)};
        }

        return {};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result verify_element_inclusion_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
        for (auto& elt : tc) {
            if (vc.find(elt) == vc.end())
                return {"element " + to_string(elt) + " exists in TestContainer, but not in VerifyContainer"};
        }

        for (auto& elt : vc) {
            if (tc.find(elt) == tc.end())
                return {"element " + to_string(elt) + " exists in VerifyContainer, but not in TestContainer"};
        }

        return {};
    }

    test_container_t& tc() {
        return m_test_container;
    }

    verify_container_t& vc() {
        return m_verify_container;
    }

   private:
    template <typename Container, typename SizeGetter>
    static constexpr size_t random_index(const Container& c, const SizeGetter& s) {
        size_t size = s(c);
        return size ? urand() % size : 0;
    }

    template <typename Container, typename SizeGetter>
    static constexpr size_t random_new_size(const Container& c, const SizeGetter& s, size_t max_minus, size_t max_plus) {
        size_t size = s(c);
        return size + (urand() % (max_minus + max_plus)) - std::min(max_minus, size);
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result return_result_check(const TestContainer_& tc, const VerifyContainer_& vc, const typename TestContainer_::const_iterator& tc_it, const typename VerifyContainer_::const_iterator& vc_it, const std::string& op_name) {
        size_t tc_count = 0, vc_count = 0;
        auto tc_cursor = tc.cbegin();
        auto vc_cursor = vc.cbegin(); 
        for (;tc_cursor != tc.cend() && vc_cursor != vc.cend();) {
            bool tc_match = tc_cursor == tc_it;
            bool vc_match = vc_cursor == vc_it;

            if (!tc_match) tc_count++, tc_cursor++;
            if (!vc_match) vc_count++, vc_cursor++;

            if (tc_match && vc_match) break;
        }

        if (tc_count != vc_count)
            return {"Return value mismatch [Iterator] on position: tc: begin() + " + to_string(tc_count) + " vc: begin() + " + to_string(vc_count) + " on operation: " + op_name};

        if (tc_it != tc.cend() && vc_it != vc.cend() && *tc_it != *vc_it)
            return {"Return value mismatch [Iterator] on value: tc: " + to_string(*tc_it) + " vc: " + to_string(*vc_it) + " on operation: " + op_name};

        return {op_name, true};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result return_result_check(const TestContainer_&, const VerifyContainer_&, typename TestContainer_::const_reference tc_val, typename VerifyContainer_::const_reference vc_val, const std::string& op_name) {
        if (tc_val == vc_val)
            return {op_name, true};

        return {"Return value mismatch [Value]: tc: " + to_string(tc_val) + " vc: " + to_string(vc_val) + " on operation: " + op_name};
    }

    template <typename TestContainer_, typename VerifyContainer_>
    static TestLogging::test_result return_result_check(const TestContainer_&, const VerifyContainer_&, typename TestContainer_::size_type tc_val, typename VerifyContainer_::size_type vc_val, const std::string& op_name) {
        if (tc_val == vc_val)
            return {op_name, true};

        return {"Return value mismatch [Size]: tc: " + to_string(tc_val) + " vc: " + to_string(vc_val) + " on operation: " + op_name};
    }

    template <typename TestContainer_, typename VerifyContainer_, template <typename, typename> class tc_pair_t, template <typename, typename> class vc_pair_t>
    static TestLogging::test_result return_result_check(const TestContainer_& tc, const VerifyContainer_& vc, const tc_pair_t<typename TestContainer_::const_iterator, bool>& tc_res, const vc_pair_t<typename VerifyContainer_::const_iterator, bool>& vc_res, const std::string& op_name) {
        if (tc_res.second != vc_res.second) return {"Return value mismatch [pair<Iterator,bool>] on bool: tc: " + to_string(tc_res.second) + " vc: " + to_string(vc_res.second) + " on operation: " + op_name};
        if (tc_res.first != tc.cend() && vc_res.first != vc.cend() && *tc_res.first != *vc_res.first) return {"Return value mismatch [pair<Iterator,bool>] on iterator value: tc: " + to_string(tc_res.second) + " vc: " + to_string(vc_res.second) + " on operation: " + op_name};

        return {op_name, true};
    }

    template <typename Iterator>
    static Iterator advance_copy(const Iterator& cit, size_t n) {
        Iterator it = cit;
        for (size_t i = 0; i < n; i++)
            it++;
        return it;
    }

    template <typename Container>
    static auto random_choice(const Container& from) {
        size_t idx = urand() % from.size();
        auto it = from.begin();
        std::advance(it, idx);
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

    enum operation {
        SHRINK,
        NEUTRAL,
        GROW
    };

    ssize_t m_median_size = 25;

    test_container_t m_test_container;
    verify_container_t m_verify_container;

    std::unordered_map<std::string, neutral_modifier_t> m_neutral_modifiers;
    std::unordered_map<std::string, grow_modifier_t> m_grow_modifiers;
    std::unordered_map<std::string, shrink_modifier_t> m_shrink_modifiers;
    std::unordered_map<std::string, verifier_t> m_verifiers;

    value_generator_t m_generator;
    size_getter_t m_size_getter;

    tc_printer_t m_tc_printer;
    tv_printer_t m_tv_printer;
};

template <typename Derived, template <typename...> class test_container_t, template <typename...> class verify_container_t>
class ContainerTestDefaultMixin {
   public:
    static TestLogging::test_result run_with_int(size_t operation_count) {
        typedef ContainerTester<test_container_t<int>, verify_container_t<int>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_uint_generator);

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_non_trivial(size_t operation_count) {
        typedef ContainerTester<test_container_t<NonTrivial>, verify_container_t<NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator(tester_t::default_non_trivial_generator);

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_int_int(size_t operation_count) {
        typedef ContainerTester<test_container_t<int, int>, verify_container_t<int, int>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, int> {
            return {tester_t::default_uint_generator(), tester_t::default_uint_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    }

    static TestLogging::test_result run_with_int_non_trivial(size_t operation_count) {
        typedef ContainerTester<test_container_t<int, NonTrivial>, verify_container_t<int, NonTrivial>> tester_t;
        tester_t tester;

        tester.set_value_generator([]() -> std::pair<const int, NonTrivial> {
            return {tester_t::default_uint_generator(), tester_t::default_non_trivial_generator()};
        });

        return Derived::run_templated(tester, operation_count);
    }
};