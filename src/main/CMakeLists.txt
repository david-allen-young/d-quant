add_executable(dquant_cli
    main.cpp
    CliParser.cpp
	SingleNoteGenerator.cpp
	JsonConfigLoader.cpp
)

target_include_directories(dquant_cli PRIVATE ${PROJECT_SOURCE_DIR}/external/json)
target_include_directories(dquant_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dquant_cli
    PRIVATE
    core
    rhythmizer
    dynamizer
    intonizer
)
