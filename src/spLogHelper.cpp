/**
 * @file spLogHelper.cpp
 * @author krokoreit (krokoreit@gmail.com)
 * @brief class and macros for preparing the output of log messages
 * @version 1.0.0
 * @date 2024-10-22
 * @copyright Copyright (c) 2024
 * 
 */

#include <spLogHelper.h>
#include <vector>

// default object
spLogHelper spDefaultLogHelper;

// callbackID & registers
uint32_t cbNextID = 0;
std::vector<uint32_t> regIDs;
std::vector<spLogHelper*> regOwners;
std::vector<splhHandlerCallback> regCallbacks;

// for level to text conversion
std::array splhLevelText
{
  "ALL",
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "CRITICAL",
  "NONE",
};

// message buffer
char* pMsgBuffer = nullptr;


/*    PUBLIC    PUBLIC    PUBLIC    PUBLIC    

      xxxxxxx   xx    xx  xxxxxxx   xx           xx      xxxxxx 
      xx    xx  xx    xx  xx    xx  xx           xx     xx    xx
      xx    xx  xx    xx  xx    xx  xx           xx     xx      
      xxxxxxx   xx    xx  xxxxxxx   xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx      
      xx        xx    xx  xx    xx  xx           xx     xx    xx
      xx         xxxxxx   xxxxxxx   xxxxxxxx     xx      xxxxxx 
     

      PUBLIC    PUBLIC    PUBLIC    PUBLIC    */

/**
 * @brief Destroy the sp Log Helper::sp Log Helper object. Ensures that all registered callbacks will be unregistered.
 * 
 */
spLogHelper::~spLogHelper()
{

  // delete callbacks for this spLogHelper
  size_t count = regOwners.size();
  size_t i = 0;
  while (i < count)
  {
    if (regOwners[i] == this)
    {
      regIDs.erase(regIDs.begin() + i);
      regOwners.erase(regOwners.begin() + i);
      regCallbacks.erase(regCallbacks.begin() + i);
      count--;
    }
    else
    {
      i++;
    }
  }
}

/**
 * @brief Registers a callback function, which will be invoked each time logf() or a log macro is used.
 * 
 * @param callback    the handler function to be called
 * @return uint32_t   an unique ID for this registration
 */
uint32_t spLogHelper::registerHandlerCallback(const splhHandlerCallback callback)
{
  regIDs.emplace_back(cbNextID);
  regOwners.insert(regOwners.end(), this);
  regCallbacks.emplace_back(callback);
  return cbNextID++;
}

/**
 * @brief Deletes a previously registered callback function from the registry.
 * 
 * @param id     ID of the registration
 */
void spLogHelper::unregisterHandlerCallback(uint32_t id)
{
  size_t count = regIDs.size();
  for (size_t i = 0; i < count; i++)
  {
    if (regIDs[i] == id)
    {
      regIDs.erase(regIDs.begin() + i);
      regOwners.erase(regOwners.begin() + i);
      regCallbacks.erase(regCallbacks.begin() + i);
      break;
    }
  }
}

/**
 * @brief Returns the current format string used to format the splhFormat::TIME part of the log message.
 * 
 * @return std::string    current format string
 */
std::string spLogHelper::getTimeFormat()
{
  return _fTimeFormat;
}

/**
 * @brief Sets the format string used to format the splhFormat::TIME part of the log message created.
 * 
 * @param formatString    new format string to use
 */
void spLogHelper::setTimeFormat(std::string formatString)
{
  _fTimeFormat = formatString;
}

/**
 * @brief Sets the log message format to the sequence of splhFormat elements specified.
 * 
 * @param formatList  list of splhFormats to be used
 */
void spLogHelper::setMessageFormat(std::initializer_list<splhFormat> formatList)
{
  _formatList = formatList;
 }


/*    PRIVATE    PRIVATE    PRIVATE    PRIVATE

      xxxxxxx   xxxxxxx      xx     xx    xx     xx     xxxxxxxx  xxxxxxxx
      xx    xx  xx    xx     xx     xx    xx    xxxx       xx     xx      
      xx    xx  xx    xx     xx     xx    xx   xx  xx      xx     xx      
      xxxxxxx   xxxxxxx      xx      xx  xx   xx    xx     xx     xxxxxxx    
      xx        xx    xx     xx      xx  xx   xxxxxxxx     xx     xx    
      xx        xx    xx     xx       xxxx    xx    xx     xx     xx      
      xx        xx    xx     xx        xx     xx    xx     xx     xxxxxxxx
     

      PRIVATE    PRIVATE    PRIVATE    PRIVATE    */

