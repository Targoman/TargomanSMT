TEMPLATE = subdirs
SUBDIRS += \
           httpclient \
           httpserver-mt \
           qjsonrpc \
           tcpclient \
           tcpserver \
           benchmark  \
           httpserver \
           localclient \
           localserver

greaterThan(QT_MAJOR_VERSION, 4) {
    qtHaveModule(script): SUBDIRS += console
} else {
    SUBDIRS += console
}
