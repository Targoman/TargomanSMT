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

#ifndef TARGOMAN_COMMON_CMDIO_H_
#define TARGOMAN_COMMON_CMDIO_H_

#include <QDateTime>
#include <QTextStream>
#include <stdio.h>
#include "libTargomanCommon/Macros.h"

/********************************************************************************************
 * Debugging Necessary Definitions and variables
 ********************************************************************************************/
namespace Targoman {
namespace Common {

class clsOutputSettings{
public:
    clsOutputSettings(){
        this->Details = 0xFF;
    }

    inline bool canBeShown(quint8 _level){
        return (this->Details & 0x0F) >= _level;
    }
    inline bool validateLevel(quint8 _level){
        Q_ASSERT_X(_level < 10, "CmdIO",  "Level must be between 0 to 9");
        if (_level > 9)
            throw std::bad_exception();
    }

    inline void setLevel(quint8 _level){
        this->validateLevel(_level);
        this->Details = (this->Details & 0xF0) + _level;
    }

    inline void setDetails(bool _time = false, bool _func = false, bool _line = false, bool _file = false) {
        this->Details &= 0x0F;
        if(_time)
            this->Details |= 0x10;
        if(_func)
            this->Details |= 0x40;
        if(_file)
            this->Details |= 0x20;
        if(_line)
            this->Details |= 0x80;
    }

    inline void set(quint8 _level, bool _time = false, bool _func = false, bool _line = false, bool _file = false) {
        this->validateLevel(_level);
        this->Details = _level;
        if(_time)
            this->Details |= 0x10;
        if(_func)
            this->Details |= 0x40;
        if(_file)
            this->Details |= 0x20;
        if(_line)
            this->Details |= 0x80;
    }

    inline QString details(const char* _function, const char* _file, quint16 _line){
        QString OutStr;
        OutStr +=  (this->Details & 0x10 ?
                    QString("[" + QDateTime().currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz") + "]") :
                    QString(""));
        if (this->Details & 0x60){
            OutStr += "[";
            if (this->Details & 0x20)
                    OutStr +=  QString(" %1 %2").arg(_file).arg(this->Details & 0xC0 ? ":" : "");
            if (this->Details & 0x40)
                    OutStr +=  QString(" %1 ").arg(_function);
            if (this->Details & 0x80)
                OutStr += QString(":%1").arg(_line) + " ]";
            else
                OutStr += " ]";
        }

        return OutStr;
    }

private:
    quint8 Details; //0-7: Levels, 0x10:TimeStamp, 0x20: File, 0x40: Func, 0x80: Line
};


class clsIOSettings{
public:
    clsIOSettings(){
        this->setDefault();
    }

    void setSilent(){
        this->Debug.setLevel(0);
        this->Info.setLevel(0);
        this->Warning.setLevel(0);
        this->Happy.setLevel(0);
        this->Normal.setLevel(0);
//        this->Error.setLevel(0); //Error must not be silented
    }

    void setFull(){
        this->Debug.set(9,true,true,true);
        this->Info.set(9,true,true,true);
        this->Warning.set(9,true,true,true);
        this->Happy.set(9,true,true,true);
        this->Error.set(9,true,true,true);
        this->Normal.set(9,true,true,true);
    }

    void setDefault(quint8 _debugLevel = 5, quint8 _otherLevel = 9){
        this->Debug.set(_debugLevel,true,true,true);
        this->Info.set(_otherLevel,true);
        this->Warning.set(_otherLevel,true);
        this->Happy.set(_otherLevel,true);
        this->Error.set(_otherLevel,true);
        this->Normal.set(_otherLevel,true);
    }

    clsOutputSettings Debug;
    clsOutputSettings Info;
    clsOutputSettings Warning;
    clsOutputSettings Happy;
    clsOutputSettings Error;
    clsOutputSettings Normal;
    bool ShowColored;

    //TODO add silent, default, and property configurations
} extern TARGOMAN_IO_SETTINGS;

void printLoadedLibs();

}
}
/********************************************************************************************
 * Colorizing Macros
 ********************************************************************************************/
#define TARGOMAN_OUTPUT_RESET        "\033[0m"

#define TARGOMAN_COLOR_RED          "\033[1;25;31m"
#define TARGOMAN_COLOR_GREEN        "\033[1;25;32m"
#define TARGOMAN_COLOR_YELLOW       "\033[1;25;33m"
#define TARGOMAN_COLOR_BLUE         "\033[1;25;34m"
#define TARGOMAN_COLOR_MAGENTA      "\033[1;25;35m"
#define TARGOMAN_COLOR_CYAN         "\033[1;25;36m"
#define TARGOMAN_COLOR_WHITE        "\033[1;25;37m"

