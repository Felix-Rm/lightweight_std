#define LWSTD_TEST_ACCELERATE(tester_type, container_type, ...) \
    template TestLogging::test_result tester_type::run_templated<container_tester::ContainerTester<lw_std::container_type<__VA_ARGS__>, std::container_type<__VA_ARGS__>>>(container_tester::ContainerTester<lw_std::container_type<__VA_ARGS__>, std::container_type<__VA_ARGS__>>&, size_t)
