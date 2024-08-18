#ifndef DISPERSIONVALUESMAPPARAMETERS_H
#define DISPERSIONVALUESMAPPARAMETERS_H

#include <QString>
#include <QtGlobal>
#include <QMetaType>
#include <QRect>

#define DISPERSIONVALUESMAP_SOURCE "image_source"
#define DISPERSIONVALUESMAP_FEATURE "feature"
#define DISPERSIONVALUESMAP_FRAME "frame_number"
#define DISPERSIONVALUESMAP_BUFFER "buffer_number"
#define DISPERSIONVALUESMAP_ROI_X "roi_x"
#define DISPERSIONVALUESMAP_ROI_Y "roi_y"
#define DISPERSIONVALUESMAP_ROI_WIDTH "roi_width"
#define DISPERSIONVALUESMAP_ROI_HEIGHT "roi_height"
#define DISPERSIONVALUESMAP_D2_START "d2_start"
#define DISPERSIONVALUESMAP_D2_END "d2_end"
#define DISPERSIONVALUESMAP_D2_STEP_SIZE "d2_step_size"
#define DISPERSIONVALUESMAP_D3_START "d3_start"
#define DISPERSIONVALUESMAP_D3_END "d3_end"
#define DISPERSIONVALUESMAP_D3_STEP_SIZE "d3_step_size"


enum BUFFER_SOURCE{
	RAW,
	PROCESSED
};

enum IMAGE_DISPERSION_FEATURE{
	MAXVALUE
};

struct DispersionValuesMapParameters {
	BUFFER_SOURCE bufferSource;
	IMAGE_DISPERSION_FEATURE imageFeature;
	QRect roi;
	int frameNr;
	int bufferNr;
	double d2Start;
	double d2End;
	double d2StepSize;
	double d3Start;
	double d3End;
	double d3StepSize;
};
Q_DECLARE_METATYPE(DispersionValuesMapParameters)


#endif //DISPERSIONVALUESMAPPARAMETERS_H


