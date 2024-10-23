/**
 * example code for spLogHelper library
 * 
 * 
 */

#include <filesystem>

#include <useLib1.h>


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

  // log only happens with callback registered
  spLOG_REG(myHandlerFunc1);

  // calling library function
  useFunc1();

  // ========================================================
  printf("done\n");
  return 0;
}
