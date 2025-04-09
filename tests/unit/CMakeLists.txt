add_executable(dquant_tests
    test_core.cpp
    test_envelope_morphing.cpp
	test_csv_io.cpp
	test_notebuilder.cpp
	test_cli_parser.cpp
	test_json_config.cpp
	../../src/main/CliParser.cpp
    ../../src/main/JsonConfigLoader.cpp
)

target_link_libraries(dquant_tests
    PRIVATE
    core
    rhythmizer
    dynamizer
    intonizer
    Catch2::Catch2WithMain
)

target_include_directories(dquant_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/external/json
)

