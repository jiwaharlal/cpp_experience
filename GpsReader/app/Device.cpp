#include <errno.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <sstream>
#include <string.h>
#include <termio.h>
#include <unistd.h>

#include "Device.hpp"

DeviceError::DeviceError(const char* reason)
   : mReason(reason)
{
}

DeviceError::~DeviceError() throw()
{
}

const char* DeviceError::what() const throw()
{
   return mReason.c_str();
}

static int rate_to_constant(int baudrate) {
#define B(x) case x: return B##x
        switch(baudrate) {
        B(50);     B(75);     B(110);    B(134);    B(150);
        B(200);    B(300);    B(600);    B(1200);   B(1800);
        B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
        B(57600);  B(115200); B(230400); B(460800); B(500000);
        B(576000); B(921600); B(1000000);B(1152000);B(1500000);
    default: return 0;
    }
#undef B
}

Device::Device(const char* path, int flags)
{
   m_fd = open(path, flags);
   if (m_fd == -1)
   {
      throw DeviceError(strerror(errno));
   }
}

Device::~Device()
{
   close(m_fd);
}

bool Device::setBaudRate(uint32_t rate)
{
   struct termios options;
   //struct serial_struct serinfo;
   int speed = 0;

   // if you've entered a standard baud the function below will return it
   speed = rate_to_constant(rate);

   if (speed == 0) 
   {
      std::stringstream ss;
      ss << "Wrong baudrate specified: " << rate;
      throw DeviceError(ss.str().c_str());
   }

   fcntl(m_fd, F_SETFL, 0);
   tcgetattr(m_fd, &options);
   cfsetispeed(&options, speed);
   cfsetospeed(&options, speed);
   cfmakeraw(&options);
   options.c_cflag |= (CLOCAL | CREAD);
   options.c_cflag &= ~CRTSCTS;

   if (tcsetattr(m_fd, TCSANOW, &options) != 0)
   {
      std::stringstream ss;
      ss << "Error setting baudrate " << rate << ": " << strerror(errno);
      throw DeviceError(ss.str().c_str());
   }

   return true;
}

int Device::descriptor()
{
   return m_fd;
}
