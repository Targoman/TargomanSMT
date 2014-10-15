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

#ifndef TARGOMAN_COMMON_DEBUG_H_
#define TARGOMAN_COMMON_DEBUG_H_

#include <QDebug>
#include "libTargomanCommon/Macros.h"

/********************************************************************************************
 * Debugging Necessary Definitions and variables
 ********************************************************************************************/
namespace Targoman {
namespace Common {

extern short int  TARGOMAN_DEBUG_LEVEL;
extern short int  TARGOMAN_ERROR_LEVEL;
extern short int  TARGOMAN_WARNING_LEVEL;
extern short int  TARGOMAN_INFO_LEVEL;
extern bool       TARGOMAN_COLORED;

#if TARGOMAN_SHOW_DEBUG
  void printLoadedLibs();
#else
  void dummyPrintLoadedLibs();
  #define printLoadedLibs dummyPrintLoadedLibs
#endif

#define TARGOMANDBG_ALLWAYS 1
}
}
/********************************************************************************************
 * Colorizing Macros
 ********************************************************************************************/
#define TARGOMAN_OUTPUT_RESET        "\033[0m"

#define TARGOMAN_OUTPUT_COLOR_RED          "\033[1;25;31m"
#define TARGOMAN_OUTPUT_COLOR_GREEN        "\033[1;25;32m"
#define TARGOMAN_OUTPUT_COLOR_YELLOW       "\033[1;25;33m"
#define TARGOMAN_OUTPUT_COLOR_BLUE         "\033[1;25;34m"
#define TARGOMAN_OUTPUT_COLOR_MAGENTA      "\033[1;25;35m"
#define TARGOMAN_OUTPUT_COLOR_CYAN         "\033[1;25;36m"
#define TARGOMAN_OUTPUT_COLOR_WHITE        "\033[1;25;37m"

#define TARGOMAN_OUTPUT_BLINKING_RED       "\033[5;25;31m"
#define TARGOMAN_OUTPUT_BLINKING_GREEN     "\033[5;25;32m"
#define TARGOMAN_OUTPUT_BLINKING_YELLOW    "\033[5;25;33m"
#define TARGOMAN_OUTPUT_BLINKING_BLUE      "\033[5;25;34m"
#define TARGOMAN_OUTPUT_BLINKING_MAGENTA   "\033[5;25;35m"
#define TARGOMAN_OUTPUT_BLINKING_CYAN      "\033[5;25;36m"
#define TARGOMAN_OUTPUT_BLINKING_WHITE     "\033[5;25;37m"

#define TARGOMAN_COLOR_DEBUG    (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_COLOR_MAGENTA : TARGOMAN_OUTPUT_RESET)
#define TARGOMAN_COLOR_ERROR    (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_BLINKING_RED : TARGOMAN_OUTPUT_RESET)
#define TARGOMAN_COLOR_WARNING  (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_COLOR_YELLOW : TARGOMAN_OUTPUT_RESET)
#define TARGOMAN_COLOR_INFO     (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_COLOR_BLUE : TARGOMAN_OUTPUT_RESET)
#define TARGOMAN_COLOR_NORMAL   (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_RESET : TARGOMAN_OUTPUT_RESET)
#define TARGOMAN_COLOR_HAPPY    (TARGOMAN_COLORED ? TARGOMAN_OUTPUT_COLOR_GREEN : TARGOMAN_OUTPUT_RESET)

/********************************************************************************************
 * Debugging Macros
 ********************************************************************************************/
#if TARGOMAN_SHOW_DEBUG
  #if TARGOMAN_DEBUG_PROCESS_LINE
    #if TARGOMAN_DEBUG_PROCESS_LINE_SHOW_FILE
        #define TARGOMAN_DEBUG_CAUSE __FUNCTION__
    #else
        #define TARGOMAN_DEBUG_CAUSE Q_FUNC_INFO
    #endif

    #define TargomanDebugLine  {if (Targoman::Common::TARGOMAN_DEBUG_LEVEL > 7) \
        {qDebug("+++> [ %s : %d ][8] Processed!", TARGOMAN_DEBUG_CAUSE,__LINE__);}}

    #define TargomanDebug_Multi(_debugLevel,_fmt,...) \
    {fprintf( stderr,TARGOMAN_OUTPUT_COLOR_MAGENTA"+++> [ %s : %d ][%d]: " _fmt ""TARGOMAN_OUTPUT_RESET, \
      TARGOMAN_DEBUG_CAUSE,__LINE__, _debugLevel, __VA_ARGS__);}

    #define TargomanDebug_Single(_debugLevel,_lbl) \
    {fprintf( stderr,TARGOMAN_OUTPUT_COLOR_MAGENTA"+++> [ %s : %d ][%d]: %s"TARGOMAN_OUTPUT_RESET, TARGOMAN_DEBUG_CAUSE,__LINE__, _debugLevel, _lbl);}

    #define TargomanDebug_None(_debugLevel) \
    cerr<<TARGOMAN_COLOR_DEBUG"+++> [ "<<TARGOMAN_DEBUG_CAUSE<<" : "<<__LINE__<<" ]["<<_debugLevel<<"]: "<<TARGOMAN_OUTPUT_RESET

    #define TargomanDebug(_debugLevel,...) {PRINT_THIS_POINTER \
        {if (Targoman::Common::TARGOMAN_DEBUG_LEVEL >= _debugLevel){ \
            TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanDebug_,__VA_ARGS__)(_debugLevel, __VA_ARGS__)}}}

    #define TargomanDebugAbstract(_debugLevel,...) \
        {if (Targoman::Common::TARGOMAN_DEBUG_LEVEL >= _debugLevel){ \
            TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanDebug_,__VA_ARGS__)(_debugLevel, __VA_ARGS__)}}

  #else
    #define TargomanDebugLine {}
    #define TargomanDebug(_debugLevel, ...) {}
    #define TargomanDebugAbstract(_debugLevel,...) {}
  #endif
