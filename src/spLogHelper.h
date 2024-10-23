/**
 * @file spLogHelper.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief a class and macros for preparing the output of log messages
 * @version 1.0.1
 * @date 2024-10-22
 * @copyright Copyright (c) 2024
 * 
 * Version history:
 * v1.0.0   initial version
 * v1.0.1   splhLevelText array constructor fix
 * 
 * Notes:
 *  The classes logf() function's code is located here in the header file to allow for the templated function style.
 * 
 *  
 */

#ifndef SPLOGHELPER_H
#define SPLOGHELPER_H

#include <stdint.h>
#include <ctime>
#include <iomanip>
#include <list>
#include <string>
#include <functional>


// log levels
enum class splhLevel
{
  ALL,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  CRITICAL,
  NONE,
};


#define SPLH_LOG_LEVEL_ALL        0
#define SPLH_LOG_LEVEL_DEBUG      1
#define SPLH_LOG_LEVEL_INFO       2
#define SPLH_LOG_LEVEL_WARNING    3
#define SPLH_LOG_LEVEL_ERROR      4
#define SPLH_LOG_LEVEL_CRITICAL   5
#define SPLH_LOG_LEVEL_NONE       6

// limit if not previously defined
#ifndef SPLH_LOG_LEVEL_LIMIT
#define SPLH_LOG_LEVEL_LIMIT    SPLH_LOG_LEVEL_ALL
#endif



// log macros
#define spLOG_FUNCTION(level, format, ...)    spDefaultLogHelper.logf(level, __FILE__, __LINE__, __func__, format, __VA_ARGS__)
#define spLOG_SUPPRESSED    do {} while(0)

#if SPLH_LOG_LEVEL_LIMIT <= SPLH_LOG_LEVEL_DEBUG
#define spLOGF_D(format, ...) spLOG_FUNCTION(splhLevel::DEBUG, format, __VA_ARGS__)
#define spLOG_D(message) spLOG_FUNCTION(splhLevel::DEBUG, "%s", message)
#else
#define spLOGF_D(format, ...)   spLOG_SUPPRESSED
#define spLOG_D(message)        spLOG_SUPPRESSED
#endif

#if SPLH_LOG_LEVEL_LIMIT <= SPLH_LOG_LEVEL_INFO
#define spLOGF_I(format, ...) spLOG_FUNCTION(splhLevel::INFO, format, __VA_ARGS__)
#define spLOG_I(message) spLOG_FUNCTION(splhLevel::INFO, "%s", message)
#else
#define spLOGF_I(format, ...)   spLOG_SUPPRESSED
#define spLOG_I(message)        spLOG_SUPPRESSED
#endif

#if SPLH_LOG_LEVEL_LIMIT <= SPLH_LOG_LEVEL_WARNING
#define spLOGF_W(format, ...) spLOG_FUNCTION(splhLevel::WARNING, format, __VA_ARGS__)
#define spLOG_W(message) spLOG_FUNCTION(splhLevel::WARNING, "%s", message)
#else
#define spLOGF_W(format, ...)   spLOG_SUPPRESSED
#define spLOG_W(message)        spLOG_SUPPRESSED
#endif

#if SPLH_LOG_LEVEL_LIMIT <= SPLH_LOG_LEVEL_ERROR
#define spLOGF_E(format, ...) spLOG_FUNCTION(splhLevel::ERROR, format, __VA_ARGS__)
#define spLOG_E(message) spLOG_FUNCTION(splhLevel::ERROR, "%s", message)
#else
#define spLOGF_E(format, ...)   spLOG_SUPPRESSED
#define spLOG_E(message)        spLOG_SUPPRESSED
#endif

#if SPLH_LOG_LEVEL_LIMIT <= SPLH_LOG_LEVEL_CRITICAL
#define spLOGF_C(format, ...) spLOG_FUNCTION(splhLevel::CRITICAL, format, __VA_ARGS__)
#define spLOG_C(message) spLOG_FUNCTION(splhLevel::CRITICAL, "%s", message)
#else
#define spLOGF_C(format, ...)   spLOG_SUPPRESSED
#define spLOG_C(message)        spLOG_SUPPRESSED
#endif


// format macro
#define spLOG_FORMAT(...)    spDefaultLogHelper.setMessageFormat(__VA_ARGS__)
#define spLOG_TIME_FORMAT(timeFormat)    spDefaultLogHelper.setTimeFormat(timeFormat)


// register macro
#define spLOG_REG(function)   spDefaultLogHelper.registerHandlerCallback(function)
#define spLOG_UNREG(id)   spDefaultLogHelper.unregisterHandlerCallback(id)


// size of format buffer used
#ifndef spLOGHELPER_MSGBUFFER_LEN
#define spLOGHELPER_MSGBUFFER_LEN  240
#endif


// log format elements
enum class splhFormat : uint16_t
{
  TIME,
  LEVEL,
  FILENAME_LINE,
  FILENAME,
  LINE,
  FUNCTION,
};


/*  typedef for handler callback function
    void myHandlerFunc(const char *message, const splhLevel level, const char *timeString, 
                        const char *fileName, const uint32_t lineNo, const char *funcName);   
*/
typedef std::function<void(const char*, const splhLevel, const char*, const char*, const uint32_t, const char*)> splhHandlerCallback;


/**
 * @brief the spLogHelper class used for preparing the output of log messages.
 * 
 */
class spLogHelper {

  private:
    splhLevel _level = splhLevel::ALL;
    std::string _fTimeFormat = "%Y-%m-%e %H:%M:%S%z";
    std::list<splhFormat> _formatList = {splhFormat::TIME, splhFormat::LEVEL, splhFormat::FILENAME_LINE, splhFormat::FUNCTION};
    char* getMsgBufferPointer();
    const char* levelText(splhLevel level);
    const char* extractFileName(const char * filePath);
    bool callbacksExist();
    void handleCallbacks(const splhLevel level, const char *fileName, const uint32_t lineNo, const char *funcName);

  public:
    ~spLogHelper();
    uint32_t registerHandlerCallback(const splhHandlerCallback callback);
    void unregisterHandlerCallback(uint32_t id);
    std::string getTimeFormat();
    void setTimeFormat(std::string formatString);
    void setMessageFormat(std::initializer_list<splhFormat> formatList = {});
    template <class... Vs>
    void logf(splhLevel level, const char *fileName, const uint32_t lineNo, 
               const char *funcName, const char *format, Vs... args);
};


/**
 * @brief Creates a formatted message and passes it on to each registered callback.
 *        The final variadic arguments can be ommitted and instead a regular string passed to format.
 * 
 * @param level     a splhLevel value
 * @param fileName  the name of the file (__FILE__)
 * @param lineNo    the number of the line (__LINE__)
 * @param funcName  the name of the function (__func__)
 * @param format    a format string with printf() specifiers 
 * @param args      arguments to be used for the format string
 */
template <class... Vs>
void spLogHelper::logf(splhLevel level, const char *fileName, const uint32_t lineNo, 
  	                    const char *funcName, const char *format, Vs... args)
{
  if (callbacksExist())
  {
    snprintf(getMsgBufferPointer(), spLOGHELPER_MSGBUFFER_LEN, format, args...);
    handleCallbacks(level, extractFileName(fileName), lineNo, funcName);
  }
}


extern spLogHelper spDefaultLogHelper;


#endif // SPLOGHELPER_H

