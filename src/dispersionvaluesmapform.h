#ifndef DISPERSIONVALUESMAPFORM_H
#define DISPERSIONVALUESMAPFORM_H

#include <QWidget>
#include <QRect>
#include "dispersionvaluesmapparameters.h"
#include "lineplot.h"
#include "imagedisplay.h"
#include "colormapplot.h"

namespace Ui {
class DispersionValuesMapForm;
}

class DispersionValuesMapForm : public QWidget
{
	Q_OBJECT

public:
	explicit DispersionValuesMapForm(QWidget *parent = 0);
	~DispersionValuesMapForm();

	void setSettings(QVariantMap settings);
	void getSettings(QVariantMap* settings);

	ImageDisplay* getImageDisplay(){return this->imageDisplay;}
	LinePlot* getLinePlot(){return this->linePlot;}
	ColorMapPlot* getColorMapPlot(){return this->colorMapPlot;}

	Ui::DispersionValuesMapForm* ui;

protected:


public slots:
	void setMaximumFrameNr(int maximum);
	void setMaximumBufferNr(int maximum);
	void plotLine(QVector<qreal> line);
	void addValueToMapPlot(qreal d2, qreal d3, qreal value);
	void displayMinValue(qreal d2, qreal d3, qreal minimum);
	void displayMaxValue(qreal d2, qreal d3, qreal maximum);
	void setColorMapAxisRange(double d2Start, double d2End, double d2Step, double d3Start, double d3End, double d3Step);

private:
	ImageDisplay* imageDisplay;
	LinePlot* linePlot;
	ColorMapPlot* colorMapPlot;

	DispersionValuesMapParameters parameters;

signals:
	void paramsChanged();
	void frameNrChanged(int);
	void bufferNrChanged(int);
	void featureChanged(int);
	void bufferSourceChanged(BUFFER_SOURCE);
	void roiChanged(QRect);
	void d2StartChanged(double);
	void d2EndChanged(double);
	void d2StepSizeChanged(double);
	void d3StartChanged(double);
	void d3EndChanged(double);
	void d3StepSizeChanged(double);
	void startPermutationsRequested();
	void cancelPermutationsRequested();
	void info(QString);
	void error(QString);
};

#endif //DISPERSIONVALUESMAPFORM_H
