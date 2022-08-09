#pragma once

#include <ftest/test_logging.hpp>

#include "../non_trivial.hpp"

namespace container_tester {

unsigned int urand();

unsigned int default_uint_generator();

double default_double_generator();

NonTrivial default_non_trivial_generator();

template <typename Iterator>
Iterator advance_copy(const Iterator& cit, size_t n) {
    Iterator it = cit;
    for (size_t i = 0; i < n; i++)
        it++;
    return it;
}

template <typename Container>
auto random_choice(const Container& from) {
    size_t idx = urand() % from.size();
    auto it = from.begin();
    std::advance(it, idx);
    return it;
}

template <typename T>
void print_item(const T& t) {
    TestLogging::test_printf_append("%3d", t);
}

void print_item(const std::string& t);
void print_item(const NonTrivial& t);

template <template <typename, typename> class S, typename A, typename B>
void print_item(const S<A, B>& t) {
    TestLogging::test_printf_append("(");
    print_item(t.first);
    TestLogging::test_printf_append(", ");
    print_item(t.second);
    TestLogging::test_printf_append(")");
}

template <typename T>
std::string to_string(const T& t) {
    return std::to_string(t);
}

std::string to_string(const NonTrivial& t);
std::string to_string(const std::string& t);

template <template <typename, typename> class S, typename A, typename B>
std::string to_string(const S<A, B>& t) {
    return "(" + to_string(t.first) + ", " + to_string(t.second) + ")";
}

template <typename Container, typename SizeGetter>
constexpr size_t random_index(const Container& c, const SizeGetter& s) {
    size_t size = s(c);
    return size ? urand() % size : 0;
}

template <typename Container, typename SizeGetter>
constexpr size_t random_new_size(const Container& c, const SizeGetter& s, size_t max_minus, size_t max_plus) {
    size_t size = s(c);
    return size + (urand() % (max_minus + max_plus)) - std::min(max_minus, size);
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result return_result_check(const TestContainer_& tc, const VerifyContainer_& vc, const typename TestContainer_::const_iterator& tc_it, const typename VerifyContainer_::const_iterator& vc_it, const std::string& op_name) {
    size_t tc_count = 0, vc_count = 0;
    auto tc_cursor = tc.cbegin();
    auto vc_cursor = vc.cbegin();
    for (; tc_cursor != tc.cend() && vc_cursor != vc.cend();) {
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
TestLogging::test_result return_result_check(const TestContainer_&, const VerifyContainer_&, typename TestContainer_::const_reference tc_val, typename VerifyContainer_::const_reference vc_val, const std::string& op_name) {
    if (tc_val == vc_val)
        return {op_name, true};

    return {"Return value mismatch [Value]: tc: " + to_string(tc_val) + " vc: " + to_string(vc_val) + " on operation: " + op_name};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result return_result_check(const TestContainer_&, const VerifyContainer_&, typename TestContainer_::size_type tc_val, typename VerifyContainer_::size_type vc_val, const std::string& op_name) {
    if (tc_val == vc_val)
        return {op_name, true};

    return {"Return value mismatch [Size]: tc: " + to_string(tc_val) + " vc: " + to_string(vc_val) + " on operation: " + op_name};
}

template <typename TestContainer_, typename VerifyContainer_, template <typename, typename> class tc_pair_t, template <typename, typename> class vc_pair_t>
TestLogging::test_result return_result_check(const TestContainer_& tc, const VerifyContainer_& vc, const tc_pair_t<typename TestContainer_::const_iterator, bool>& tc_res, const vc_pair_t<typename VerifyContainer_::const_iterator, bool>& vc_res, const std::string& op_name) {
    if (tc_res.second != vc_res.second) return {"Return value mismatch [pair<Iterator,bool>] on bool: tc: " + to_string(tc_res.second) + " vc: " + to_string(vc_res.second) + " on operation: " + op_name};
    if (tc_res.first != tc.cend() && vc_res.first != vc.cend() && *tc_res.first != *vc_res.first) return {"Return value mismatch [pair<Iterator,bool>] on iterator value: tc: " + to_string(tc_res.second) + " vc: " + to_string(vc_res.second) + " on operation: " + op_name};

    return {op_name, true};
}

template <typename Container>
size_t default_size_getter_templated(const Container& c) {
    return c.size();
}

template <typename Container>
void default_printer_templated(const Container& c) {
    TestLogging::test_printf_start("[");
    for (const auto& item : c) {
        TestLogging::test_printf_append(" ");
        print_item(item);
        TestLogging::test_printf_append(",");
    }
    TestLogging::test_printf_end("]");
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result modify_by_copy_templated(TestContainer_& tc, VerifyContainer_& vc) {
    auto tc_copy = TestContainer_{tc};
    auto vc_copy = VerifyContainer_{vc};

    tc = tc_copy;
    vc = vc_copy;

    return {"copy", true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result modify_by_move_templated(TestContainer_& tc, VerifyContainer_& vc) {
    auto tc_moved = TestContainer_{std::move(tc)};
    auto vc_moved = VerifyContainer_{std::move(vc)};

    tc = std::move(tc_moved);
    vc = std::move(vc_moved);

    return {"move", true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result modify_by_assign_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
    auto size = size_getter(tc);
    auto value = gen();

    tc.assign(size, value);
    vc.assign(size, value);

    return {"assign " + to_string(value) + " x " + to_string(size), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result modify_by_assign_range_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
    auto size = size_getter(tc);
    std::vector<typename TestContainer_::value_type> range;

    for (size_t i = 0; i < size; ++i)
        range.push_back(gen());

    tc.assign(range.begin(), range.end());
    vc.assign(range.begin(), range.end());

    return {"assign range", true};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result modify_by_reserve_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto new_size = random_new_size(tc, size_getter, 10, 10);

    tc.reserve(new_size);
    vc.reserve(new_size);

    return {"reserve " + to_string(new_size), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result modify_by_resize_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto new_size = random_new_size(tc, size_getter, 10, 10);

    tc.resize(new_size);
    vc.resize(new_size);

    return {"resize to " + to_string(new_size), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result modify_by_resize_with_value_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
    auto value = gen();
    auto new_size = random_new_size(tc, size_getter, 10, 10);

    tc.resize(new_size, value);
    vc.resize(new_size, value);

    return {"resize to " + to_string(new_size) + " with " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result modify_by_swap_templated(TestContainer_& tc, VerifyContainer_& vc) {
    auto tc_copy = TestContainer_{tc};
    auto vc_copy = VerifyContainer_{vc};

    tc.swap(tc_copy);
    vc.swap(vc_copy);

    return {"swap", true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result modify_by_shrink_to_fit_templated(TestContainer_& tc, VerifyContainer_& vc) {
    tc.shrink_to_fit();
    vc.shrink_to_fit();

    return {"shrink_to_fit", true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_push_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    tc.push(value);
    vc.push(value);

    return {"push value " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_push_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    tc.push_back(value);
    vc.push_back(value);

    return {"push_back value " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_push_back_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    auto value_copy = value;
    auto value_copy2 = value;

    tc.push_back(std::move(value));
    vc.push_back(std::move(value_copy));

    return {"push_back rvalue " + to_string(value_copy2), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_push_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    tc.push_front(value);
    vc.push_front(value);

    return {"push_front value " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_push_front_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    auto value_copy = value;
    auto value_copy2 = value;

    tc.push_front(std::move(value));
    vc.push_front(std::move(value_copy));

    return {"push_front rvalue " + to_string(value_copy2), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_emplace_back_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();

    return return_result_check(tc, vc,
                               tc.emplace_back(value),
                               vc.emplace_back(value),
                               "emplace_back value " + to_string(value));
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_emplace_front_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();

    return return_result_check(tc, vc,
                               tc.emplace_front(value),
                               vc.emplace_front(value),
                               "emplace_front value " + to_string(value));
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_emplace_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();

    tc.emplace(value);
    vc.emplace(value);
    return {"emplace value " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result grow_by_emplace_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
    auto value = gen();
    auto idx = random_index(tc, size_getter);

    return return_result_check(tc, vc,
                               tc.emplace(advance_copy(tc.begin(), idx), value),
                               vc.emplace(advance_copy(vc.begin(), idx), value),
                               "emplace value " + to_string(value) + " at index " + to_string(idx));
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_insert_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();

    tc.insert(value);
    vc.insert(value);
    return {"insert value " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result grow_by_insert_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
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
TestLogging::test_result grow_by_insert_rvalue_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    auto value = gen();
    auto value_copy = value;
    auto value_copy2 = value;

    tc.insert(std::move(value));
    vc.insert(std::move(value_copy));
    return {"insert rvalue " + to_string(value_copy2), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result grow_by_insert_rvalue_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
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
TestLogging::test_result grow_by_insert_count_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
    auto value = gen();
    size_t count = urand() % 10;
    auto idx = random_index(tc, size_getter);

    return return_result_check(tc, vc,
                               tc.insert(advance_copy(tc.begin(), idx), count, value),
                               vc.insert(advance_copy(vc.begin(), idx), count, value),
                               "insert count " + to_string(count) + " of " + to_string(value) + " at index " + to_string(idx));
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator>
TestLogging::test_result grow_by_insert_range_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen) {
    size_t size = urand() % 10;
    std::vector<typename TestContainer_::value_type> range;

    for (size_t i = 0; i < size; ++i)
        range.push_back(gen());

    tc.insert(range.begin(), range.end());
    vc.insert(range.begin(), range.end());
    return {"insert range of size " + to_string(size), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename Generator, typename TestContainerSizeGetter>
TestLogging::test_result grow_by_insert_range_templated(TestContainer_& tc, VerifyContainer_& vc, const Generator& gen, const TestContainerSizeGetter& size_getter) {
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
TestLogging::test_result shrink_by_clear_templated(TestContainer_& tc, VerifyContainer_& vc) {
    tc.clear();
    vc.clear();

    return {"clear", true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result shrink_by_pop_templated(TestContainer_& tc, VerifyContainer_& vc) {
    tc.pop();
    vc.pop();

    return {"pop", true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result shrink_by_pop_back_templated(TestContainer_& tc, VerifyContainer_& vc) {
    tc.pop_back();
    vc.pop_back();

    return {"pop_back", true};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result shrink_by_pop_front_templated(TestContainer_& tc, VerifyContainer_& vc) {
    tc.pop_front();
    vc.pop_front();

    return {"pop_front", true};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_iterator_no_pos_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto idx = random_index(tc, size_getter);

    auto tc_it = advance_copy(tc.begin(), idx);

    auto value = *tc_it;
    auto vc_it = vc.find(value);

    tc.erase(tc_it);
    vc.erase(vc_it);

    return {"erase at idx " + to_string(idx) + " with value: " + to_string(value), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_iterator_for_map_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto idx = random_index(tc, size_getter);

    auto tc_it = advance_copy(tc.begin(), idx);

    auto value = *tc_it;
    auto vc_it = vc.find(value.first);

    tc.erase(tc_it);
    vc.erase(vc_it);

    return {"erase at idx " + to_string(idx) + " with value: " + to_string(value.first), true};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_iterator_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto idx = random_index(tc, size_getter);

    return return_result_check(tc, vc,
                               tc.erase(advance_copy(tc.begin(), idx)),
                               vc.erase(advance_copy(vc.begin(), idx)),
                               "erase at idx " + to_string(idx));
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_value_for_map_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto idx = random_index(tc, size_getter);
    auto value = *advance_copy(tc.begin(), idx);

    return return_result_check(tc, vc,
                               tc.erase(value.first),
                               vc.erase(value.first),
                               "erase value " + to_string(value));
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_value_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
    auto idx = random_index(tc, size_getter);
    auto value = *advance_copy(tc.begin(), idx);

    return return_result_check(tc, vc,
                               tc.erase(value),
                               vc.erase(value),
                               "erase value " + to_string(value));
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result shrink_by_erase_by_range_templated(TestContainer_& tc, VerifyContainer_& vc, const TestContainerSizeGetter& size_getter) {
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
TestLogging::test_result verify_size_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter, const VerifyContainerSizeGetter& vc_size_getter) {
    if (tc_size_getter(tc) != vc_size_getter(vc))
        return {"sizes don't match: TestContainer: " + to_string(tc_size_getter(tc)) + " VerifyContainer: " + to_string(vc_size_getter(vc))};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_front_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
    if (tc_size_getter(tc) == 0) return {};
    if (tc.front() != vc.front())
        return {"fronts don't match: TestContainer: " + to_string(tc.front()) + " VerifyContainer: " + to_string(vc.front())};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_back_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
    if (tc_size_getter(tc) == 0) return {};
    if (tc.back() != vc.back())
        return {"backs don't match: TestContainer: " + to_string(tc.back()) + " VerifyContainer: " + to_string(vc.back())};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result verify_element_position_with_at_for_map_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
    for (auto& elt : vc)
        if (tc.at(elt.first) != vc.at(elt.first))
            return {"values don't match at idx " + to_string(elt.first) + " TestContainer: " + to_string(tc.at(elt.first)) + " VerifyContainer: " + to_string(vc.at(elt.first))};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_element_position_with_at_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
    for (size_t idx = 0; idx < tc_size_getter(tc); ++idx)
        if (tc.at(idx) != vc.at(idx))
            return {"values don't match at idx " + to_string(idx) + " TestContainer: " + to_string(tc.at(idx)) + " VerifyContainer: " + to_string(vc.at(idx))};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result verify_element_position_with_operator_brackets_for_map_templated(TestContainer_& tc, VerifyContainer_& vc) {
    for (auto& elt : vc)
        if (tc[elt.first] != vc[elt.first])
            return {"values don't match at idx " + to_string(elt.first) + " TestContainer: " + to_string(tc[elt.first]) + " VerifyContainer: " + to_string(vc[elt.first])};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_element_position_with_operator_brackets_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter& tc_size_getter) {
    for (size_t idx = 0; idx < tc_size_getter(tc); ++idx)
        if (tc[idx] != vc[idx])
            return {"values don't match at idx " + to_string(idx) + " TestContainer: " + to_string(tc[idx]) + " VerifyContainer: " + to_string(vc[idx])};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_>
TestLogging::test_result verify_element_position_with_iterator_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
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
TestLogging::test_result verify_front_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter&) {
    if (tc.front() != vc.front())
        return {"front values don't match TestContainer: " + to_string(tc.front()) + " VerifyContainer: " + to_string(vc.front())};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_back_element_position_templated(const TestContainer_& tc, const VerifyContainer_& vc, const TestContainerSizeGetter&) {
    if (tc.back() != vc.back())
        return {"back values don't match TestContainer: " + to_string(tc.back()) + " VerifyContainer: " + to_string(vc.back())};

    return {};
}

template <typename TestContainer_, typename VerifyContainer_, typename TestContainerSizeGetter>
TestLogging::test_result verify_find_existing_element_for_map_templated(const TestContainer_& tc, const VerifyContainer_&, const TestContainerSizeGetter& tc_size_getter) {
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
TestLogging::test_result verify_find_existing_element_templated(const TestContainer_& tc, const VerifyContainer_&, const TestContainerSizeGetter& tc_size_getter) {
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
TestLogging::test_result verify_element_inclusion_for_map_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
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
TestLogging::test_result verify_element_inclusion_templated(const TestContainer_& tc, const VerifyContainer_& vc) {
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

}  // namespace container_tester