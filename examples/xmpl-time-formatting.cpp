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
  spLOG_FORMAT({splhFormat::TIME});

  spLOG_I("this is with the default time format");

  // object's function to set time format
  spDefaultLogHelper.setTimeFormat("%a %e %b %H:%M:%S");
  spLOG_I("this is with a modified time format");

  // macro to set time format
  spLOG_TIME_FORMAT("%c");
  spLOG_I("this is with another modified time format");

  // get the time format string
  std::string f = spDefaultLogHelper.getTimeFormat();
  spLOGF_I("the current time format string is '%s'", f.c_str());


  // ========================================================
  printf("done\n");
  return 0;
}
