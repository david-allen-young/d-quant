add_executable(analyze_rhythm
    main.cpp
)

target_link_libraries(analyze_rhythm
    PRIVATE
    core
    rhythmizer
)

target_include_directories(analyze_rhythm
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/rhythmizer
)
