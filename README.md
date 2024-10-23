# spLogHelper Library

This library provides the C++ spLogHelper class as well as various macros for preparing the output of log messages. Whether the messages are indeed displayed on the console, written to a file or otherwise processed, fully depends on the application, which can register callbacks and set log levels to receive messages or not.

The major advantage of spLogHelper is, that code can be written with log messages included and completely independent of how the final application will later use them or what logging system will be implemented. This is particularly useful for writing libraries, which can be used by various applications or other libraries without predefining the policies on how to log.

You have the freedom to use spLogHelper to provide your callback with a fully formatted message (i.e. all you need is a simple println(message)) or to make use of the raw values (i.e. message, time stamp, log level, file name, line number, function name) in order to process or pass them on to the application's logging system.

Therefore you can use spLogHelper as a versatile logging system with many options to control the formatting of the messages, which you can then print to console, write into a file or send via a websocket connection to wherever you may need it.
There are various options to format log messages as well as having the same message content passed in various formats by using multiple spLogHelper objects. This allows you to custom format the log messages for each output target.


Enjoy

&emsp;krokoreit  
&emsp;&emsp;&emsp;<img src="assets/krokoreit-01.svg" width="140"/>



## Usage & API

### spLogHelper Class and Object
Include library:
```cpp
  #include <spLogHelper.h>

  spLOG_REG(myPrintFunc);
  spLOGF_W("remaining memory is %d bytes", 256);

```

There is no need to create an object, as you can use log macros, e.g. spLOGF_E(), which direct the logging to the spDefaultLogHelper object. The spDefaultLogHelper object is automatically created by the library.
Note that using the log macros or calling the logf() function of an spLogHelper object has no effect until at least one callback is registered. 

Similarly to log macros, also all formatting macros will be applied to the spDefaultLogHelper object. Depending on your needs, it may make sense to create additional objects and apply different settings for formats and log levels to them. This enables one log message to be directed with multiple formats and levels to the callbacks registered via these spLogHelper objects.

Create an additional object besides the default one and use the following functions
```cpp
  spLogHelper mySpecialLogHelper;

  mySpecialLogHelper.setTimeFormat("%a %e %b %H:%M:%S");
  mySpecialLogHelper.setMessageFormat({splhFormat::TIME});
  mySpecialLogHelper.registerHandlerCallback(myPrintFunc);
  spLOG_I("my special format");
```
will pass "[Tue 15 Oct 20:48:35]: my special format" as the message argument to myPrintFunc().

</br>

### Callbacks

Define your callbacks to receive the logging information in line with this function:
```cpp
  void myHandlerFunc(const char *message, const splhLevel level, const char *timeString, 
                     const char *fileName, const uint32_t lineNo, const char *funcName);
```
Note that you must use the same sequence and types of arguments, but you are free to use names for the function and arguments as you see fit.

After you have registered your callback, it will receive the logging information to be processed, e.g. print them to console or file, pass to syslog or a logging system used for your application. Typically the message argument will be the only one processed, as it comes formatted with the other elements included. However, you can redefine the message format to exclude any or even all of these elements. Therefore you are free to process them separately as needed.

Callback functions can be registered with the object's function
```cpp
  uint32_t cbID = spDefaultLogHelper.registerHandlerCallback(myHandlerFunc);
```
or via the macro
```cpp
  uint32_t cbID = spLOG_REG(myHandlerFunc);
```

The returned ID can be used to unregister a callback either with the object's function
```cpp
  spDefaultLogHelper.unregisterHandlerCallback(cbID);
```
or via the macro
```cpp
  spLOG_UNREG(cbID);
```

</br>

### Log Levels

The log levels implemented are: ALL, DEBUG, INFO, WARNING, ERROR, CRITICAL and NONE. 

They are defined as splhLevel enum class types (e.g. splhLevel::INFO), and are only needed when you directly call the logf() function.
Furthermore, there is a constant (e.g. SPLH_LOG_LEVEL_INFO) for each level, which is only needed when you want to limit the messages to be below a certain level. This is done by defining 
```cpp
  #define SPLH_LOG_LEVEL_LIMIT    SPLH_LOG_LEVEL_INFO
```
before including the library in your code. 

However, in most cases the log levels are indirectly used via the logging macros (e.g. spLOGF_I & spLOG_I for INFO), where the first letter of each level is used as a distinguishing suffix of the macro. 

</br>

### Log Macros

There are two macros defined for DEBUG, INFO, WARNING, ERROR and CRITICAL log messages.

The first one is for using string formatting like printf()
```cpp
  spLOGF_D("one %s and %d bananas", "apple", 5);  
  spLOGF_W("you are about to get %d errors", 2);
  spLOGF_E("error no %d of %d", 1, 2);
```
See https://en.cppreference.com/w/cpp/io/c/fprintf for details on the format specifiers available.

The second one is for simple string messages without the need for formatting
```cpp
  spLOG_I("this is without args");
  spLOG_E("something broke");
```  

Note that both versions will only be active and in fact compile into your application, when not restricted by defining a SPLH_LOG_LEVEL_LIMIT, as per above. However, even when a limit is defined to e.g. WARNING, you can still emit an INFO message via the logf() function:
```cpp
  spDefaultLogHelper.logf(splhLevel::INFO, __FILE__, __LINE__, __func__, "%s this", "log");
```

</br>

### Message Format

