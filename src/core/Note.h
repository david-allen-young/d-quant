#pragma once
#include <vector>

using Envelope = std::vector<std::pair</*position*/double, /*value*/double>>;

struct NoteData
{
	int keyNumber;
	double velocity; // normalized 0.0 to 1.0
	Envelope expression; // normalized 0.0 to 1.0
	Envelope intonation; // normalized -1.0 to +1.0
	double positionInBeats;
	double durationInBeats;

	/*Perhaps not relevant for this application...*/
	//double positionInSeconds;
	//double durationInSeconds;
};

class NoteInterface
{
public:
	virtual ~NoteInterface() {}
	virtual NoteData getNoteData() const = 0;
	virtual int getKeyNumber() const = 0;
	virtual double getVelocity() const = 0;
	virtual Envelope getExpression() const = 0;
	virtual Envelope getIntonation() const = 0;
	virtual double getPositionInBeats() const = 0;
	virtual double getDurationInBeats() const = 0;
	virtual NoteInterface* getPrev() const = 0;
	virtual NoteInterface* getNext() const = 0;
};

class NoteImplV1 : public NoteInterface
{
public:
	NoteImplV1(NoteData noteData, NoteInterface* prev, NoteInterface* next)
		: noteData(noteData), prev(prev), next(next) {}
	NoteData getNoteData() const override { return noteData; }
	int getKeyNumber() const override { return noteData.keyNumber; }
	double getVelocity() const override { return noteData.velocity; }
	Envelope getExpression() const override { return noteData.expression; }
	Envelope getIntonation() const override { return noteData.intonation; }
	double getPositionInBeats() const override { return noteData.positionInBeats; }
	double getDurationInBeats() const override { return noteData.durationInBeats; }
	NoteInterface* getPrev() const override { return prev; }
	NoteInterface* getNext() const override { return next; }
private:
	NoteData noteData;
	NoteInterface* prev;
	NoteInterface* next;
};