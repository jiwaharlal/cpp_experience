#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>
#include <exception>
#include <string>

class DeviceError: public std::exception
{
public:
   DeviceError(const char* reason);
   ~DeviceError() throw();
   virtual const char* what() const throw();
private:
   std::string mReason;
};

class Device
{
public:
    Device(const char* path, int flags);
    ~Device();
    bool setBaudRate(uint32_t rate);
    int descriptor();
private:
    int m_fd;
};

#endif // __DEVICE_H__
