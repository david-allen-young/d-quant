add_library(dynamizer
    dynamizer.cpp
	EnvelopeUtils.cpp
	ExpressionGenerator.cpp
	ExpressionMarks.cpp
)

target_include_directories(dynamizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dynamizer PUBLIC core)
