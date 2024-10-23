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

  // debug log macro
  spLOGF_D("one %s and %d bananas", "apple", 5);
  
  // info log macro
  spLOG_I("this is without args");

  // warning log macro
  spLOGF_W("you are about to get %d errors", 2);

  // error log macro
  spLOGF_E("error no %d of %d", 1, 2);

  // error log macro - simple string
  spLOG_E("the second error");

  // critical log macro
  spLOGF_C("one critical %s", "situation");

  // object's log function with format string & argument
  spDefaultLogHelper.logf(splhLevel::INFO, __FILE__, __LINE__, __FUNCTION__, "%s here", "log");

  // object's log function with simple string
  spDefaultLogHelper.logf(splhLevel::INFO, __FILE__, __LINE__, __FUNCTION__, "simple string (not format string followed by arguments)");


  // ========================================================
  printf("done\n");
  return 0;
}
