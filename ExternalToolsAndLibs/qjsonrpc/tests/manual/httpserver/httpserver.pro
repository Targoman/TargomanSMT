BasePath = "../../.."

HEADERS = \
    testservice.h
SOURCES = \
    testservice.cpp \
    httpserver.cpp

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)


TEMPLATE = app
QT+=testlib
TARGET = httpserver
DESTDIR = $$BaseTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
INCLUDEPATH += $$BasePath/libsrc
QMAKE_LIBDIR += $$BaseLibraryFolder
LIBS += -l$$ProjectName
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/Private/
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/

