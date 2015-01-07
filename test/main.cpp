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

#include "libTargomanCore/Private/Input/clsInput.h"
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
    QTime Start = QTime::currentTime();

#if 1
    try{
        Targoman::Core::Private::Input::clsInput::init();

        Targoman::Core::Private::Input::clsInput Input;

    //    Input.parseRichIXML("This is a test .");
    //   Input.parseRichIXML("This <num> is</num> a test .");
    //    Input.parseRichIXML("This <num a=\"ss\">is</num> a test .");
    //    Input.parseRichIXML("<num a=\"ss>is</num> a test .");
    //    Input.parseRichIXML("This <num a=\"ss\">is<num> a test .");
        Input.parseRichIXML("This <1num a=\"ss\">isnum> a test .");

    }catch(exTargomanCore& e){
        TargomanError(e.what());
    }



#elif 0
    QFile File(_filePath);
    File.open(QFile::ReadOnly);

    if (!File.isReadable() || !File.isOpen())
        throw exConfiguration("Unable to open ARPA File: " + _filePath);
    Start = QTime::currentTime();
    char Buff[2000];
    while (!File.atEnd()) {
        File.readLine(Buff, 2000);
        Line++;
        if (Line % 10000000 == 0)
            TargomanLogDebug(1, QString::number(Line));
    }
 //133226ms
#elif 0
    QFile File(_filePath);
    File.open(QFile::ReadOnly);
    QTextStream Stream(&File);

    if (!File.isReadable() || !File.isOpen())
        throw exConfiguration("Unable to open ARPA File: " + _filePath);
    Start = QTime::currentTime();
    while (!File.atEnd()) {
        Stream.readLine();
        Line++;
        if (Line % 10000000 == 0)
            TargomanLogDebug(1, QString::number(Line));
    }
 //145652
#elif 0
    std::ifstream File(_filePath.toUtf8().constData());
    if (File.is_open() == false)
        throw exConfiguration("Unable to open ARPA File: " + _filePath);

    std::string LineString;
    Start = QTime::currentTime();
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


