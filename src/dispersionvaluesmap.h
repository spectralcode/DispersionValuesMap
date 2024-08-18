#ifndef DISPERSIONVALUESMAPEXTENSION_H
#define DISPERSIONVALUESMAPEXTENSION_H


#include <QCoreApplication>
#include <QThread>
#include "octproz_devkit.h"
#include "dispersionvaluesmapform.h"
#include "featureextractor.h"
#include "valuepermutator.h"

#define NUMBER_OF_BUFFERS 2


class DispersionValuesMap : public Extension
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID Extension_iid)
	Q_INTERFACES(Extension)
	QThread featureExtractorThread;

public:
	DispersionValuesMap();
	~DispersionValuesMap();

	virtual QWidget* getWidget() override;
	virtual void activateExtension() override;
	virtual void deactivateExtension() override;
	virtual void settingsLoaded(QVariantMap settings) override;

private:
	DispersionValuesMapForm* form;
	FeatureExtractor* featureExtractor;
	ValuePermutator* dispersionValuesPermutator;
	BUFFER_SOURCE bufferSource;
	int frameNr;
	int bufferNr;
	int nthBuffer;
	int bufferCounter;
	bool active;
	bool isCalculating;

	QVector<void*> frameBuffersRaw;
	QVector<void*> frameBuffersProcessed;
	int copyBufferId;
	size_t bytesPerFrameRaw;
	size_t bytesPerFrameProcessed;
	int lostBuffersRaw;
	int lostBuffersProcessed;
	unsigned int framesPerBuffer;
	unsigned int buffersPerVolume;

	void setupGuiConnections();
	void setupFeatureExtractor();
	void setupDispersionValuesPermutator();
	void initializeFrameBuffers();
	void releaseFrameBuffers(QVector<void*> buffers);

public slots:
	void storeParameters();
	virtual void rawDataReceived(void* buffer, unsigned int bitDepth, unsigned int samplesPerLine, unsigned int linesPerFrame, unsigned int framesPerBuffer, unsigned int buffersPerVolume, unsigned int currentBufferNr) override;
	virtual void processedDataReceived(void* buffer, unsigned int bitDepth, unsigned int samplesPerLine, unsigned int linesPerFrame, unsigned int framesPerBuffer, unsigned int buffersPerVolume, unsigned int currentBufferNr) override;

signals:
	void newFrame(void* frame, unsigned int bitDepth, unsigned int samplesPerLine, unsigned int linesPerFrame);
	void maxFrames(int max);
	void maxBuffers(int max);
};

#endif //DISPERSIONVALUESMAPEXTENSION_H
