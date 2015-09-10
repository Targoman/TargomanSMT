ISO639
===========================

This is a simple C header file used to convert between different ISO639 Language codes. All of the ISO639 types including ISO639-1, ISO639-2/T and ISO639-2/B are supported.

## API Documentation
* void ISO639init() initializes ISO639Tree to perform fast search. This function will be called implicitily by other functions if needed so no need to call it separately
* const stuISO639Info& ISO639getInfo(const char* _code): is used to get corresponding struct based on input _code which can be in ISO639-1 OR ISO639-2/B OR ISO639-2/T standard notation. If required input is found will return a valid stuISO639 struct with corresponding fields else it will return a stuISO639 struct with all fields set to NULL
* bool ISO639isValid(const char* _code): checks wheter input code is valid ISO639 code or not
* const char* ISO639getAlpha2(const char* _code) Retrieve ISO639-1 code from input code if input is valid ISO639 code else return NULL
* const char* ISO639getAlpha3B(const char* _code) Retrieve ISO639-2/B code from input code if input is valid ISO639 else return NULL code
* const char* ISO639getAlpha3T(const char* _code) Retrieve ISO639-2/T code from input code if input is valid ISO639 else return NULL
* const char* ISO639getName(const char* _code) Retrieve ISO639 name from input code if input is valid ISO639 else return NULL

## Build and test:

Dependencies:
 * GCC (+4)
 * Make or any IDE supported by CMake (Visual Studio, Eclipse, XCode, KDevelop, etc)


#### Configure and generate build files:

    mkdir -p $ISO639/build
    cd $ISO639/build
    cmake -DCMAKE_INSTALL_PREFIX:PATH=~/local ..

#### Install:

    cd $ISO639/build
    make install

###References
 http://www.iso.org/iso/home/standards/language_codes.htm

