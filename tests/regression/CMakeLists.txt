# tests/regression/CMakeLists.txt

add_executable(dquant_regression_tests
    test_midi_to_envelope_csv.cpp
    test_morph2_generation.cpp
    test_breath_cc_generation.cpp
    test_expressive_note.cpp
    test_rhythmizer_csv.cpp
    ../../src/main/CliParser.cpp
    ../../src/main/JsonConfigLoader.cpp
)

target_link_libraries(dquant_regression_tests
    PRIVATE
    core
    rhythmizer
    dynamizer
    intonizer
    Catch2::Catch2WithMain
)

target_include_directories(dquant_regression_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/json
)
