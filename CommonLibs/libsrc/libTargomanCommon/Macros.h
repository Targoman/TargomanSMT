/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
 /**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_COMMON_MACROS_H_
#define TARGOMAN_COMMON_MACROS_H_

#include <string>
#include <QString>
#include <cstring>

/********************************************************************************************
  Environmet detection macros
 ********************************************************************************************/
// Check and recognize arechitecture
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define TARGOMAN_ARCHITECTURE_64
  #else
    #define TARGOMAN_ARCHITECTURE_32
  #endif
#elif _WIN32 || _WIN64
  #if _WIN64
    #define TARGOMAN_ARCHITECTURE_64
  #else
    #define TARGOMAN_ARCHITECTURE_32
  #endif
#endif

/********************************************************************************************
 * Helper Macros (Macro in Macro) These macros must be used jusrt in other macros
 ********************************************************************************************/

#define TARGOMAN_MINCLUDE #include
#define TARGOMAN_M2CONCAT(_Arg1, _Arg2) _Arg1##_Arg2
#define TARGOMAN_M3CONCAT(_Arg1, _Arg2, _Arg3) _Arg1##_Arg2##_Arg3
#define TARGOMAN_M4CONCAT(_Arg1, _Arg2, _Arg3, _Arg4) _Arg1##_Arg2##_Arg3##_Arg4
#define TARGOMAN_M7CONCAT(_Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) _Arg1##_Arg2##_Arg3##_Arg4##_Arg5##_Arg6##_Arg7

#define TARGOMAN_STR_VALUE(_arg)      #_arg
#define TARGOMAN_M2STR(_Macro) TARGOMAN_STR_VALUE(_Macro)

#define TARGOMAN_MACRO_ARG_COUNT_HELPER( \
    _1,_2,_3,_4,_5,_6,_7,_8,_9,  \
    _10,_11,_12,_13,_14,_15,_16,_17,_18,_19,  \
    _20,_21,_22,_23,_24,_25,_26,_27,_28,_29,  \
    _30,_31,_32,_33,_34,_35,_36,_37,_38,_39,  \
    _40,_41,_42,_43,_44,_45,_46,_47,_48,_49,  \
    _50,_51,_52,_53,_54,_55,_56,_57,_58,_59,  \
    N,...) N

#define TARGOMAN_MACRO_ARG_COUNT(...) \
    TARGOMAN_MACRO_ARG_COUNT_HELPER(__VA_ARGS__,\
    59,58,57,56,55,54,53,52,51,50,  \
    49,48,47,46,45,44,43,42,41,40,  \
    39,38,37,36,35,34,33,32,31,30,  \
    29,28,27,26,25,24,23,22,21,20,  \
    19,18,17,16,15,14,13,12,11,10,  \
    9,8,7,6,5,4,3,2,1,0)

#define TARGOMAN_MACRO_ARG_BASED_FUNC(_fn,...) TARGOMAN_MACRO_ARG_COUNT_HELPER(__VA_ARGS__,\
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Single, None)

#define TARGOMAN_MACRO_ARG_FIRST(_arg, ...) _arg
#define TARGOMAN_MACRO_ARG_REST(_arg, ...) __VA_ARGS__
// Following definition is used in complex templates where a template is instantiating other templates which needs comma
#define TARGOMAN_VIRTUAL_COMMA() ,
/********************************************************************************************
 * Predefined functions. This macro must be included in all libraries.
 ********************************************************************************************/
#ifdef TARGOMAN_SHOW_DEBUG
    #define TARGOMAN_LIBRARY_VERSION \
    extern "C" QString getTargomanLibVersion() { \
      return QString("Version: ") + \
             QString(TARGOMAN_M2STR(PROJ_VERSION)) + \
             QString(" [DEBUG] Last Build: ") + \
             QString(__DATE__);}
#else
    #define TARGOMAN_LIBRARY_VERSION \
    extern "C" QString getTargomanLibVersion() { \
      return QString("Version: ") + \
             QString(TARGOMAN_M2STR(PROJ_VERSION)) + \
             QString(" [RELEASE] Last Build: ") + \
             v(__DATE__);}
#endif

/********************************************************************************************
 * Base Macros.
 ********************************************************************************************/
/** @brief A Macro to define a property with it's getters and setters */
#define TARGOMAN_DEFINE_PROPERTY(_type,_name) \
  public: inline _type& _name()  {return this->m_##_name;} \
  inline void set##_name(_type _value)  {this->m_##_name = _value;} \
  protected: _type m_##_name;

/** @brief A macro to define Enum in a semi-enhanced method in which Enums will reside in a namespace */
#define TARGOMAN_DEFINE_ENUM_BEGIN(_name) \
    namespace _name{ \
      enum Type {

#define TARGOMAN_DEFINE_ENUM_END \
      }; \
      inline static int getCount(){ return -1; } \
      inline static const char* toStr(Type) { return "Unknown"; } \
      inline static Type toEnum(const char*) { return (Type)-1; toEnum(""); }\
    }

/**
 * @brief A macro to define Enum in an enhanced method in which Enums will reside in a namespace and can have string
          definitions
 *
 * Use below enumeration definition macro to enable string manipulation of enums.
 * !!! IMPORTANT !!!
 *     All the enumerations defined by this macro will have a default member named Unknown numbered -1.
 *     You must not set default values for enumerations. Enumeration will start from 0.
 */
//TODO change this to VA_ARGS ENUM
#define TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(_name) \
namespace _name { enum Type { Unknown = -1,

#define TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS \
  }; static const char* Strings[] ={

/**
* @def getCount()
* returns number of values in this enumerations.
*/

/**
 * @def toStr
 * @return converts enum type value to char*
 */

/**
 * @def ToEnum
 * converts char* to enumeration
 */

#define TARGOMAN_DEFINE_ENHANCED_ENUM_END \
  }; \
  inline static int getCount() { return sizeof(Strings)/sizeof(char*); } \
  inline static const char* toStr(Type _type) { \
    if ((int)_type <= Unknown || (int)_type > getCount()) return "Unknown";\
    return Strings[_type]; \
    toStr(Unknown);  \
  } \
  inline static Type toEnum(const char* _name) { \
    if (!_name) \
      return Unknown; \
    int EnumSize = getCount(); \
    for(int i=0; i< EnumSize; i++) { \
      if (!strcmp(_name, Strings[i])) \
        return (Type) i; \
    } \
    return Unknown; \
    toEnum(""); \
  } \
  inline static QString listOptions(){ \
    QString Options = "( "; \
    int EnumSize = getCount(); \
    for(int i=0; i< EnumSize - 1; i++) \
        Options += QString("%1 | ").arg(Strings[i]); \
    return Options + Strings[EnumSize - 1] + " )"; \
  } \
}

/********************************************************************************************
 * Usefull Macros.
 ********************************************************************************************/
#define fastToLower(c)  ((c >= 'A' && c <= 'Z') ? c + 0x20 : c)
#define fastToUpper(c)  ((c >= 'a' && c <= 'z') ? c - 0x20 : c)
#define BIG_CONSTANT(x) (x##LLU)
#define MACRO_SAFE_COMMA ,

#endif /* TARGOMAN_COMMON_MACROS_H_ */