#else
    #define TargomanDebugLine {}
    #define TargomanDebug(_debugLevel, ...) {}
    #define TargomanDebugAbstract(_debugLevel,...) {}
#endif

#if TARGOMAN_DEFAULT_ERROR_LEVEL
    #if TARGOMAN_ERROR_SHOW_FILE
        #define TARGOMAN_ERROR_CAUSE __FILE__
    #else
        #define TARGOMAN_ERROR_CAUSE Q_FUNC_INFO

    #endif
    #define TargomanError_Multi(_fmt,...)\
        {qCritical("[ %s : %d ][ERROR][%d] " _fmt "", TARGOMAN_ERROR_CAUSE,__LINE__, Targoman::Common::TARGOMAN_ERROR_LEVEL, __VA_ARGS__);}
    #define TargomanError_Single(_lbl)\
        {qCritical("[ %s : %d ][ERROR][%d] %s", TARGOMAN_ERROR_CAUSE,__LINE__, Targoman::Common::TARGOMAN_ERROR_LEVEL, _lbl);}

    #define TargomanError(...)  {TARGOMAN_MACRO_ARG_BASED_FUNC(TargomanError_,__VA_ARGS__)(__VA_ARGS__)}

#else
    #define TargomanError(...) ;
#endif

#if TARGOMAN_DEFAULT_WARNING_LEVEL
    #if TARGOMAN_WARNING_SHOW_FILE
        #define TARGOMAN_WARNING_CAUSE __FILE__
    #else
        #define TARGOMAN_WARNING_CAUSE Q_FUNC_INFO

    #endif
    #define TARGOMANWarn_Multi(_warnLevel,_fmt,...)\
        {qWarning("[ %s : %d ][WARN][%d] " _fmt "", TARGOMAN_WARNING_CAUSE,__LINE__, _warnLevel, __VA_ARGS__);}
    #define TARGOMANWarn_Single(_warnLevel,_lbl)\
        {qWarning("[ %s : %d ][WARN][%d] %s", TARGOMAN_WARNING_CAUSE,__LINE__, _warnLevel, _lbl);}

    #define TARGOMANWarn(_warnLevel,...)  {if (Targoman::Common::TARGOMAN_WARNING_LEVEL >= _warnLevel) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TARGOMANWarn_,__VA_ARGS__)(_warnLevel, __VA_ARGS__)}}

#else
    #define TARGOMANWarn(_warnLevel,...) ;
#endif

#if TARGOMAN_DEFAULT_INFO_LEVEL
    #if TARGOMAN_INFO_SHOW_FILE
        #define TARGOMAN_INFO_CAUSE __FILE__
    #else
        #define TARGOMAN_INFO_CAUSE Q_FUNC_INFO

    #endif
    #define TARGOMANInfo_Multi(_infoLevel,_fmt,...)\
        {printf("[ %s : %d ][INFO][%d] " _fmt "\n", TARGOMAN_INFO_CAUSE,__LINE__, _infoLevel, __VA_ARGS__);}
    #define TARGOMANInfo_Single(_infoLevel,_lbl)\
        {printf("[ %s : %d ][INFO][%d] %s\n", TARGOMAN_INFO_CAUSE,__LINE__, _infoLevel, _lbl);}

    #define TARGOMANInfo(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_INFO_LEVEL >= _infoLevel) {\
        TARGOMAN_MACRO_ARG_BASED_FUNC(TARGOMANInfo_,__VA_ARGS__)(_infoLevel,__VA_ARGS__)}}

#else
    #define TargomanInfo(_infoLevel,...) ;
#endif

#if TARGOMAN_DEFAULT_INFO_LEVEL
  #if TARGOMAN_INFO_SHOW_FILE
    #define TARGOMAN_INFO_CAUSE __FILE__
  #else
    #define TARGOMAN_INFO_CAUSE Q_FUNC_INFO
  #endif

  #define TARGOMANInfo_Multi(_infoLevel,_fmt,...)\
  {printf("[ %s : %d ][INFO][%d] " _fmt "\n", TARGOMAN_INFO_CAUSE,__LINE__, _infoLevel, __VA_ARGS__);}
  #define TARGOMANInfo_Single(_infoLevel,_lbl)\
  {printf("[ %s : %d ][INFO][%d] %s\n", TARGOMAN_INFO_CAUSE,__LINE__, _infoLevel, _lbl);}

  #define TARGOMANInfo(_infoLevel,...)  {if (Targoman::Common::TARGOMAN_INFO_LEVEL >= _infoLevel) {\
  TARGOMAN_MACRO_ARG_BASED_FUNC(TARGOMANInfo_,__VA_ARGS__)(_infoLevel,__VA_ARGS__)}}

#else
  #define TargomanInfo(_infoLevel,...) ;
#endif

#endif /* TARGOMAN_COMMON_DEBUG_H_ */
