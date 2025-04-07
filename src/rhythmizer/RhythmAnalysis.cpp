#include "RhythmAnalysis.h"
#include <fstream>
#include <iomanip>


namespace rhythmizer {
//-------------------------------------------------------------------------------------------

void writeRhythmDeviationCSV(const std::vector<NoteData>& notes, const std::string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
        return;

    out << "Position,DurationRatio,Velocity\n";
    for (const auto& note : notes)
    {
        out << std::fixed << std::setprecision(5)
            << note.positionInBeats << ","
            << note.durationInBeats / 1.0 << ","
            << note.velocity << "\n";
    }
    out.close();
}

//-------------------------------------------------------------------------------------------
}//namespace rhythmizer
