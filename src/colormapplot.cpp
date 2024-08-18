#include "colormapplot.h"

ColorMapPlot::ColorMapPlot(QWidget *parent) : QWidget(parent)
{
	this->plot = new QCustomPlot();
	this->plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
	this->plot->axisRect()->setupFullAxesBox(true);


	this->plot->xAxis->setLabel("x");
	this->plot->yAxis->setLabel("y");



	//make sure the axis rect and color scale synchronize their bottom and top margins (so they line up)
	QCPMarginGroup *marginGroup = new QCPMarginGroup(this->plot);
	this->plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);


	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(this->plot);
	this->plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->statusLabel = new QLabel("Hover over points to see values");
	layout->addWidget(this->statusLabel);
	this->setLayout(layout);

	this->setupPlot();
	this->setupContextMenu();

	connect(this->plot, &QCustomPlot::mouseMove, this, &ColorMapPlot::showPointToolTip);
	connect(plot, &QCustomPlot::mouseDoubleClick, this, &ColorMapPlot::rescaleAndReplot);


	//default values:
	this->setAxisRanges(-300, 300, 1.0, -300, 300, 1.0);
}

void ColorMapPlot::setupPlot() {
	this->colorMap = new QCPColorMap(this->plot->xAxis, this->plot->yAxis);
	this->colorScale = new QCPColorScale(this->plot);
	this->plot->plotLayout()->addElement(0, 1, this->colorScale);

	this->colorMap->setInterpolate(false);
	this->colorMap->setDataRange(QCPRange(0, 255));
	
	this->plot->xAxis->setLabel("d2");
	this->plot->yAxis->setLabel("d3");
	this->colorScale->setLabel("Value");
	
	this->colorMap->setColorScale(this->colorScale);
	this->colorMap->setGradient(QCPColorGradient::gpPolar);

//	this->plot->setBackground( QColor(50, 50, 50));
//	this->plot->axisRect()->setBackground(QColor(55, 55, 55));
//	this->plot->plotLayout()->setMargins(QMargins(0,0,0,0));

	this->plot->rescaleAxes();
	this->plot->replot();
}

void ColorMapPlot::setupContextMenu() {
	this->plot->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this->plot, &QCustomPlot::customContextMenuRequested, this, &ColorMapPlot::contextMenuRequest);
}

void ColorMapPlot::setAxisRanges(double xLower, double xUpper, double xStepSize, double yLower, double yUpper, double yStepSize) {
	this->xLower = xLower;
	this->xUpper = xUpper;
	this->xStepSize = xStepSize;
	this->yLower = yLower;
	this->yUpper = yUpper;
	this->yStepSize = yStepSize;
	int nx = static_cast<int>((xUpper - xLower) / xStepSize + 0.5);
	int ny = static_cast<int>((yUpper - yLower) / yStepSize + 0.5);
	this->colorMap->data()->setSize(nx, ny); //number of data points
	this->colorMap->data()->setRange(QCPRange(xLower, xUpper), QCPRange(yLower, yUpper)); //axis range
	this->colorMap->data()->fill(0);
	this->plot->rescaleAxes(true);
	this->plot->replot();
}

void ColorMapPlot::initializeData() {
	int nx = colorMap->data()->keySize();
	int ny = colorMap->data()->valueSize();
	double x, y, z;
	for (int xIndex = 0; xIndex < nx; ++xIndex) {
		for (int yIndex = 0; yIndex < ny; ++yIndex) {
			colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
			z = sin(x)+sin(y);
			colorMap->data()->setCell(xIndex, yIndex, z);
		}
	}
	this->plot->rescaleAxes();
	this->plot->replot();
}


void ColorMapPlot::addValueToPlot(double x, double y, double value) {
	this->colorMap->data()->setData(x, y, value);
	this->colorMap->rescaleDataRange(true);
	this->plot->replot();
}

//void ColorMapPlot::rescaleAndReplot() {
//	this->colorMap->rescaleDataRange(true);
//	this->plot->replot();
//}

void ColorMapPlot::clearPlotData() {
	this->colorMap->data()->clear();
	this->rescaleAndReplot();
}

void ColorMapPlot::showPointToolTip(QMouseEvent *event) {
	double xKey = this->plot->xAxis->pixelToCoord(event->pos().x());
	double yKey = this->plot->yAxis->pixelToCoord(event->pos().y());
	int xIndex = 0;
	int yIndex = 0;
	this->colorMap->data()->coordToCell(xKey, yKey, &xIndex, &yIndex);
	double value = this->colorMap->data()->cell(xIndex, yIndex);
	this->statusLabel->setText(QString("d2: %1, d3: %2, Value: %3").arg(xKey).arg(yKey).arg(value));
}

void ColorMapPlot::contextMenuRequest(QPoint pos) {
	QMenu *menu = new QMenu(this);
	menu->addAction("Save as PNG", this, [&](){
		QString fileName = QFileDialog::getSaveFileName(this, "Save Plot", "", "PNG Image (*.png)");
		if (!fileName.isEmpty())
			this->plot->savePng(fileName);
	});
	menu->addAction("Save as PDF", this, [&](){
		QString fileName = QFileDialog::getSaveFileName(this, "Save Plot", "", "PDF Document (*.pdf)");
		if (!fileName.isEmpty())
			this->plot->savePdf(fileName);
	});
	menu->popup(this->plot->mapToGlobal(pos));
}

void ColorMapPlot::rescaleAndReplot() {
	int nx = static_cast<int>((this->xUpper - this->xLower) / this->xStepSize + 0.5);
	int ny = static_cast<int>((this->yUpper - this->yLower) / this->yStepSize + 0.5);
	this->colorMap->data()->setSize(nx, ny); //number of data points
	this->colorMap->data()->setRange(QCPRange(this->xLower, this->xUpper), QCPRange(this->yLower, this->yUpper)); //axis range
	this->plot->rescaleAxes(true);
	this->colorMap->rescaleDataRange(true);
	this->plot->replot();
}
