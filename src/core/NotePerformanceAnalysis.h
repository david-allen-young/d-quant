#pragma once
#include <utility>

struct PerformanceData
{
	double velocityDelta;
	double positionDelta;
	double durationDelta;
};

struct PerformanceContext
{
	bool isFirstNote;
	bool isLastNote;
	bool isDownbeat;

	bool isMarcato; // Accent 1
	bool isSforzato; // Accent 2

	std::pair<bool,bool> isSlurred; // Articulation 1
	bool isTenuto; // Articulation 2
	bool isStaccato; // Articulation 3
	bool isStaccatissimo; // Articulation 4
};