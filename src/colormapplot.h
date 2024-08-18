#ifndef COLORMAPPLOT_H
#define COLORMAPPLOT_H

#include "qcustomplot.h"
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include <QFileDialog>

class ColorMapPlot : public QWidget
{
	Q_OBJECT
public:
	explicit ColorMapPlot(QWidget *parent = nullptr);


public slots:
	void setAxisRanges(double xLower, double xUpper, double xStepSize, double yLower, double yUpper, double yStepSize);
	void initializeData();
	void addValueToPlot(double x, double y, double value);
	void clearPlotData();
	void rescaleAndReplot();


private slots:
	void showPointToolTip(QMouseEvent *event);
	void contextMenuRequest(QPoint pos);

private:
	QCustomPlot *plot;
	QCPColorMap *colorMap;
	QCPColorScale *colorScale;
	QLabel *statusLabel;
	double xLower;
	double xUpper;
	double xStepSize;
	double yStepSize;
	double yLower;
	double yUpper;
	void setupPlot();
	void setupContextMenu();
};

#endif //COLORMAPPLOT_H
