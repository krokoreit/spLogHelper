/**
 * example code for spLogHelper library
 * 
 * 
 */

#include <filesystem>
#include <spLogHelper.h>


void myHandlerFunc1(const char *message, const splhLevel level, const char *timeString, 
                    const char *fileName, const uint32_t lineNo, const char *funcName)
{
  printf("1: %s\n", message);
}
void myHandlerFunc2(const char *message, const splhLevel level, const char *timeString, 
                    const char *fileName, const uint32_t lineNo, const char *funcName)
{
  printf("2: %s\n", message);
}


/**
 * @brief our main function
 * 
 */
int main(int argc, char *argv[])
{
  std::string a = argv[0];
  printf("running %s\n", a.substr(a.rfind(std::filesystem::path::preferred_separator) + 1).c_str());
  // ========================================================

  uint32_t id1 = spLOG_REG(myHandlerFunc1);

  spLOG_I("this is with default formatting");

  // object's function to set format
  spDefaultLogHelper.setMessageFormat({splhFormat::FILENAME, splhFormat::LEVEL, splhFormat::TIME});
  spLOG_I("this is with modified format specs");
  spDefaultLogHelper.setMessageFormat();
  spLOG_I("this is without format specs");

  // macro to set format
  spLOG_FORMAT({splhFormat::LEVEL, splhFormat::TIME, splhFormat::FUNCTION});
  spLOG_I("this is with modified format specs set with the spLOG_FORMAT macro");
  spLOG_FORMAT();
  spLOG_I("this is also without format specs (setting with macro)");


  // ========================================================
  printf("done\n");
  return 0;
}
