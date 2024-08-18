#include "dispersionvaluesmapform.h"
#include "ui_dispersionvaluesmapform.h"

DispersionValuesMapForm::DispersionValuesMapForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DispersionValuesMapForm) {
	ui->setupUi(this);

	this->imageDisplay = this->ui->widget_imageDisplay;
	connect(this->imageDisplay, &ImageDisplay::info, this, &DispersionValuesMapForm::info);
	connect(this->imageDisplay, &ImageDisplay::error, this, &DispersionValuesMapForm::error);
	connect(this->imageDisplay, QOverload<QRect>::of(&ImageDisplay::roiChanged),this, [this](QRect roiRect) {
		this->parameters.roi = roiRect;
		emit roiChanged(roiRect);
		emit paramsChanged();
	});

	this->linePlot = this->ui->widget_linePlot;
	connect(this->linePlot, &LinePlot::info, this, &DispersionValuesMapForm::info);
	connect(this->linePlot, &LinePlot::error, this, &DispersionValuesMapForm::error);

	this->colorMapPlot = this->ui->widget_mapPlot;


///// --- For future use
	//ComboBox Image Feature
//	QStringList featureOptions = {"Max. Value"};
//	this->ui->comboBox_imageFeature->addItems(featureOptions);
//	connect(this->ui->comboBox_imageFeature, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
//		this->parameters.imagefeature = static_cast<IMAGE_DISPERSION_FEATURE>(index);
//		emit featureChanged(index);
//		emit paramsChanged();
//	});
///// ------------------------

	//SpinBox Buffer
	this->ui->spinBox_buffer->setMaximum(2);
	this->ui->spinBox_buffer->setMinimum(-1);
	this->ui->spinBox_buffer->setSpecialValueText(tr("All"));
	connect(this->ui->spinBox_buffer, QOverload<int>::of(&QSpinBox::valueChanged),[this](int bufferNr) {
		this->parameters.bufferNr = bufferNr;
		emit bufferNrChanged(bufferNr);
		emit paramsChanged();
	});

	//Frame slider and spinBox
	connect(this->ui->horizontalSlider_frame, &QSlider::valueChanged, this->ui->spinBox_frame, &QSpinBox::setValue);
	connect(this->ui->spinBox_frame, QOverload<int>::of(&QSpinBox::valueChanged), this->ui->horizontalSlider_frame, &QSlider::setValue);
	connect(this->ui->horizontalSlider_frame, &QSlider::valueChanged, [this](int frameNr) { // Der `int`-Parameter wird hier ignoriert
		this->parameters.frameNr = frameNr;
		emit frameNrChanged(frameNr);
		emit paramsChanged();
	});
	this->setMaximumFrameNr(512);	

	//start, end, step size values for d2 and d3
	// Connect d2Start SpinBox
	connect(this->ui->doubleSpinBox_d2Start, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d2Start = value;
			emit d2StartChanged(value);
			emit paramsChanged();
		});

	// Connect d2End SpinBox
	connect(this->ui->doubleSpinBox_d2End, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d2End = value;
			emit d2EndChanged(value);
			emit paramsChanged();
		});

	// Connect d2StepSize SpinBox
	connect(this->ui->doubleSpinBox_d2StepSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d2StepSize = value;
			emit d2StepSizeChanged(value);
			emit paramsChanged();
		});

	// Connect d3Start SpinBox
	connect(this->ui->doubleSpinBox_d3Start, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d3Start = value;
			emit d3StartChanged(value);
			emit paramsChanged();
		});

	// Connect d3End SpinBox
	connect(this->ui->doubleSpinBox_d3End, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d3End = value;
			emit d3EndChanged(value);
			emit paramsChanged();
		});

	// Connect d3StepSize SpinBox
	connect(this->ui->doubleSpinBox_d3StepSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
		[this](double value) {
			this->parameters.d3StepSize = value;
			emit d3StepSizeChanged(value);
			emit paramsChanged();
		});

	//start, cancel buttons
	connect(this->ui->pushButton_start, &QPushButton::clicked, [this]() {
		emit info("Permutation of dispersion values started!");
		this->colorMapPlot->clearPlotData();
		emit this->startPermutationsRequested();
	});

	// Connect the Cancel Permutations button
	connect(this->ui->pushButton_cancel, &QPushButton::clicked, [this]() {
		emit info("Permutation of dispersion values canceled!");
		emit this->cancelPermutationsRequested();
	});

	//default values
	this->parameters.bufferNr= -1;
	this->parameters.bufferSource = PROCESSED;
	this->parameters.frameNr = 0;
	this->parameters.imageFeature = MAXVALUE;
	this->parameters.roi = QRect(50,50, 400, 800);
	this->parameters.d2Start = -10.0;
	this->parameters.d2End = 10.0;
	this->parameters.d2StepSize = 0.5;
	this->parameters.d3Start = -10.0;
	this->parameters.d3End = 10.0;
	this->parameters.d3StepSize = 0.5;
}

