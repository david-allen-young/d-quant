add_executable(dquant_tests
    test_core.cpp
    # Add other test files here
)

target_link_libraries(dquant_tests
    PRIVATE
    core
    rhythmizer
    dynamizer
    intonizer
    Catch2::Catch2WithMain
)

target_include_directories(dquant_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