#define TARGOMAN_COLOR_BLINKING_RED       "\033[5;25;31m"
#define TARGOMAN_COLOR_BLINKING_GREEN     "\033[5;25;32m"
#define TARGOMAN_COLOR_BLINKING_YELLOW    "\033[5;25;33m"
#define TARGOMAN_COLOR_BLINKING_BLUE      "\033[5;25;34m"
#define TARGOMAN_COLOR_BLINKING_MAGENTA   "\033[5;25;35m"
#define TARGOMAN_COLOR_BLINKING_CYAN      "\033[5;25;36m"
#define TARGOMAN_COLOR_BLINKING_WHITE     "\033[5;25;37m"

#define TARGOMAN_COLOR_DEBUG    (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_COLOR_MAGENTA : "" )
#define TARGOMAN_COLOR_ERROR    (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_COLOR_BLINKING_RED : "" )
#define TARGOMAN_COLOR_WARNING  (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_COLOR_YELLOW : "" )
#define TARGOMAN_COLOR_INFO     (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_COLOR_BLUE : "" )
#define TARGOMAN_COLOR_NORMAL   (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_OUTPUT_RESET : "" )
#define TARGOMAN_COLOR_HAPPY    (Targoman::Common::TARGOMAN_IO_SETTINGS.ShowColored ? TARGOMAN_COLOR_GREEN : "" )

/********************************************************************************************
 * Output Macros
 ********************************************************************************************/
#if TARGOMAN_SHOW_DEBUG
    #if TARGOMAN_DEBUG_PROCESS_LINE

      #define TargomanDebugLine  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.canBeShown(7)) \
          {fprintf(stderr,"%s+++> %s[8] %sProcessed!\n", TARGOMAN_COLOR_DEBUG, \
                Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
                TARGOMAN_COLOR_NORMAL);}}
    #else
      #define TargomanDebugLine {}
    #endif

    #define TargomanDebug_Multi(_debugLevel,_newline,_fmt,...) \
    {fprintf(stderr,"%s+++> %s[%d]: %s" _fmt _newline,TARGOMAN_COLOR_DEBUG, \
        Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
        _debugLevel, TARGOMAN_COLOR_NORMAL, __VA_ARGS__);}

    #define TargomanDebug_Single(_debugLevel,_newline,_stream) \
    {QString Buffer; fprintf(stderr,"%s+++> %s[%d]: %s%s" _newline, TARGOMAN_COLOR_DEBUG, \
        Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
         _debugLevel, \
        ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()) \
        , TARGOMAN_COLOR_NORMAL);}

    #define TargomanDebug(_debugLevel,...) { \
        {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.canBeShown(_debugLevel)){ \
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanDebug_,__VA_ARGS__)(_debugLevel, "\n",__VA_ARGS__)}}}

    #define TargomanInlineDebug(_debugLevel,...) { \
        {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.canBeShown(_debugLevel)){ \
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanDebug_,__VA_ARGS__)(_debugLevel,"",__VA_ARGS__)}}}

    #define TargomanFinishInlineDebug(_colorType, _stream) {\
        QString Buffer;\
        if (Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.canBeShown(1)) \
        fprintf(stderr,"%s[%s]%s\n", _colorType, \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()) \
            ,TARGOMAN_COLOR_NORMAL);}

#else
    #define TargomanDebugLine {}
    #define TargomanDebug(_debugLevel, ...) {}
    #define TargomanInlineDebug(_debugLevel, ...) {}
    #define TargomanFinishInlineDebug(_color, _lbl) {}
#endif

#define TargomanError_Multi(_fmt,...)\
    {fprintf(stderr,"%s%s[ERROR][-] %s" _fmt "\n", TARGOMAN_COLOR_ERROR, \
        Targoman::Common::TARGOMAN_IO_SETTINGS.Error.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
        TARGOMAN_COLOR_NORMAL, __VA_ARGS__);}
#define TargomanError_Single(_stream)\
    {QString Buffer; fprintf(stderr,"%s%s[ERROR][-] %s%s\n", TARGOMAN_COLOR_ERROR, \
        Targoman::Common::TARGOMAN_IO_SETTINGS.Error.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()) \
            , TARGOMAN_COLOR_NORMAL);}

#define TargomanError(...)  {TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanError_,__VA_ARGS__)(__VA_ARGS__)}

#if TARGOMAN_SHOW_WARNING
    #define TargomanWarn_Multi(_warnLevel,_fmt,...)\
        {fprintf(stderr,"%s%s[WARN][%d] %s" _fmt "\n", TARGOMAN_COLOR_WARNING, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Warning.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _warnLevel, TARGOMAN_COLOR_NORMAL, __VA_ARGS__);}
    #define TargomanWarn_Single(_warnLevel,_stream)\
        {QString Buffer;fprintf(stderr,"%s%s[WARN][%d] %s%s\n", TARGOMAN_COLOR_WARNING,  \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Warning.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _warnLevel, TARGOMAN_COLOR_NORMAL, \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()));}

    #define TargomanWarn(_warnLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Warning.canBeShown(_warnLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanWarn_,__VA_ARGS__)(_warnLevel, __VA_ARGS__)}}
