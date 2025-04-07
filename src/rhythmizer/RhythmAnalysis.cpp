#include "RhythmAnalysis.h"
#include <fstream>
#include <iomanip>


namespace rhythmizer {
//-------------------------------------------------------------------------------------------

//void writeRhythmDeviationCSV(const std::vector<NoteData>& notes, const std::string& filename)
//{
//    std::ofstream out(filename);
//    if (!out.is_open())
//        return;
//
//    out << "Position,DurationRatio,Velocity\n";
//    for (const auto& note : notes)
//    {
//        out << std::fixed << std::setprecision(5)
//            << note.positionInBeats << ","
//            << note.durationInBeats / 1.0 << ","
//            << note.velocity << "\n";
//    }
//    out.close();
//}

void writeRhythmDeviationCSV(const std::vector<NoteData>& notes, const std::string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
        return;

    const double firstNominalPos = notes.front().positionInBeats;
    const double nominalDuration = 1.0;
    const double nominalVelocity = 60.0 / 127.0;

    out << "NominalPosition,DeltaPosition,DurationRatio,VelocityDelta\n";

    for (size_t i = 0; i < notes.size(); ++i)
    {
        double nominalPosition = firstNominalPos + static_cast<double>(i);
        double deltaPos = notes[i].positionInBeats - nominalPosition;
        double durationRatio = notes[i].durationInBeats / nominalDuration;
        double velocityDelta = notes[i].velocity - nominalVelocity;

        out << std::fixed << std::setprecision(5)
            << nominalPosition << ","
            << deltaPos << ","
            << durationRatio << ","
            << velocityDelta << "\n";
    }

    out.close();
}



//-------------------------------------------------------------------------------------------
}//namespace rhythmizer
