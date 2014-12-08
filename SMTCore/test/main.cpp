/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

//#include "libLIB_NAME/???.h"
#include "libTargomanCore/Configurations.h"
#include "libTargomanCommon/Logger.h"
#include <iostream>

#include <QFile>

//TODO remove
#include <fstream>
#include <string>
#include <QTime>


int main(int argc, char *argv[])
{
    QString ActorUUID;
    TARGOMAN_REGISTER_ACTOR("main")
    QString _filePath = "../../Resources/phrase-table";
    quint64 Line = 0;

#if 0
    QFile File(_filePath);
    File.open(QFile::ReadOnly);

    if (!File.isReadable() || !File.isOpen())
        throw exConfiguration("Unable to open ARPA File: " + _filePath);
    QTime Start = QTime::currentTime();
    char Buff[2000];
    while (!File.atEnd()) {
        File.readLine(Buff, 2000);
        Line++;
        if (Line % 10000000 == 0)
            TargomanLogDebug(1, QString::number(Line));
    }
 //133226ms
#elif 1
    QFile File(_filePath);
    File.open(QFile::ReadOnly);
    QTextStream Stream(&File);

    if (!File.isReadable() || !File.isOpen())
        throw exConfiguration("Unable to open ARPA File: " + _filePath);
    QTime Start = QTime::currentTime();
    while (!File.atEnd()) {
        Stream.readLine();
        Line++;
        if (Line % 10000000 == 0)
            TargomanLogDebug(1, QString::number(Line));
    }
 //145652
#else
    std::ifstream File(_filePath.toUtf8().constData());
    if (File.is_open() == false)
        throw exConfiguration("Unable to open ARPA File: " + _filePath);

    std::string LineString;
    QTime Start = QTime::currentTime();
    while (std::getline(File, LineString)) {
        Line++;
        if (Line % 10000000 == 0)
            TargomanLogDebug(1, QString::number(Line))
    }
//139433
#endif
    TargomanLogDebug(1, QString::number(Line))
    TargomanLogDebug(1,QString::number(Start.msecsTo(QTime::currentTime())));
}


