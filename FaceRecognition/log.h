#pragma once
#include "Poco/Logger.h"
//using  Poco::Logger;                    // for global decorator 

namespace Loggering {
#define logger_handle (Poco::Logger::get("logger"))

	void Logger_initiation();  // initiation
	void Setup_logger();       // init only once 
	void log_information(char *format, ...);
	void log_error(char *format, ...);

}