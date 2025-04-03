#include "NoteBuilder.h"
#include "ExpressionMarks.h"
#include <algorithm>

//enum ExpressionMarksXml
//{
//	niente = 0,
//	pppp = 1,
//	ppp = 2,
//	pp = 3,
//	p = 4,
//	mp = 5,
//	mf = 6,
//	f = 7,
//	ff = 8,
//	fff = 9,
//	ffff = 10
//};

NoteBuilderXml::NoteBuilderXml()
{
	// TODO: Extend builder to establish dynamic limits (pppp/ffff vs ppp/fff vs pp/ff)
	int niente = static_cast<int>(ExpressionMark::niente);
	int ffff = static_cast<int>(ExpressionMark::ffff);
	velocityRange = { niente, ffff };
	expressionRange = { niente, ffff };
	intonationRange = { 0, 0 };
}

void NoteBuilderXml::addIntonation([[MaybeUnused]] double position, [[MaybeUnused]] int intonation)
{
	return;
}

NoteBuilderMidi::NoteBuilderMidi()
{
    // TODO: Extend builder to establish dynamic limits (pppp/ffff vs ppp/fff vs pp/ff)
    int niente = expressionMarkToCC(ExpressionMark::niente, ExpressionMark::niente, ExpressionMark::ffff);
	int ffff = expressionMarkToCC(ExpressionMark::ffff, ExpressionMark::ffff, ExpressionMark::ffff);
	velocityRange = { niente, ffff };
	expressionRange = { niente, ffff };
	intonationRange = { -8192, 8191 };
}

void NoteBuilderMidi::addIntonation(double position, int intonation)
{
	double normalizedIntonation = static_cast<double>(intonation) / static_cast<double>(intonationRange.second);
	data.intonation.push_back({ position, normalizedIntonation });
}

void NoteBuilder::setKeyNumber(int keyNumber)
{
	data.keyNumber = keyNumber;
}

void NoteBuilder::setVelocity(int velocity)
{
	data.velocity = velocity;
}

void NoteBuilder::addExpression(double position, int expression)
{
	double normalizedExpression = static_cast<double>(expression) / static_cast<double>(expressionRange.second);
	data.expression.push_back({position, normalizedExpression});
}

void NoteBuilder::setPosition(double position)
{
	data.positionInBeats = position;
}

void NoteBuilder::setDuration(double duration)
{
	data.durationInBeats = duration;
}

void NoteBuilder::setPreviousNote(NoteInterface* note)
{
	prev = note;
}

void NoteBuilder::setNextNote(NoteInterface* note)
{
	next = note;
}

std::unique_ptr<NoteInterface> NoteBuilder::build()
{
	std::sort(data.expression.begin(), data.expression.end());
	std::sort(data.intonation.begin(), data.intonation.end());
	return std::make_unique<NoteImplV1>(data, prev, next);
}