#pragma once
#include "Point.h"
#include <vector>
#include <algorithm>
#include <fstream>

double linearInterpolate(double time, const std::vector<Point>& envelope)
{
	if(envelope.empty())
	{
		return 0;
	}
	if(time <= envelope.front().time)
	{
		return envelope.front().value;
	}
	if(time >= envelope.back().time)
	{
		return envelope.back().value;
	}
	for(size_t i = 1; i < envelope.size(); i++)
	{
		if(time < envelope[i].time)
		{
			double t1 = envelope[i - 1].time;
			double t2 = envelope[i].time;
			double v1 = envelope[i - 1].value;
			double v2 = envelope[i].value;
			return v1 + (v2 - v1) * (time - t1) / (t2 - t1);
		}
	}
	return envelope.back().value;
}

std::vector<double> mergeTimelines(const std::vector<std::vector<Point>>& envelopes)
{
	std::vector<double> timeline;
	for(const auto& envelope : envelopes)
	{
		for(const auto& point : envelope)
		{
			timeline.push_back(point.time);
		}
	}
	std::sort(timeline.begin(), timeline.end());
	timeline.erase(std::unique(timeline.begin(), timeline.end()), timeline.end());
	return timeline;
}

std::vector<Point> computeMeanEnvelope(const std::vector<std::vector<Point>>& envelopes)
{
	std::vector<Point> meanEnvelope;
	std::vector<double> commonTimeline = mergeTimelines(envelopes);
	for(const auto& time : commonTimeline)
	{
		double sum = 0;
		int count = 0;
		for(const auto& envelope : envelopes)
		{
			double value = linearInterpolate(time, envelope);
			sum += value;
			count++;
		}
		meanEnvelope.push_back({ time, sum / count });
	}
	return meanEnvelope;
}

void writeCSV(const std::vector<Point>& envelope, const std::string& filename)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		return;
	}
	file << "Position,Expression\n";
	for (const auto& [time, expression] : envelope)
	{
		file << time << "," << expression << "\n";
	}
	file.close();
}

#include <sstream>
#include <string>

bool readCSV(const std::string& filename, std::vector<Point>& outEnvelope)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string timeStr, exprStr;

        if (!std::getline(ss, timeStr, ','))
        {
            continue;
        }
        if (!std::getline(ss, exprStr, ','))
        {
            continue;
        }

        double time, expr;
        std::istringstream tss(timeStr);
        std::istringstream ess(exprStr);

        if ((tss >> time) && (ess >> expr))
        {
            outEnvelope.emplace_back(time, expr);
        }
    }

    return true;
}
