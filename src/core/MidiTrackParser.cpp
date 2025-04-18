#include "MidiTrackParser.h"
#include <unordered_map>

enum MidiControllers
{
    Modulation = 0x01,
    Breath = 0x02,
    Volume = 0x07,
    Pan = 0x0A,
    Expression = 0x0B,
};

enum MidiEvents
{
    NoteOff = 0x80,
    NoteOn = 0x90,
    KeyPressure = 0xA0,
    ControlChange = 0xB0,
    ProgramChange = 0xC0,
    ChannelPressure = 0xD0,
    PitchBend = 0xE0,
};

//enum MidiMetaEvents
//{
//    SequenceNumber = 0x00,
//    Text = 0x01,
//    Copyright = 0x02,
//    SequenceName = 0x03,
//    InstrumentName = 0x04,
//    Lyric = 0x05,
//    Marker = 0x06,
//    CuePoint = 0x07,
//    MidiChannelPrefix = 0x20,
//    EndOfTrack = 0x2F,
//    SetTempo = 0x51,
//    SMPTEOffset = 0x54,
//    TimeSignature = 0x58,
//    KeySignature = 0x59,
//    SequencerSpecific = 0x7F,
//};

uint32_t MidiTrackParser::readVariableLengthQuantity(const std::vector<char>& track_data, int& index)
{
    uint32_t value = 0;
    unsigned char byte;

    while (true)
    {
        if (index >= track_data.size())
        {
            // prevent out-of-bounds
            break;
        }
        byte = static_cast<unsigned char>(track_data[index++]);
        value = (value << 7) | (byte & 0x7F);
        if (!(byte & 0x80))
        {
            // MSB = 0 means "no more bytes follow"
            break;
        }
    }

    return value;
}

void MidiTrackParser::parseTrackEvents(std::vector<char>& track_data, std::vector<Point>& envelope)
{
    int i = 0;
    unsigned char runningStatus = 0;
    int tick_position = 0; // Cumulative tick count from delta times

    while (i < track_data.size())
    {
        // Step 1: Parse delta time
        uint32_t deltaTime = readVariableLengthQuantity(track_data, i);
        tick_position += deltaTime;
        if (i >= track_data.size())
            break;

        unsigned char byte = static_cast<unsigned char>(track_data[i]);

        // Step 2: Check for meta or SysEx events
        if (byte == 0xFF)
        {
            // Meta event
            ++i;
            if (i >= track_data.size())
                break;

            unsigned char metaType = static_cast<unsigned char>(track_data[i++]);
            uint32_t metaLength = readVariableLengthQuantity(track_data, i);
            i += metaLength;
            continue;
        }
        else if (byte == 0xF0 || byte == 0xF7)
        {
            // SysEx event
            ++i;
            uint32_t sysexLength = readVariableLengthQuantity(track_data, i);
            i += sysexLength;
            continue;
        }

        // Step 3: Channel voice messages
        if (byte & 0x80)
        {
            // New status byte
            runningStatus = byte;
            ++i;
        }

        // At this point, 'runningStatus' is the current status
        unsigned char eventType = runningStatus & 0xF0;
        unsigned char channel = runningStatus & 0x0F;

        if (eventType == ControlChange && i + 1 < track_data.size())
        {
            unsigned char controller = static_cast<unsigned char>(track_data[i]);
            unsigned char value = static_cast<unsigned char>(track_data[i + 1]);
            i += 2;

            double normalizedValue = (double)value / 127.0;
            double positionInBeats = (double)tick_position / 960.0;
            
            switch (controller)
            {
            case Modulation:
                std::cout << "Modulation: " << (int)value << std::endl;
                break;
            case Breath:
                std::cout << "Breath: " << (int)value << std::endl;
                std::cout << "Position: " << positionInBeats << std::endl;
                envelope.push_back(Point(positionInBeats, normalizedValue));
                break;
            case Volume:
                std::cout << "Volume: " << (int)value << std::endl;
                break;
            case Pan:
                std::cout << "Pan: " << (int)value << std::endl;
                break;
            case Expression:
                std::cout << "Expression: " << (int)value << std::endl;
                break;
            }
        }
        else
        {
            // For other channel events, we need to know how many data bytes to skip
            // Most take 2 data bytes, except ProgramChange and ChannelPressure (1 data byte)
            int dataBytes = 2;
            if ((eventType == ProgramChange) || (eventType == ChannelPressure))
            {
                dataBytes = 1;
            }

            if (i + dataBytes <= track_data.size())
            {
                i += dataBytes;
            }
            else
            {
                break; // avoid out-of-bounds
            }
        }
    }
}
//
//void MidiTrackParser::extractNoteEvents(std::vector<char>& trackData, std::vector<NoteData>& notes)
//{
//    int i = 0;
//    unsigned char runningStatus = 0;
//    int tick_position = 0; // Cumulative tick count from delta times
//
//    std::unique_ptr<NoteData> note;
//
//    while (i < trackData.size())
//    {
//        // Step 1: Parse delta time
//        uint32_t deltaTime = readVariableLengthQuantity(trackData, i);
//        tick_position += deltaTime;
//        if (i >= trackData.size())
//            break;
//
//        unsigned char byte = static_cast<unsigned char>(trackData[i]);
//
//        // Step 2: Check for meta or SysEx events
//        if (byte == 0xFF)
//        {
//            // Meta event
//            ++i;
//            if (i >= trackData.size())
//                break;
//
//            unsigned char metaType = static_cast<unsigned char>(trackData[i++]);
//            uint32_t metaLength = readVariableLengthQuantity(trackData, i);
//            i += metaLength;
//            continue;
//        }
//        else if (byte == 0xF0 || byte == 0xF7)
//        {
//            // SysEx event
//            ++i;
//            uint32_t sysexLength = readVariableLengthQuantity(trackData, i);
//            i += sysexLength;
//            continue;
//        }
//
//        // Step 3: Channel voice messages
//        if (byte & 0x80)
//        {
//            // New status byte
//            runningStatus = byte;
//            ++i;
//        }
//
//        // At this point, 'runningStatus' is the current status
//        unsigned char eventType = runningStatus & 0xF0;
//        unsigned char channel = runningStatus & 0x0F;
//
//        if (eventType == NoteOn && i + 1 < trackData.size())
//        {
//            unsigned char noteNum = static_cast<unsigned char>(trackData[i + 1]);
//            double positionInBeats = (double)tick_position / 480.0;
//            i += 2;
//            if (!note)
//            {
//                note = std::make_unique<NoteData>();
//            }
//            note->keyNumber = static_cast<int>(noteNum);
//            note->positionInBeats = positionInBeats;
//        }
//        else if (eventType == NoteOff && i + 1 < trackData.size())
//        {
//            unsigned char noteNum = static_cast<unsigned char>(trackData[i + 1]);
//            double positionInBeats = (double)tick_position / 480.0;
//            i += 2;
//            if (!note)
//            {
//                // skip orphaned NoteOff event
//                continue;
//            }
//            note->durationInBeats = positionInBeats - note->positionInBeats;
//            notes.emplace_back(*note);
//            note.reset();
//        }
//        else
//        {
//            // For other channel events, we need to know how many data bytes to skip
//            // Most take 2 data bytes, except ProgramChange and ChannelPressure (1 data byte)
//            int dataBytes = 2;
//            if ((eventType == ProgramChange) || (eventType == ChannelPressure))
//            {
//                dataBytes = 1;
//            }
//
//            if (i + dataBytes <= trackData.size())
//            {
//                i += dataBytes;
//            }
//            else
//            {
//                break; // avoid out-of-bounds
//            }
//        }
//    }
//}

