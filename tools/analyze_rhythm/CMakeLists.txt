add_executable(analyze_rhythm
    main.cpp
    # add AnalyzeRhythm.cpp, etc. here later
)

target_link_libraries(analyze_rhythm
    PRIVATE
    core
    rhythmizer
)
