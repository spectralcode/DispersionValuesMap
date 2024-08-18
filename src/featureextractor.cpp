#include "featureextractor.h"
#include <QtMath>


FeatureExtractor::FeatureExtractor(QObject *parent)
	: QObject(parent),
	isFeatureExtracting(false),
	selectedFeature(IMAGE_DISPERSION_FEATURE::MAXVALUE)
{
	this->roi.setRect(0, 0, 1024, 1024);
}

QPoint FeatureExtractor::indexToPoint(int index, int width) {
	int x = index%width;
	int y = index/width;
	return QPoint(x, y);
}

void FeatureExtractor::setRoi(QRect roi) {
	this->roi = roi;
}

void FeatureExtractor::setFeature(int featureOption) {
	this->selectedFeature =static_cast<IMAGE_DISPERSION_FEATURE>(featureOption);
}

void FeatureExtractor::extractFeature(void* frameBuffer, unsigned int bitDepth, unsigned int samplesPerLine, unsigned int linesPerFrame) {
	if (!this->isFeatureExtracting) {
		this->isFeatureExtracting = true;
		qreal featureValue = 0;
		QVector<qreal> averagedLine;

		//calculate averaged A-scan
		if (bitDepth <= 8) {
			// Handling 8-bit data
			unsigned char* frame = static_cast<unsigned char*>(frameBuffer);
			averagedLine = this->calculateAveragedLine<unsigned char>(this->roi, frame, samplesPerLine, linesPerFrame);
		} else if (bitDepth > 8 && bitDepth <= 16) {
			// Handling 16-bit data
			unsigned short* frame = static_cast<unsigned short*>(frameBuffer);
			averagedLine = this->calculateAveragedLine<unsigned short>(this->roi, frame, samplesPerLine, linesPerFrame);
		} else if (bitDepth > 16 && bitDepth <= 32) {
			// Handling 32-bit data
			unsigned long* frame = static_cast<unsigned long*>(frameBuffer);
			averagedLine = this->calculateAveragedLine<unsigned long>(this->roi, frame, samplesPerLine, linesPerFrame);
		}
		emit averagedLineCalculated(averagedLine);

		//extract selected feature from averaged A-scan
		switch (this->selectedFeature) {
			case MAXVALUE:
				featureValue = this->findMaxValue(averagedLine);
				break;
			//todo: additional cases for other features
		}

		emit featureExtracted(featureValue);
		this->isFeatureExtracting = false;
	}
}

//todo: compare this findMaxValue with custom implementation below
//qreal FeatureExtractor::findMaxValue(const QVector<qreal>& line) {
//	return *std::max_element(line.begin(), line.end());
//}

qreal FeatureExtractor::findMaxValue(const QVector<qreal>& line) {
	if (line.isEmpty()) {
		return 0;
	}
	qreal max = line[0];
	for (int i = 1; i < line.size(); ++i) {
		if (line[i] > max) {
			max = line[i];
		}
	}
	return max;
}


QRect FeatureExtractor::clampRoi(QRect roi, unsigned int samplesPerLine, unsigned int linesPerFrame) {
	QRect clampedRoi(0, 0, 0, 0);
	QRect normalizedRoi = roi.normalized();

	int roiX = normalizedRoi.x();
	int roiWidth = normalizedRoi.width();
	int roiY = normalizedRoi.y();
	int roiHeight = normalizedRoi.height();

	int frameWidth = static_cast<int>(samplesPerLine);
	int frameHeight = static_cast<int>(linesPerFrame);

	//check if roi is fully outside of frame and return zero sized roi
	if(roiX >= frameWidth || roiY >= frameHeight || (roiX + roiWidth) < 0 || (roiY + roiHeight) < 0){
		return clampedRoi;
	}

	//clamp roi to ensure it is fully within the frame
	int endX = (qMin(roiX + roiWidth, frameWidth));
	int endY = (qMin(roiY + roiHeight, frameHeight));
	int clampedX = qMax(0, roiX);
	int clampedY = qMax(0, roiY);
	int clampedWidth = qMin(endX-clampedX, frameWidth);
	int clampedHeight = qMin(endY-clampedY, frameHeight);
	clampedRoi.setX(clampedX);
	clampedRoi.setY(clampedY);
	clampedRoi.setWidth(clampedWidth);
	clampedRoi.setHeight(clampedHeight);

	return clampedRoi;
}


template<typename T>
QVector<qreal> FeatureExtractor::calculateAveragedLine(QRect roi, T* frame, unsigned int samplesPerLine, unsigned int linesPerFrame) {
	QVector<qreal> averagedLine(samplesPerLine, 0);
	QRect clampedRoi = this->clampRoi(roi, samplesPerLine, linesPerFrame);
	int roiY = clampedRoi.y();
	int roiHeight = clampedRoi.height();
	int roiX = clampedRoi.x();
	int roiWidth = clampedRoi.width();

	//if roi is out of the frame clampedRoi(..) will return a QRect with 0 width and 0 height
	if (roiWidth <= 0 || roiHeight <= 0) {

		return averagedLine;
	}

	//loop through ROI and sum up the values
	int endY = roiY + roiHeight;
	int endX = roiX + roiWidth;
	QVector<qreal> sumLine(roiWidth, 0);


	for (int y = roiY; y < endY; ++y) {
		for (int x = roiX; x < endX; ++x) {
			sumLine[x - roiX] += frame[y * samplesPerLine + x];
		}
	}

	//compute average per column in ROI
	for (int i = 0; i < roiWidth; ++i) {
		averagedLine[roiX + i] = sumLine[i] / roiHeight;
	}

	return averagedLine;
}
