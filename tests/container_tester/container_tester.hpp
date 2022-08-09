#pragma once

#include <time.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ftest/test_logging.hpp>
#include <functional>
#include <string>
#include <unordered_map>

#include "../non_trivial.hpp"
#include "container_tester_helpers.hpp"
#include "impl/pair.hpp"

namespace container_tester {

template <typename TestContainer, typename VerifyContainer, typename ContainerValueType = typename VerifyContainer::value_type>
class ContainerTester {
   public:
    typedef TestContainer test_container_t;
    typedef VerifyContainer verify_container_t;

    typedef std::function<size_t(const test_container_t&)> tc_size_getter_t;
    typedef std::function<size_t(const verify_container_t&)> vc_size_getter_t;

    typedef std::function<ContainerValueType()> value_generator_t;

    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&, const tc_size_getter_t&, const value_generator_t&)> neutral_modifier_t;
    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&, const tc_size_getter_t&, const value_generator_t&)> grow_modifier_t;
    typedef std::function<TestLogging::test_result(test_container_t&, verify_container_t&, const tc_size_getter_t&)> shrink_modifier_t;

    typedef std::function<TestLogging::test_result(const test_container_t&, const verify_container_t&, const tc_size_getter_t&, const vc_size_getter_t&)> verifier_t;

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

    void set_test_container_size_getter(const tc_size_getter_t& getter) {
        m_tc_size_getter = getter;
    }

    void set_verify_container_size_getter(const vc_size_getter_t& getter) {
        m_vc_size_getter = getter;
    }

    void set_test_container_printer(const tc_printer_t& printer) {
        m_tc_printer = printer;
    }

    void set_verify_container_printer(const tv_printer_t& printer) {
        m_tv_printer = printer;
    }

    TestLogging::test_result run_operations(size_t num) {
        if (!m_tc_size_getter || !m_vc_size_getter || !m_generator || !m_tc_printer || !m_tv_printer || m_grow_modifiers.empty() || m_shrink_modifiers.empty() || m_verifiers.empty()) {
            return {"one of the following is empty or unset: grow or shrink modifiers, validators, generator, size_getter, tc or tv printer "};
        }

        auto seed = time(NULL);
        std::srand(0);  // NOTE: if there is an error it is repeatable with constant seed

        TestLogging::test_printf("Random seed for container test is %ld", seed);

        for (size_t i = 0; i < num; ++i) {
            TestLogging::test_result step_result;
            operation op = operation::GROW;

            size_t curent_size = m_tc_size_getter(m_test_container);

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
                step_result = it->second(m_test_container, m_verify_container, m_tc_size_getter, m_generator);
            } else if (op == operation::NEUTRAL && m_neutral_modifiers.size()) {
                auto it = random_choice(m_neutral_modifiers);
                last_mod_name = it->first;
                step_result = it->second(m_test_container, m_verify_container, m_tc_size_getter, m_generator);
            } else if (m_shrink_modifiers.size()) {
                auto it = random_choice(m_shrink_modifiers);
                last_mod_name = it->first;
                step_result = it->second(m_test_container, m_verify_container, m_tc_size_getter);
            }

            // TestLogging::test_printf("%s", step_result.message.c_str());
            // m_tc_printer(m_test_container);
            // TestLogging::test_printf("");

            if (step_result.passed) {
                auto last_mod_description = step_result.message;
                for (auto& verifier : m_verifiers) {
                    step_result = verifier.second(m_test_container, m_verify_container, m_tc_size_getter, m_vc_size_getter);
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

    static size_t default_test_container_size_getter(const test_container_t& c) {
        return default_size_getter_templated(c);
    }

    static size_t default_verify_container_size_getter(const verify_container_t& c) {
        return default_size_getter_templated(c);
    }

    static void default_test_container_printer(const test_container_t& tc) {
        default_printer_templated(tc);
    }

    static void default_verify_container_printer(const verify_container_t& vc) {
        default_printer_templated(vc);
    }

    static TestLogging::test_result modify_by_copy(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t&) {
        return modify_by_copy_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_move(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t&) {
        return modify_by_move_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_assign(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return modify_by_assign_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result modify_by_assign_range(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return modify_by_assign_range_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result modify_by_reserve(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t&) {
        return modify_by_reserve_templated(tc, vc, sg);
    }

    static TestLogging::test_result modify_by_resize(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t&) {
        return modify_by_resize_templated(tc, vc, sg);
    }

    static TestLogging::test_result modify_by_resize_with_value(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return modify_by_resize_with_value_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result modify_by_swap(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t&) {
        return modify_by_swap_templated(tc, vc);
    }

    static TestLogging::test_result modify_by_shrink_to_fit(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t&) {
        return modify_by_shrink_to_fit_templated(tc, vc);
    }

    static TestLogging::test_result grow_by_push(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_push_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_back(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_push_back_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_back_rvalue(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_push_back_rvalue_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_front(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_push_front_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_push_front_rvalue(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_push_front_rvalue_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_back(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_emplace_back_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_front(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_emplace_front_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace_no_pos(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_emplace_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_emplace(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return grow_by_emplace_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result grow_by_insert_no_pos(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_insert_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return grow_by_insert_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result grow_by_insert_rvalue_no_pos(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_insert_rvalue_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert_rvalue(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return grow_by_insert_rvalue_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result grow_by_insert_count(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return grow_by_insert_count_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result grow_by_insert_range_no_pos(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&, const value_generator_t& gen) {
        return grow_by_insert_range_no_pos_templated(tc, vc, gen);
    }

    static TestLogging::test_result grow_by_insert_range(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg, const value_generator_t& gen) {
        return grow_by_insert_range_templated(tc, vc, gen, sg);
    }

    static TestLogging::test_result shrink_by_clear(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&) {
        return shrink_by_clear_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&) {
        return shrink_by_pop_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop_back(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&) {
        return shrink_by_pop_back_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_pop_front(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t&) {
        return shrink_by_pop_front_templated(tc, vc);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator_no_pos(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_iterator_no_pos_templated(tc, vc, sg);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator_for_map(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_iterator_for_map_templated(tc, vc, sg);
    }

    static TestLogging::test_result shrink_by_erase_by_iterator(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_iterator_templated(tc, vc, sg);
    }

    static TestLogging::test_result shrink_by_erase_by_value_for_map(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_value_for_map_templated(tc, vc, sg);
    }

    static TestLogging::test_result shrink_by_erase_by_value(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_value_templated(tc, vc, sg);
    }

    static TestLogging::test_result shrink_by_erase_by_range(test_container_t& tc, verify_container_t& vc, const tc_size_getter_t& sg) {
        return shrink_by_erase_by_range_templated(tc, vc, sg);
    }

    static TestLogging::test_result verify_size(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t& vc_sg) {
        return verify_size_templated(tc, vc, tc_sg, vc_sg);
    }

    static TestLogging::test_result verify_front(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_front_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_back(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_back_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_element_position_with_at_for_map(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t&, const vc_size_getter_t&) {
        return verify_element_position_with_at_for_map_templated(tc, vc);
    }

    static TestLogging::test_result verify_element_position_with_at(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_element_position_with_at_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_element_position_with_operator_brackets_for_map(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t&, const vc_size_getter_t&) {
        return verify_element_position_with_operator_brackets_for_map_templated(const_cast<test_container_t&>(tc), const_cast<verify_container_t&>(vc));
    }

    static TestLogging::test_result verify_element_position_with_operator_brackets(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_element_position_with_operator_brackets_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_element_position_with_iterator(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t&, const vc_size_getter_t&) {
        return verify_element_position_with_iterator_templated(tc, vc);
    }

    static TestLogging::test_result verify_front_element_position(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_front_element_position_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_back_element_position(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_back_element_position_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_find_existing_element_for_map(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_find_existing_element_for_map_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_find_existing_element(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t& tc_sg, const vc_size_getter_t&) {
        return verify_find_existing_element_templated(tc, vc, tc_sg);
    }

    static TestLogging::test_result verify_element_inclusion_for_map(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t&, const vc_size_getter_t&) {
        return verify_element_inclusion_for_map_templated(tc, vc);
    }

    static TestLogging::test_result verify_element_inclusion(const test_container_t& tc, const verify_container_t& vc, const tc_size_getter_t&, const vc_size_getter_t&) {
        return verify_element_inclusion_templated(tc, vc);
    }

    test_container_t& tc() {
        return m_test_container;
    }

    verify_container_t& vc() {
        return m_verify_container;
    }

   private:
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

    tc_size_getter_t m_tc_size_getter;
    vc_size_getter_t m_vc_size_getter;

    tc_printer_t m_tc_printer;
    tv_printer_t m_tv_printer;
};
}  // namespace container_tester