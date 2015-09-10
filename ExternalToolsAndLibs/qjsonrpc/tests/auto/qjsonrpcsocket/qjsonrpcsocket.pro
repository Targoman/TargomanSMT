BasePath = "../../.."

SOURCES = tst_qjsonrpcsocket.cpp
RESOURCES = tst_qjsonrpcsocket.qrc

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
TARGET = tst_qjsonrpcsocket
DESTDIR = $$BaseUnitTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
INCLUDEPATH += $$BasePath/libsrc
QMAKE_LIBDIR += $$BaseLibraryFolder
LIBS += -l$$ProjectName
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/Private/
INCLUDEPATH+=$$BasePath/libsrc/libQJsonRPC/



