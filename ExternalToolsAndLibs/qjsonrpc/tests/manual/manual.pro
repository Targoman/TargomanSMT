TEMPLATE = subdirs
SUBDIRS += \
           httpclient \
           httpserver-mt \
           httpserver \
           tcpclient \
           localclient \
           localserver \
           tcpserver \
           qjsonrpc \
           benchmark

greaterThan(QT_MAJOR_VERSION, 4) {
    qtHaveModule(script): SUBDIRS += console
} else {
    SUBDIRS += console
}
