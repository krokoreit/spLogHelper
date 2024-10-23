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


void functionWithOwnLogging()
{
  // ... whatever the function does
   
  spLogHelper specialLH = spLogHelper();
  specialLH.setMessageFormat({splhFormat::TIME, splhFormat::LEVEL});
  uint32_t id2 = specialLH.registerHandlerCallback(myHandlerFunc2);
  spLOG_I("log from inside this function");

  // more stuff done in the function ...

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
  spLOGF_D("one %s and %d bananas", "apple", 5);
  
  functionWithOwnLogging();

  spLOG_I("after functionWithOwnLogging");


  // ========================================================
  printf("done\n");
  return 0;
}
