#pragma once

class ILogger
{
public:
   virtual void log_info(const char* message) = 0;
   virtual void log_error(const char* message) = 0;
};
