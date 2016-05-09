#include <stdlib.h>
//#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <termio.h>
//#include <linux/serial.h>
#include <iostream>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

#include "SentenceExtractor.hpp"

namespace po = boost::program_options;

//static int rate_to_constant(int baudrate) {
//#define B(x) case x: return B##x
        //switch(baudrate) {
        //B(50);     B(75);     B(110);    B(134);    B(150);
        //B(200);    B(300);    B(600);    B(1200);   B(1800);
        //B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
        //B(57600);  B(115200); B(230400); B(460800); B(500000);
        //B(576000); B(921600); B(1000000);B(1152000);B(1500000);
    //default: return 0;
    //}
//#undef B
//}

struct AppSettings
{
   std::string device;
   uint32_t baudRate;
};

AppSettings handleArgs(int argc, char** argv)
{
   AppSettings settings;

   po::options_description desc("Available options");
   desc.add_options()
      ("help,h", "Print help message")
      ("dev,d", po::value<std::string>(&settings.device)->default_value("/dev/ttyUSB0"),
         "Device or file to read")
      ("baudrate,b", po::value<uint32_t>(&settings.baudRate)->default_value(38400),
         "Baud rate");

   po::variables_map vm;
   try
   {
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);
   }
   catch (const po::error& e)
   {
      std::cout << e.what() << std::endl;
      std::cout << desc << std::endl;
      exit(EXIT_FAILURE);
   }

   if (vm.count("help"))
   {
      std::cout << desc << std::endl;
      exit(EXIT_SUCCESS);
   }

   return settings;
}

bool& isStop()
{
   static bool result = false;
   return result;
}

void interuptionHandler(int)
{
   isStop() = true;
   //executionCondition.notify_one();
}

void setupInteruptionHandler()
{
   struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = interuptionHandler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}

class FdHolder
{
public:
   FdHolder(int descriptor) : fd(descriptor) {}
   ~FdHolder() { close(fd); }
   int fd;
};

int main(int argc, char** argv) 
{
   AppSettings settings = handleArgs(argc, argv);

   std::cout << "Device: " << settings.device << std::endl
            << "Baudrate: " << settings.baudRate << std::endl;

   setupInteruptionHandler();

   int fd = open(settings.device.c_str(), O_RDONLY | O_NOCTTY);
   if (fd < 0)
   {
      std::cout << "Error opening " << settings.device << " : " << strerror(errno);
      return 1;
   }
   FdHolder fdHolder(fd);

   char buf[512];
   int readCount;

   SentenceExtractor se;
   do
   {
      readCount = read(fd, buf, sizeof(buf) - 1);
      if (readCount > 0)
      {
         buf[readCount] = 0;
         se.pushBytes(buf);
         std::string sentence;
         do {
            sentence = se.tryPopSentence();
            if (!sentence.empty())
            {
               std::cout << sentence << std::endl;
               continue;
            }
         } while (!sentence.empty());
      }
      std::cout << "readCount " << readCount << std::endl << "isStop() " << isStop() << std::endl;
   } while (readCount > 0 && !isStop());

   return 0;

    //struct termios options;
    //struct serial_struct serinfo;
    //int fd;
    //int speed = 0;
    //int rate = 625000;

    //[> Open and configure serial port <]
    //if ((fd = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY)) == -1)
    //{
        //return -1;
    //}

    //// if you've entered a standard baud the function below will return it
    //speed = rate_to_constant(rate);

    //if (speed == 0) {
        //[> Custom divisor <]
        //serinfo.reserved_char[0] = 0;
        //if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
            //return -1;
        //serinfo.flags &= ~ASYNC_SPD_MASK;
        //serinfo.flags |= ASYNC_SPD_CUST;
        //serinfo.custom_divisor = (serinfo.baud_base + (rate / 2)) / rate;
        //if (serinfo.custom_divisor < 1)
            //serinfo.custom_divisor = 1;
        //if (ioctl(fd, TIOCSSERIAL, &serinfo) < 0)
            //return -1;
        //if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
            //return -1;
        //if (serinfo.custom_divisor * rate != serinfo.baud_base) {
            //warnx("actual baudrate is %d / %d = %f",
                  //serinfo.baud_base, serinfo.custom_divisor,
                  //(float)serinfo.baud_base / serinfo.custom_divisor);
        //}
        
       //std::cout << "Custom boud rate is not supported" << std::endl;
       //return 0;
    //}

    //fcntl(fd, F_SETFL, 0);
    //tcgetattr(fd, &options);
    //cfsetispeed(&options, speed ?: B38400);
    //cfsetospeed(&options, speed ?: B38400);
    //cfmakeraw(&options);
    //options.c_cflag |= (CLOCAL | CREAD);
    //options.c_cflag &= ~CRTSCTS;

    //if (tcsetattr(fd, TCSANOW, &options) != 0)
    //{
        ////return -1;
    //}


    ////return fd;

    //char ping_cmd[] = {2,1};
    //char ping_rec[7];

    //write(fd,&ping_cmd,sizeof(ping_cmd));
    //read(fd,&ping_rec,sizeof(ping_rec));

    //int i;
    //for (i = 0; i < static_cast<int>(sizeof(ping_rec)); i++)
    //{
        //printf("%d ",ping_rec[i]);
    //}


    //close(fd);
    //return 0;
}
