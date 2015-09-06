/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanSMT/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanSMT/Translator.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"


//JUST FOR DEBUG
#include "Private/RuleTable/clsBinaryRuleTable.h"


#include <QDebug>

#include <iostream>

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace Targoman::SMT;

void printMemoryUsage(const QString& _stepString){
    QFile ProcessMemStatFile(QString("/proc/%1/status").arg(QCoreApplication::applicationPid()));
    ProcessMemStatFile.open(QFile::ReadOnly);
    QString Line;
    QTextStream ProcessMemInfoStream(ProcessMemStatFile.readAll());

    QString VirtualPeak;
    QString ResidentPeak;
    QString VirtualMemory;
    QString ResidentMemory;
    QString SharedMemory;


    while(ProcessMemInfoStream.atEnd() == false)
    {
      Line = ProcessMemInfoStream.readLine();
      if (Line.startsWith("VmPeak"))
        VirtualPeak = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmHWM"))
        ResidentPeak =Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmSize"))
        VirtualMemory = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmRSS"))
        ResidentMemory = Line.replace(" ","").split(":").at(1).trimmed();
      else if (Line.startsWith("VmLib"))
        SharedMemory = Line.replace(" ","").split(":").at(1).trimmed();
    }
    std::cerr<<"********************************************************************"<<std::endl;
    std::cerr<<"Memory Stat ["<<qPrintable(_stepString)<<
              "]: VirtPeak:"<<qPrintable(VirtualMemory)<<
              " CurrVirt:"<<qPrintable(VirtualMemory)<<
              " ResPeak:"<<qPrintable(ResidentPeak)<<
              " CurRes:"<<qPrintable(ResidentMemory)<<
              " Shared:"<<qPrintable(SharedMemory)<<std::endl;
    std::cerr<<"********************************************************************"<<std::endl;
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    TARGOMAN_IO_SETTINGS.setDefault(9);
    TARGOMAN_IO_SETTINGS.Debug.setDetails(true);
    try{

        /*
        ConfigManager::instance().init("dummy-license", QStringList()<<"-c"<<"./Targoman.conf");
        Translator::init(ConfigManager::instance().configFilePath());

        QStringList RefEn = {
            "Izzet Ibrahim Meets Saudi Trade Official in Baghdad ",
             "Baghdad 1-1 ( AFP - Iraq's official news agency reported that the Deputy Chairman of the Iraqi Revolutionary Command Council , Izzet Ibrahim , today met with Abdul Rahman al-Zamil , Managing Director of the Saudi Center for Export Development . ",
             "The agency said Ibrahim welcomed this occasion for trade exchange and cooperation between Iraq and Saudi Arabia . ",
             "The agency also reported that the Iraqi Minister of Trade , Mohamed Mehdi Salih , took part in the meeting . ",
             "Baghdad and Riyadh , who broke diplomatic relations during the Gulf War in 1991 , began to improve their relations over the course of the Beirut Summit last March . ",
             "Saudi sources this week denied reports in the American New York Times that Saudi Arabia had agreed to allow the United States to use Saudi military bases should a war against Iraq take place . ",
             "Prince Abdulrahman bin Azziz , Saudi Arabia's Defense Minister , said in a statement to Saudi newspaper Uqath that \" what was claimed by the newspaper is incorrect . \" ",
             "He added : \" The Kingdom's position has been clear since the beginning and we are not able to place our airspace and bases \" under American control . ",
             "Saudi Arabia was a base for American forces during the Gulf War ( January and February 1991 ) . ",
             "Israeli Army Destroys 10 Palestinian Homes during Incursion into Rafah "};

        QStringList ExpectedFa = {
            QStringLiteral("Izzet ابراهیم با تجارت سعودی در بغداد رسمی"),
            QStringLiteral("بغداد Iraq's 1-1 ( AFP ) - آژانس خبری رسمی گزارش داد که معاون رئیس شورای فرماندهی انقلاب عراق , Izzet ابراهیم , امروز با عبدالرحمان al-Zamil , مدیر مرکز توسعه صادرات عربستان سعودی ."),
            QStringLiteral("این آژانس گفت که ابراهیم از این موقعیت را برای تبادل تجاری و همکاری بین عراق و عربستان سعودی است ."),
            QStringLiteral("این آژانس همچنین گزارش داد که وزیر تجارت عراق , محمد مهدی صالح , در این جلسه شرکت کردند ."),
            QStringLiteral("بغداد و ریاض , که روابط دیپلماتیک در طی جنگ خلیج فارس در سال 1991 , شروع به بهبود روابط خود را در طول نشست سران بیروت در ماه مارس گذشته وارد کرده است ."),
            QStringLiteral("منابع عربستان سعودی را در این هفته گزارش نیویورک تایمز آمریکا که عربستان سعودی موافقت کرده بود تا به ایالات متحده اجازه استفاده از پایگاه های نظامی عربستان سعودی باید یک جنگ علیه عراق است ."),
            QStringLiteral("شاهزاده عبدالرحمن بن طارق عزیز Arabia's سعودی , وزیر دفاع , در بیانیه ای به روزنامه سعودی Uqath \" آنچه که ادعا شد که توسط روزنامه نادرست است . \""),
            QStringLiteral("وی افزود : \" این موقعیت Kingdom's روشن است و ما از آغاز قادر به خود و پایگاه های هوایی \" تحت کنترل آمریکا ."),
            QStringLiteral("عربستان سعودی یک پایگاه برای نیروهای آمریکایی در جنگ خلیج فارس ( ژانویه و فوریه 1991 ) ."),
            QStringLiteral("ارتش اسرائیل در طی 10 فرانیجا در خانه‌های فلسطینی در رفح پیدا نمودند ; تهاجم")
        };

        for(int i = 0; i < ExpectedFa.size(); ++i) {
            QString Tranlation = Translator::translate(RefEn[i], false).Translation;
            if( Tranlation != ExpectedFa.at(i)) {
                qDebug() << "At line" << i << "Expected to get";
                qDebug() << ExpectedFa[i];
                qDebug() << "but got";
                qDebug() << Tranlation;
                exit(0);
            }
        }

        qDebug() << "Translation from En to Fa finished successfully";
        //*/

        //*
        ConfigManager::instance().init("dummy-license", QStringList()<<"-c"<<"./Targoman.fa_en.conf");
        Translator::init(ConfigManager::instance().configFilePath());

        QStringList RefFa = {
            QStringLiteral("ایزت ابراهیم مامور تجاری عبرستانی را در بغداد ملاقات کرد "),
            QStringLiteral("بغداد 1-1 ( AFP اژانس خبری رسمی عراق گزارش داد که امروز نایب رییس هییت فرماندهی انقلابی عراق با عبدالرحمان الزملی مدیر عامل مرکز صعودی برای توسعه ی صادرات ملاقات کرد . "),
            QStringLiteral("این اژانس گزارش داد که ابراهیم از این موقعیت برای مبادله ی تجاری و همکاری بین عراق و عربستان صعودی استقبال کرد . "),
            QStringLiteral("این اژانس همچنین گزارش داد که مهدی صالح در این جلسه شرکت کرده است "),
            QStringLiteral("بغداد و ریاض که هنگام جنگ خلیج در سال 1991 روابط دیپلماتیک خود را با هم قطع کرده بودند , بهبود روابط خود را در جریان جلسه ی بیروت در مارچ گذشته اغاز کردند . "),
            QStringLiteral("منابع صعودی در این هفته گزارشات نیویورک تایمز را در مورد این که عربستان پذیرفته است که به امریکا اجازه بدهد که از پایگاه نظامی او برای جنگ احتمالی عراق اغاز کند رد کرد . "),
            QStringLiteral("ولیعهد عبدالرحمان ابن عزیز , وزیر دفاع عربستان در یک بیانیه در روزنامه ی اوقات اعلام کرد که این ادعا نادرست این روزنامه است . "),
            QStringLiteral("او افزود موضع این حکومت سلطنتی از اغاز روشن بود و ما نمی توانیم از محدوه ی هوایی و پایگاه های خود تحت کنترل امریکا استفاده کنیم . "),
            QStringLiteral("عربستان صعودی یک پایگاه نیروهای امریکایی هنگام جنگ خلیج در ژانویه و فوریه ی سال 1991 ) بوده است . "),
            QStringLiteral("ارتش اسراییل 10 خانه ی فلسطینی را هنگام حمله به رفا ویران کردند . ")
        };

        QStringList ExpectedEn = {
            "ایزت Ibrahim commercial agent عبرستانی met in Baghdad",
            "Baghdad ( AFP اژانس 1-1 official Iraqi news reported today that the board vice رییس Revolutionary Command Iraq with Abdul Rahman الزملی , managing director of the Saudi Centre for developing exports .",
            "The اژانس reported that Ibrahim of the occasion for trade exchange and cooperation between Iraq and Saudi Arabia .",
            "The اژانس also reported that the Mahdi Saleh has participated in the meeting",
            "Baghdad and Riyadh during the first Gulf War in 1991 with the diplomatic ties were severed , improve their relations at a meeting of اغاز in Beirut last March .",
            "Saudi sources in this week in the New York Times reports about the that Saudi Arabia has agreed to allow امریکا from the military base for the potential war on Iraq to اغاز .",
            "Crown Prince Abd al - Rahman Ibn Aziz , the Saudi defence minister in a statement at the time , the newspaper said that this is false claim the newspaper .",
            "He added that the position of the monarchy of اغاز was clear , and we don 't we can radius from the air and their bases امریکا under control .",
            "Saudi Arabia a base امریکایی forces during the Gulf War in January and February 1991 ) .",
            "Israeli army 10 Palestinian houses during the attack destroyed Rifa ."
        };

        for(int i = 0; i < ExpectedEn.size(); ++i) {
            QString Tranlation = Translator::translate(RefFa[i], false).Translation;
            if( Tranlation != ExpectedEn.at(i)) {
                qDebug() << "At line" << i << "Expected to get";
                qDebug() << ExpectedEn[i];
                qDebug() << "but got";
                qDebug() << Tranlation;
                exit(0);
            }
        }

        qDebug() << "Translation from FA to En finished successfully";
        //*/

    }catch(exTargomanBase& e){
        qDebug()<<e.what();
        TargomanError(e.what());
        return 1;
    }
    return 0;
}


