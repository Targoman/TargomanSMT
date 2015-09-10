BasePath = "../../.."

HEADERS = \
    testservices.h
SOURCES = \
    testservices.cpp \
    tst_qjsonrpcserver.cpp

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
TARGET = tst_qjsonrpcserver
DESTDIR = $$BaseUnitTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
INCLUDEPATH += $$BasePath/libsrc
QMAKE_LIBDIR += $$BaseLibraryFolder
LIBS += -l$$ProjectName
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/Private/
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/