DispersionValuesMapForm::~DispersionValuesMapForm() {
	delete ui;
}

void DispersionValuesMapForm::setSettings(QVariantMap settings) {
	// Update parameters struct
	if (!settings.isEmpty()) {
		this->parameters.bufferNr = settings.value(DISPERSIONVALUESMAP_BUFFER).toInt();
		this->parameters.bufferSource = static_cast<BUFFER_SOURCE>(settings.value(DISPERSIONVALUESMAP_SOURCE).toInt());
		this->parameters.imageFeature = static_cast<IMAGE_DISPERSION_FEATURE>(settings.value(DISPERSIONVALUESMAP_FEATURE).toInt());
		this->parameters.frameNr = settings.value(DISPERSIONVALUESMAP_FRAME).toInt();
		int roiX = settings.value(DISPERSIONVALUESMAP_ROI_X).toInt();
		int roiY = settings.value(DISPERSIONVALUESMAP_ROI_Y).toInt();
		int roiWidth = settings.value(DISPERSIONVALUESMAP_ROI_WIDTH).toInt();
		int roiHeight = settings.value(DISPERSIONVALUESMAP_ROI_HEIGHT).toInt();
		this->parameters.roi = QRect(roiX, roiY, roiWidth, roiHeight);
	}

	// Update GUI elements
	this->ui->spinBox_buffer->setValue(this->parameters.bufferNr);
	//this->ui->comboBox_imageFeature->setCurrentIndex(static_cast<int>(this->parameters.imageFeature));
	this->ui->horizontalSlider_frame->setValue(this->parameters.frameNr);
	this->ui->widget_imageDisplay->setRoi(this->parameters.roi);
}

void DispersionValuesMapForm::getSettings(QVariantMap* settings) {
	if (!settings) {
		return;
	}

	settings->insert(DISPERSIONVALUESMAP_BUFFER, this->parameters.bufferNr);
	settings->insert(DISPERSIONVALUESMAP_SOURCE, static_cast<int>(this->parameters.bufferSource));
	settings->insert(DISPERSIONVALUESMAP_FEATURE, static_cast<int>(this->parameters.imageFeature));
	settings->insert(DISPERSIONVALUESMAP_FRAME, this->parameters.frameNr);
	settings->insert(DISPERSIONVALUESMAP_ROI_X, this->parameters.roi.x());
	settings->insert(DISPERSIONVALUESMAP_ROI_Y, this->parameters.roi.y());
	settings->insert(DISPERSIONVALUESMAP_ROI_WIDTH, this->parameters.roi.width());
	settings->insert(DISPERSIONVALUESMAP_ROI_HEIGHT, this->parameters.roi.height());
}

void DispersionValuesMapForm::setMaximumFrameNr(int maximum) {
	this->ui->horizontalSlider_frame->setMaximum(maximum);
	this->ui->spinBox_frame->setMaximum(maximum);
}

void DispersionValuesMapForm::setMaximumBufferNr(int maximum) {
	this->ui->spinBox_buffer->setMaximum(maximum);
}

void DispersionValuesMapForm::plotLine(QVector<qreal> line) {
	this->linePlot->plotLine(line);
}

void DispersionValuesMapForm::addValueToMapPlot(qreal d2, qreal d3, qreal value) {
	this->colorMapPlot->addValueToPlot(d2, d3, value);
}

void DispersionValuesMapForm::displayMinValue(qreal d2, qreal d3, qreal minimum) {
	this->ui->lineEdit_resultMinD2->setText(QString::number(d2));
	this->ui->lineEdit_resultMinD3->setText(QString::number(d3));
	this->ui->lineEdit_resultMinValue->setText(QString::number(minimum));
}

void DispersionValuesMapForm::displayMaxValue(qreal d2, qreal d3, qreal maximum) {
	this->ui->lineEdit_resultMaxD2->setText(QString::number(d2));
	this->ui->lineEdit_resultMaxD3->setText(QString::number(d3));
	this->ui->lineEdit_resultMaxValue->setText(QString::number(maximum));
}

void DispersionValuesMapForm::setColorMapAxisRange(double d2Start, double d2End, double d2Step, double d3Start, double d3End, double d3Step) {
	this->colorMapPlot->setAxisRanges(d2Start, d2End, d2Step, d3Start, d3End, d3Step);
}
