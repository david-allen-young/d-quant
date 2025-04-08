#pragma once
#include "Note.h"
#include "../../src/dynamizer/ExpressionMarks.h"
#include <memory>

class NoteBuilderInterface
{
public:
	virtual ~NoteBuilderInterface() {};
	virtual void setKeyNumber(int keyNumber) = 0;
	virtual void setVelocity(int velocity) = 0;
	virtual void addExpression(double position, int expression) = 0;
	virtual void addIntonation(double position, int intonation) = 0;
	virtual void setPosition(double position) = 0;
	virtual void setDuration(double duration) = 0;
	virtual void setPreviousNote(NoteInterface* note) = 0;
	virtual void setNextNote(NoteInterface* note) = 0;
	virtual std::unique_ptr<NoteInterface> build() = 0;
};

class NoteBuilder : public NoteBuilderInterface
{
public:
	NoteBuilder() = default;
	~NoteBuilder() = default;
	void setKeyNumber(int keyNumber) override;
	void setVelocity(int velocity) override;
	void addExpression(double position, int expression) override;
	void addIntonation(double position, int intonation) override {};
	void setPosition(double position) override;
	void setDuration(double duration) override;
	void setPreviousNote(NoteInterface* note) override;
	void setNextNote(NoteInterface* note) override;
	std::unique_ptr<NoteInterface> build() override;
    const NoteData& getData() const;

protected:
	NoteData data;
	NoteInterface* prev = nullptr;
	NoteInterface* next = nullptr;
	std::pair<int, int> velocityRange;
	std::pair<int, int> expressionRange;
	std::pair<int, int> intonationRange;
};

using dynamizer::DynamicRangePreset;

class NoteBuilderXml : public NoteBuilder
{
public:
    NoteBuilderXml(DynamicRangePreset dynamicRangePreset);
	~NoteBuilderXml() = default;
	void addIntonation(double position, int intonation) override;
};

class NoteBuilderMidi : public NoteBuilder
{
public:
    NoteBuilderMidi();
	~NoteBuilderMidi() = default;
	void addIntonation(double position, int intonation) override;
};