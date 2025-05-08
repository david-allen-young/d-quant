add_executable(dquant_phrase_cli
    main.cpp
    PhraseJsonParser.cpp
    SongContextLoader.cpp
	PhraseRenderer.cpp
)

target_include_directories(dquant_phrase_cli PRIVATE
	${PROJECT_SOURCE_DIR}/external/json
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/phrase_cli
)

target_link_libraries(dquant_phrase_cli PRIVATE
    core rhythmizer dynamizer intonizer
)
