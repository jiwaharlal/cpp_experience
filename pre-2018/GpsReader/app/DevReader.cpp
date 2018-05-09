#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Device.hpp"
#include "minmea/minmea.h"
#include "SentenceExtractor.hpp"

namespace po = boost::program_options;

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

   try
   {
      Device dev(settings.device.c_str(), O_RDONLY | O_NOCTTY);
      struct stat sb;

      if (stat(settings.device.c_str(), &sb) == -1) {
         throw DeviceError("Could not get stat for device");
         exit(EXIT_FAILURE);
      }

      if ((sb.st_mode & S_IFMT) == S_IFCHR)
      {
         dev.setBaudRate(settings.baudRate);
      }

      char buf[512];
      int readCount;

      SentenceExtractor se;
      do
      {
         readCount = read(dev.descriptor(), buf, sizeof(buf) - 1);
         if (readCount > 0)
         {
            buf[readCount] = 0;

            //std::cout << buf << std::endl;

            se.pushBytes(buf);
            std::string sentence;
            do {
               sentence = se.tryPopSentence();
               if (!sentence.empty())
               {
                  if (minmea_sentence_id(sentence.c_str(), false) == MINMEA_SENTENCE_RMC)
                  {
                     minmea_sentence_rmc frame;
                     if (minmea_parse_rmc(&frame, sentence.c_str()))
                     {
                        double lat = minmea_tocoord(&(frame.latitude));
                        double lon = minmea_tocoord(&(frame.longitude));
                        std::cout << "Lat " << std::fixed << std::setprecision(5) << lat << " Lon " << lon << std::endl;
                     }
                  }
                  //std::cout << sentence << std::endl;
                  continue;
               }
            } while (!sentence.empty());
         }
         //std::cout << "readCount " << readCount << std::endl << "isStop() " << isStop() << std::endl;
      } while (readCount > 0 && !isStop());
   }
   catch(DeviceError& e)
   {
      std::cout << e.what() << std::endl;
      return 1;
   }
   catch(...)
   {
      std::cout << "Unknown error" << std::endl;
      return 1;
   }

   return 0;
}