#else
    #define TargomanWarn(_warnLevel,...) {}
#endif

#if TARGOMAN_SHOW_INFO
    #define TargomanInfo_Multi(_infoLevel,_newline,_fmt,...)\
        {fprintf(stdout,"%s%s[INFO][%d] %s" _fmt _newline, TARGOMAN_COLOR_INFO, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Info.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _infoLevel, TARGOMAN_COLOR_NORMAL, __VA_ARGS__);}
    #define TargomanInfo_Single(_infoLevel,_newline,_stream)\
        {QString Buffer; fprintf(stdout,"%s%s[INFO][%d] %s%s" _newline, TARGOMAN_COLOR_INFO, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Info.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _infoLevel, TARGOMAN_COLOR_NORMAL, \
           ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()));}

    #define TargomanInfo(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Info.canBeShown(_infoLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanInfo_,__VA_ARGS__)(_infoLevel, "\n",__VA_ARGS__)}}
    #define TargomanInlineInfo(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Info.canBeShown(_infoLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanInfo_,__VA_ARGS__)(_infoLevel, "",__VA_ARGS__)}}

    #define TargomanFinishInlineInfo(_colorType, _stream) {\
        QString Buffer;\
        if (Targoman::Common::TARGOMAN_IO_SETTINGS.Info.canBeShown(1))   \
            fprintf(stdout,"%s[%s]%s\n", _colorType, \
                ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()) \
                ,TARGOMAN_COLOR_NORMAL);}
#else
    #define TargomanInfo(_infoLevel,...) {}
    #define TargomanInlineInfo(_infoLevel,...) {}
    #define TargomanFinishInlineInfo(_color, _lbl) {}
#endif

#if TARGOMAN_SHOW_NORMAL
    #define TargomanOut_Multi(_infoLevel,_newline,_fmt,...)\
        {fprintf(stdout,"%s[NORMAL][%d] " _fmt _newline, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Normal.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _infoLevel,  __VA_ARGS__);}
    #define TargomanOut_Single(_infoLevel,_newline,_stream)\
        {QString Buffer;fprintf(stdout,"%s[NORMAL][%d] %s" _newline,  \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Normal.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _infoLevel,  \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()));}

    #define TargomanOut(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Normal.canBeShown(_infoLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanOut_,__VA_ARGS__)(_infoLevel, "\n",__VA_ARGS__)}}
    #define TargomanInlineOut(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Normal.canBeShown(_infoLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanOut_,__VA_ARGS__)(_infoLevel, "",__VA_ARGS__)}}

    #define TargomanFinishInlineOut(_colorType, _stream) {\
        QString Buffer; \
        if (Targoman::Common::TARGOMAN_IO_SETTINGS.Normal.canBeShown(1)) \
            fprintf(stdout,"%s[%s]%s\n", _colorType, \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()) \
            ,TARGOMAN_COLOR_NORMAL);}
#else
    #define TargomanOut(_infoLevel,...) {}
    #define TargomanInlineOut(_infoLevel,...) {}
    #define TargomanFinishInlineOut(_color, _lbl) {}
#endif

#if TARGOMAN_SHOW_HAPPY
    #define TargomanHappy_Multi(_happyLevel,_fmt,...)\
        {fprintf(stdout,"%s%s[HAPPY][%d] %s" _fmt "\n", TARGOMAN_COLOR_HAPPY, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Happy.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _happyLevel, TARGOMAN_COLOR_NORMAL, __VA_ARGS__);}
    #define TargomanHappy_Single(_happyLevel,_stream)\
        {QString Buffer; fprintf(stdout,"%s%s[HAPPY][%d] %s%s\n", TARGOMAN_COLOR_HAPPY, \
            Targoman::Common::TARGOMAN_IO_SETTINGS.Happy.details(Q_FUNC_INFO,__FILE__,__LINE__).toUtf8().constData(), \
            _happyLevel, TARGOMAN_COLOR_NORMAL, \
            ((QTextStream(&Buffer)<<_stream).string()->toUtf8().constData()));}

    #define TargomanHappy(_happyLevel,...)  {if (Targoman::Common::TARGOMAN_IO_SETTINGS.Happy.canBeShown(_happyLevel)) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanHappy_,__VA_ARGS__)(_happyLevel, __VA_ARGS__)}}
#else
    #define TargomanHappy(_happyLevel,...) {}
#endif

#endif /* TARGOMAN_COMMON_CMDIO_H_ */