The default setting will create messages with the following format  
- [2024-10-15 20:48:35+0200][DEBUG][xmplcode.cpp:27] main(): this is your formatted message text
- [2024-10-15 20:48:35+0200][ERROR][xmplcode.cpp:34] main(): an error message example

</br>

The format specifiers implemented are: 
  - TIME  -  [2024-10-15 20:48:35+0200] or other defined time format
  - LEVEL  -  [DEBUG] .. [CRITICAL]
  - FILENAME_LINE  -  [file.ext:no]
  - FUNCTION  -  [thisFunc()]  
  - FILENAME  -  [file.ext]  (optional)
  - LINE  -  [no]  (optional)

They are defined as splhFormat enum class types (e.g. splhFormat::TIME), and are used as items in a {formatList} when redefining the message format in the object's function
```cpp
  spDefaultLogHelper.setMessageFormat({splhFormat::LEVEL, splhFormat::TIME, splhFormat::FUNCTION});
```
or via the macro
```cpp
  spLOG_FORMAT({splhFormat::LEVEL, splhFormat::TIME, splhFormat::FUNCTION});
```
This modified setting will create messages with the following format  
- [DEBUG][2024-10-15 20:48:35+0200] main(): this is your formatted message text
- [ERROR][2024-10-15 20:48:35+0200] main(): an error message example

</br>

In order to get plain messages without the splhFormat elements, you can call the function or the macro without any parameters
```cpp
  spDefaultLogHelper.setMessageFormat();
```
or via the macro
```cpp
  spLOG_FORMAT();
```
This setting will create messages as follows  
- this is your formatted message text
- an error message example

Such plain messages may be useful, when passing them on to another log system, which then combines them with other elements to the final log record.  
</br>

### Time Format

In order to modify the splhFormat::TIME part of the log message created, you can use the object's function
```cpp
  spDefaultLogHelper.setTimeFormat("%a %e %b %H:%M:%S");
```
or via the macro
```cpp
  spLOG_TIME_FORMAT("%a %e %b %H:%M:%S");
```
This modified setting will create messages like this  
- [Tue 15 Oct 20:48:35][DEBUG][xmplcode.cpp:27] main(): one apple and 5 bananas
- [Tue 15 Oct 20:48:35][ERROR][xmplcode.cpp:33] main(): an error message example
For information about the time format specifiers, see https://en.cppreference.com/w/cpp/chrono/c/strftime.


</br>

The library limits log messages by default to a length of 240 characters, which may be overridden by placing a #define 
```cpp
  #define spLOGHELPER_MSGBUFFER_LEN  160
```
before including the library in your code. 



</br>


### API

#### Functions
* [registerHandlerCallback()](#registerhandlercallback-function)  
* [unregisterHandlerCallback()](#unregisterhandlercallback-function)  
* [getTimeFormat()](#gettimeformat-function)  
* [setTimeFormat()](#settimeformat-function)  
* [setMessageFormat()](#setmessageformat-function)  
* [logf()](#logf-function)  

#### registerHandlerCallback() Function
```cpp
  uint32_t registerHandlerCallback(splhHandlerCallback callback);
```
Registers a callback function, which will be invoked each time logf() or a log macro is used.

The return value is an unique ID for this registration, which can be used to unregister the function. Any functions registered with a specific spLogHelper object stay active during the life time of that object. Therefore, if a spLogHelper object is created and used to register callbacks within one function, then subsequent logging via this registration is only active within such function.


<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### unregisterHandlerCallback() Function
```cpp
  void unregisterHandlerCallback(uint32_t id);
```
Deletes a previously registered callback function from the registry. Note that the ID refers to the registration made (not the callback) and may already be removed. However, no need to check for and confirm the active status before calling unregisterHandlerCallback().

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### getTimeFormat() Function
```cpp
  std::string getTimeFormat();
```
Returns the current format string used to format the splhFormat::TIME part of the log message.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### setTimeFormat() Function
```cpp
  void setTimeFormat(std::string formatString);
```
Sets the format string used to format the splhFormat::TIME part of the log message created. Formatting is done internally with the strftime() function. For information about the time format specifiers, see https://en.cppreference.com/w/cpp/chrono/c/strftime.


<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### setMessageFormat() Function
```cpp
  void setMessageFormat(std::initializer_list<splhFormat> formatList = {});
```
Sets the log message format to the sequence of splhFormat elements specified. In order to modify the format, you can pass a list with the desired splhFormat elements to setMessageFormat(). Calling this function without a list or with an empty list will create plain messages without any splhFormat elements included. 

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


#### logf() Function
```cpp
  void logf(splhLevel level, const char *fileName, const uint32_t lineNo, const char *funcName, const char *format, Vs... args);
```
 Creates a log message and pass it with the additional information to the registered callback functions. In most cases it will be more convenient to call any of the log macros, as they automatically feed the first four arguments in a call to logf(). Nevertheless, if you call logf() directly, make use of __FILE__, __LINE__, __func__ macros / variable provided by your compiler.

 Note that spLogHelper uses C++ printf() specification, so you must provide a format string followed by one or more arguments (see https://en.cppreference.com/w/cpp/io/c/fprintf for details on the format specifiers available). Alternatively, the final variadic arguments can be omitted and instead a regular string passed to format.

<div style="text-align: right"><a href="#functions">&#8679; back up to list of functions</a></div>


</br>

## License
MIT license  
Copyright &copy; 2024 by krokoreit
