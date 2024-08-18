#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <QObject>
#include <QVector>
#include <QRect>
#include <QApplication>
#include <QtMath>
#include "dispersionvaluesmapparameters.h"


class FeatureExtractor : public QObject
{
	Q_OBJECT
public:
	explicit FeatureExtractor(QObject *parent = nullptr);

private:
	bool isFeatureExtracting;
	IMAGE_DISPERSION_FEATURE selectedFeature;
	QRect roi;

	QPoint indexToPoint(int index, int width);
	qreal findMaxValue(const QVector<qreal>& line);
	QRect clampRoi(QRect roi, unsigned int samplesPerLine, unsigned int linesPerFrame);
	template <typename T> QVector<qreal> calculateAveragedLine(QRect roi, T* frame, unsigned int samplesPerLine, unsigned int linesPerFrame);


signals:
	void averagedLineCalculated(QVector<qreal>);
	void featureExtracted(qreal);
	void info(QString);
	void error(QString);

public slots:
	void extractFeature(void* frameBuffer, unsigned int bitDepth, unsigned int samplesPerLine, unsigned int linesPerFrame);
	void setRoi(QRect roi);
	void setFeature(int featureOption);
};

#endif //FEATUREEXTRACTOR_H
