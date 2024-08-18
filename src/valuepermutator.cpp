#include "ValuePermutator.h"
#include <limits>

ValuePermutator::ValuePermutator(QObject *parent) : QObject(parent),
	d2Start(-300), d2End(300), d2Step(1),
	d3Start(-300), d3End(300), d3Step(1),
	currentD2(0), currentD3(0),
	d2AtMax(0), d3AtMax(0), maxValue(std::numeric_limits<double>::lowest()),
	d2AtMin(0), d3AtMin(0), minValue(std::numeric_limits<double>::max()),
	isPermutationActive(false), isFirstResult(true)
{
}

void ValuePermutator::setD2Range(double start, double end, double step) {
	this->d2Start = start;
	this->d2End = end;
	this->d2Step = step;
}

void ValuePermutator::setD3Range(double start, double end, double step) {
	this->d3Start = start;
	this->d3End = end;
	this->d3Step = step;
}

void ValuePermutator::startPermutation() {
	this->currentD2 = d2Start;
	this->currentD3 = d3Start;
	this->isPermutationActive = true;
	emit permutationChanged(this->currentD2, this->currentD3);
}

void ValuePermutator::cancelPermutation() {
	this->isPermutationActive = false;
	this->maxValue = std::numeric_limits<double>::lowest();
	this->minValue = std::numeric_limits<double>::max();
}

void ValuePermutator::setD2RangeStart(double start) {
	this->d2Start = start;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::setD2RangeEnd(double end) {
	this->d2End = end;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::setD2StepSize(double step) {
	this->d2Step = step;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::setD3RangeStart(double start) {
	this->d3Start = start;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::setD3RangeEnd(double end) {
	this->d3End = end;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::setD3StepSize(double step) {
	this->d3Step = step;
	emit d2d3RangesChanged(this->d2Start, this->d2End, this->d2Step, this->d3Start, this->d3End, this->d3Step);
}

void ValuePermutator::handleResult(double result) {
	if (!this->isPermutationActive) { //check if the permutation was canceled.
		return;
	}

	if (this->isFirstResult) {
		//discard the first result of each new permutation to ensure that previous modification of dispersion values was applied
		this->isFirstResult = false; //reset the flag after discarding
		return;
	}

	emit resultReady(this->currentD2, this->currentD3, result);

	if(result < this->minValue){
		this->minValue = result;
		this->d2AtMin = this->currentD2;
		this->d3AtMin = this->currentD3;
		emit minimumFound(this->d2AtMin, this->d3AtMin, this->minValue);
	}

	if(result > this->maxValue){
		this->maxValue = result;
		this->d2AtMax = this->currentD2;
		this->d3AtMax = this->currentD3;
		emit maximumFound(this->d2AtMax, this->d3AtMax, this->maxValue);
	}

	this->nextPermutation();
}

void ValuePermutator::nextPermutation() {
	if (!this->isPermutationActive) {
		return;
	}

	this->currentD3 += this->d3Step;
	if(this->currentD3 > this->d3End) {
		this->currentD3 = this->d3Start;
		this->currentD2 += this->d2Step;
		if(this->currentD2 > this->d2End) {
			this->isPermutationActive = false; //stop permutation if the end is reached
			this->maxValue = std::numeric_limits<double>::lowest();
			this->minValue = std::numeric_limits<double>::max();
			return;
		}
	}

	this->isFirstResult = true; //reset the first result flag for the new permutation
	emit permutationChanged(this->currentD2, this->currentD3);
}
