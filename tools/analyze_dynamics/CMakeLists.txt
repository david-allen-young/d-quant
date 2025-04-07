add_executable(analyze_dynamics
    main.cpp
    # add AnalyzeDynamics.cpp, etc. here later
)

target_link_libraries(analyze_dynamics
    PRIVATE
    core
    dynamizer
)
