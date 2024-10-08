QT += core gui widgets printsupport
QMAKE_PROJECT_DEPTH = 0

TARGET = dispersionvaluesmapextension
TEMPLATE = lib
CONFIG += plugin

#define path of OCTproZ_DevKit share directory, plugin/extension directory
SHAREDIR = $$shell_path($$PWD/../../octproz_share_dev)
PLUGINEXPORTDIR = $$shell_path($$SHAREDIR/plugins)

CONFIG(debug, debug|release) {
	PLUGINEXPORTDIR = $$shell_path($$SHAREDIR/plugins/debug)
}
CONFIG(release, debug|release) {
	PLUGINEXPORTDIR = $$shell_path($$SHAREDIR/plugins/release)
}

#Create PLUGINEXPORTDIR directory if it does not already exist
exists($$PLUGINEXPORTDIR){
	message("sharedir already existing")
}else{
	QMAKE_POST_LINK += $$quote(mkdir $${PLUGINEXPORTDIR} $$escape_expand(\\n\\t))
}


DEFINES += \
	DISPERSIONVALUESMAP_LIBRARY \
	QT_DEPRECATED_WARNINGS #emit warnings if depracted Qt features are used

SOURCES += \
	src/thirdparty/qcustomplot/qcustomplot.cpp \
	src/dispersionvaluesmap.cpp \
	src/dispersionvaluesmapform.cpp \
	src/bitdepthconverter.cpp \
	src/imagedisplay.cpp \
	src/lineplot.cpp \
	src/colormapplot.cpp \
	src/featureextractor.cpp \
	src/valuepermutator.cpp \
	src/overlayitems/anchorpoint.cpp \
	src/overlayitems/overlayitem.cpp \
	src/overlayitems/rectoverlay.cpp

HEADERS += \
	src/thirdparty/qcustomplot/qcustomplot.h \
	src/dispersionvaluesmap.h \
	src/dispersionvaluesmapform.h \
	src/dispersionvaluesmapparameters.h \
	src/bitdepthconverter.h \
	src/imagedisplay.h \
	src/lineplot.h \
	src/colormapplot.h \
	src/featureextractor.h \
	src/valuepermutator.h \
	src/overlayitems/anchorpoint.h \
	src/overlayitems/overlayitem.h \
	src/overlayitems/rectoverlay.h

FORMS +=  \
	src/dispersionvaluesmapform.ui

INCLUDEPATH += \
	$$SHAREDIR \
	src \
	src/overlayitems \
	src/thirdparty \
	src/thirdparty/qcustomplot


#set system specific output directory for extension
unix{
	OUTFILE = $$shell_path($$OUT_PWD/lib$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})
}
win32{
	CONFIG(debug, debug|release) {
		OUTFILE = $$shell_path($$OUT_PWD/debug/$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})
	}
	CONFIG(release, debug|release) {
		OUTFILE = $$shell_path($$OUT_PWD/release/$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})
	}
}


#specifie OCTproZ_DevKit libraries to be linked to extension project
CONFIG(debug, debug|release) {
	unix{
		LIBS += $$shell_path($$SHAREDIR/debug/libOCTproZ_DevKit.a)
	}
	win32{
		LIBS += $$shell_path($$SHAREDIR/debug/OCTproZ_DevKit.lib)
	}
}
CONFIG(release, debug|release) {
	PLUGINEXPORTDIR = $$shell_path($$SHAREDIR/plugins/release)
	unix{
		LIBS += $$shell_path($$SHAREDIR/release/libOCTproZ_DevKit.a)
	}
	win32{
		LIBS += $$shell_path($$SHAREDIR/release/OCTproZ_DevKit.lib)
	}
}


##Copy extension to "PLUGINEXPORTDIR"
unix{
	QMAKE_POST_LINK += $$QMAKE_COPY $$quote($${OUTFILE}) $$quote($$PLUGINEXPORTDIR) $$escape_expand(\\n\\t)
}
win32{
	QMAKE_POST_LINK += $$QMAKE_COPY $$quote($${OUTFILE}) $$quote($$shell_path($$PLUGINEXPORTDIR/$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})) $$escape_expand(\\n\\t)
}

##Add extension to clean directive. When running "make clean" plugin will be deleted
unix {
	QMAKE_CLEAN += $$shell_path($$PLUGINEXPORTDIR/lib$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})
}
win32 {
	QMAKE_CLEAN += $$shell_path($$PLUGINEXPORTDIR/$$TARGET'.'$${QMAKE_EXTENSION_SHLIB})
}
