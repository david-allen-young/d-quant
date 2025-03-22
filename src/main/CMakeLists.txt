add_executable(dquant_cli
    main.cpp
    # Add other CLI-specific files if needed
)

target_include_directories(dquant_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dquant_cli
    PRIVATE
    core
    rhythmizer
    dynamizer
    intonizer
)
