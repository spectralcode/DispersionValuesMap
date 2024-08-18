#ifndef VALUEPERMUTATOR_H
#define VALUEPERMUTATOR_H

#include <QObject>

class ValuePermutator : public QObject
{
	Q_OBJECT

public:
	explicit ValuePermutator(QObject *parent = nullptr);
	void setD2Range(double start, double end, double step);
	void setD3Range(double start, double end, double step);

signals:
	void permutationChanged(double d2, double d3);
	void resultReady(double d2, double d3, double result);
	void minimumFound(double d2, double d3, double minimum);
	void maximumFound(double d2, double d3, double maximum);
	void d2d3RangesChanged(double d2Start, double d2End,double d2Step, double d3Start, double d3End, double d3Step);

public slots:
	void startPermutation();
	void cancelPermutation();
	void setD2RangeStart(double start);
	void setD2RangeEnd(double end);
	void setD2StepSize(double step);
	void setD3RangeStart(double start);
	void setD3RangeEnd(double end);
	void setD3StepSize(double step);
	void handleResult(double result);

private:
	double d2Start, d2End, d2Step;
	double d3Start, d3End, d3Step;
	double currentD2, currentD3;
	double d2AtMin, d3AtMin, minValue;
	double d2AtMax, d3AtMax, maxValue;
	bool isPermutationActive;
	bool isFirstResult;
	void nextPermutation();
};

#endif // VALUEPERMUTATOR_H