void MidiTrackParser::extractNoteEvents(std::vector<char>& trackData, std::vector<NoteData>& notes)
{
    int i = 0;
    unsigned char runningStatus = 0;
    int tickPosition = 0;

    NoteData pendingNote;
    bool hasPendingNote = false;

    while (i < trackData.size())
    {
        // Step 1: Delta time
        uint32_t deltaTime = readVariableLengthQuantity(trackData, i);
        tickPosition += deltaTime;
        if (i >= trackData.size())
            break;

        unsigned char byte = static_cast<unsigned char>(trackData[i]);

        // Step 2: Meta/SysEx
        if (byte == 0xFF)
        {
            ++i;
            if (i >= trackData.size())
                break;
            uint32_t length = readVariableLengthQuantity(trackData, i);
            i += length;
            continue;
        }
        else if (byte == 0xF0 || byte == 0xF7)
        {
            ++i;
            uint32_t length = readVariableLengthQuantity(trackData, i);
            i += length;
            continue;
        }

        // Step 3: Running status
        if (byte & 0x80)
        {
            runningStatus = byte;
            ++i;
        }

        unsigned char eventType = runningStatus & 0xF0;

        // Step 4: Note On or Note Off
        if ((eventType == NoteOn || eventType == NoteOff) && i + 1 < trackData.size())
        {
            unsigned char noteNumber = static_cast<unsigned char>(trackData[i]);
            unsigned char velocity = static_cast<unsigned char>(trackData[i + 1]);
            i += 2;

            double beatTime = static_cast<double>(tickPosition) / 480.0;

            if (eventType == NoteOn && velocity > 0)
            {
                pendingNote = {};
                pendingNote.keyNumber = noteNumber;
                pendingNote.velocity = static_cast<double>(velocity) / 127.0; // Normalize to [0.0�1.0]
                pendingNote.positionInBeats = beatTime;
                hasPendingNote = true;
            }
            else if ((eventType == NoteOff || (eventType == NoteOn && velocity == 0)) && hasPendingNote)
            {
                pendingNote.durationInBeats = beatTime - pendingNote.positionInBeats;
                notes.push_back(pendingNote);
                hasPendingNote = false;
            }
        }
        else
        {
            int skip = (eventType == ProgramChange || eventType == ChannelPressure) ? 1 : 2;
            if (i + skip <= trackData.size())
                i += skip;
            else
                break;
        }
    }
}