/**
 * @brief Ensudres that a message buffer exists and returns a pointer to it.
 * 
 * @return char*    pointer to char buffer
 */
char* spLogHelper::getMsgBufferPointer()
{
  if (pMsgBuffer == nullptr)
  {
    pMsgBuffer = (char*)malloc(spLOGHELPER_MSGBUFFER_LEN);
  }
  return pMsgBuffer;
}

/**
 * @brief Returns the text description for a given splhLevel.
 * 
 * @param level           the level to look for
 * @return const char*    the text for level
 */
const char* spLogHelper::levelText(splhLevel level)
{
  if (level < splhLevel::DEBUG)
  {
    level = splhLevel::DEBUG;
  }
  else if (level > splhLevel::CRITICAL)
  {
    level = splhLevel::CRITICAL;
  }
  uint32_t lvl = (uint32_t) level;
  return splhLevelText[lvl];
}


/**
 * @brief Extracts file name from __FILE__.
 *        Simplified approach to find the part after the last separator (Linux & Windows).
 *        This ignores that '\\' is a valid file name char in Linux !!
 * 
 * @param filePath      file name with path
 * @return const char*  pointer to position after last separator
 */
const char* spLogHelper::extractFileName(const char * filePath)
{
    size_t pos = 0;
    size_t foundPos = 0;
    char* c = (char *)filePath;
    while(*c)
    {
        pos++;
        if(*c == '/' || *c == '\\')
        {
            foundPos = pos;
        }
        c++;
    }
    return filePath + foundPos;
}

/**
 * @brief Returns whether at least one callback is registered.
 * 
 * @return true 
 * @return false 
 */
bool spLogHelper::callbacksExist()
{
  return (regCallbacks.size() > 0);
}

/**
 * @brief Processes callbacks with given arguments and format settings.
 * 
 * @param level 
 * @param fileName 
 * @param lineNo 
 * @param funcName 
 */
void spLogHelper::handleCallbacks(const splhLevel level, const char *fileName, const uint32_t lineNo, const char *funcName)
{
  // buffer to create log message in
  char logBuffer[spLOGHELPER_MSGBUFFER_LEN];

  // time
  char timeBuffer[50];
  time_t ts = time(nullptr);
  struct tm tm = *localtime(&ts);

  // loop callbacks
  size_t count = regCallbacks.size();
  for (uint32_t index = 0; index < count; index++) {

    // spLogHelper object for this callback
    spLogHelper* pLH = regOwners[index];

    int used = 0;
    timeBuffer[0] = 0;

    for (splhFormat item : pLH->_formatList)
    {
      switch (item)
      {
      case splhFormat::TIME:
        if (pLH->_fTimeFormat.length() > 0)
        {
          strftime(timeBuffer, 50, pLH->_fTimeFormat.c_str(), &tm);
          used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, "[%s]", timeBuffer);
        }
        break;
      
      case splhFormat::LEVEL:
        used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, "[%s]", levelText(level));
        break;
      
      case splhFormat::FILENAME_LINE:
        used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, "[%s:%u]", fileName, lineNo);
        break;
      
      case splhFormat::FILENAME:
        used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, "[%s]", fileName);
        break;
      
      case splhFormat::LINE:
        used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, "[%u]", lineNo);
        break;
      
      case splhFormat::FUNCTION:
        used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, " %s()", funcName);
        break;
      
      default:
        break;
      }

      // max length reached?
      if (used >= spLOGHELPER_MSGBUFFER_LEN)
      {
        break;
      }
    }

    std::string msgFormat;
    if (used > 0)
    {
      msgFormat.append(": ");
    }
    msgFormat.append("%s");
    used += snprintf(logBuffer + used, spLOGHELPER_MSGBUFFER_LEN - used, msgFormat.c_str(), getMsgBufferPointer());

    regCallbacks[index](logBuffer, level, timeBuffer, fileName, lineNo, funcName);

  }

}
