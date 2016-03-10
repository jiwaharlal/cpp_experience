#include <boost/chrono.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/random.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../calculatorCommon/ICalculator.hpp"
#include "DynamicLoader.hpp"
#include "CUniqueInstance.hpp"
#include "LoggerInstance.hpp"

//#define CALCULATOR_TEST_VERSION 42
//#define CALCULATOR_REAL_VERSION 0

namespace ip = boost::interprocess;
namespace po = boost::program_options;

const char* SHM_NAME = "carPositionProvider2";
const char* CALC_LIB_NAME = "libcalculator.so";

boost::condition_variable executionCondition;
bool isUseTestLib = false;

bool handleArguments(int argc, char** argv, po::variables_map& vm)
{
   po::options_description desc("Valid options");

   srand(time(NULL));
   std::stringstream ss;
   ss << rand();
   std::string logHeader(ss.str());

   desc.add_options()
         ("help,?", "Produce help message")
         ("daemon,d", "Start a deamon")
         ("stop,k", "Stop daemon")
         ("test,t", "Use test library version")
         ("header,h", po::value<std::string>(&logHeader)->default_value(logHeader), "Header to use in log messages")
         ;

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

   if (vm.count("test"))
   {
      isUseTestLib = true;
   }

   if (vm.count("header"))
   {
      LoggerInstance::instance()->setHeader(logHeader.c_str());
   }

   return true;
}

bool startDaemon()
{
   int pid = fork();
   switch ( pid )
   {
      case -1: // failed to create a deamon
         std::cout << "Error creating daemon" << std::endl;
         return false;
      case 0: // daemon branch
         setsid();

         close(STDIN_FILENO);
         close(STDOUT_FILENO);
         close(STDERR_FILENO);
         return true;
      default: // parent branch
         std::cout << "Daemon started, terminating parent process.." << std::endl;
         exit(EXIT_SUCCESS);
   };
}

bool& isStop()
{
   static bool sIsStop = false;
   return sIsStop;
}

void interuptHandler(int s)
{
   (void) s;
   std::cout << "Interuption handler, notifying interuption..." << std::endl;

   isStop() = true;
   executionCondition.notify_one();
}

void setupInteruptionHandler()
{
   struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = interuptHandler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);
}

std::string calculatorLibFullName()
{
   const char* libVersion = (isUseTestLib ? CALCULATOR_TEST_VERSION : CALCULATOR_REAL_VERSION);
   std::stringstream ss;
   ss << "./lib/" << CALC_LIB_NAME << "." << libVersion;

   return ss.str();
}

void runCalculation()
{
   try
   {
      DynamicLoader<ICalculator> loader(calculatorLibFullName().c_str());
      ICalculator* calc = loader.create();

      boost::mutex executionMutex;
      boost::unique_lock<boost::mutex> lock(executionMutex);

      boost::random::mt19937 rng;
      boost::random::uniform_int_distribution<> rndInt(-1000, 1000);

      while (!isStop())
      {
         int v1 = rndInt(rng);
         int v2 = rndInt(rng);
         int result = calc->add(v1, v2);

         std::cout << v1 << " + " << v2 << " = " << result << std::endl;

         (void) result;

         executionCondition.wait_for(lock, boost::chrono::milliseconds(1000), &isStop);
      }
   }
   catch (const std::exception& e)
   {
      std::cout << "Error loading calculator from library: " << e.what() << std::endl;
   }
   catch (...)
   {
      std::cout << "Unknown error while loading library or running a calculation." << std::endl;
   }
}

int main(int argc, char** argv)
{
   std::cout << "in main()" << std::endl;
   po::variables_map vm;
   handleArguments(argc, argv, vm);

   if (vm.count("stop")) // stop daemon
   {
      //boost::scoped_ptr<CUniqueInstance>
         //handler(CInstanceSynchronizer::getHandler("carPositioner"));
      int processId = CUniqueInstance::getProcessId(SHM_NAME);

      //CInstanceSynchronizer* sync = CInstanceSynchronizer::tryOpen("carPositioner");
      if (processId != -1)
      {
         if (kill(processId, SIGINT) != 0)
         {
            std::cout << "Error sending interuption signal. " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
         }
         else
         {
            std::cout << "Interuption signal sent." << std::endl;
            exit(EXIT_SUCCESS);
         }
      }
      else
      {
         std::cout << "Seems like service is not running." << std::endl;
         exit(EXIT_FAILURE);
      }
   }

   boost::shared_ptr<CUniqueInstance> instance(CUniqueInstance::create(SHM_NAME));

   //if (!instance)
   //{
      //std::cout << "Could not create ID, seems like the service is already running." << std::endl;
      //exit(EXIT_FAILURE);
   //}

   //instance->setAttached(false);
   if (vm.count("daemon")) // run daemon
   {
      if (!startDaemon())
      {
         exit(EXIT_FAILURE);
      }
   }
   else
   {
      std::cout << "Running in console mode" << std::endl;
   }

   //instance->setAttached(true);
   //instance->saveProcessId();
   setupInteruptionHandler();
   std::cout << "Start calculation" << std::endl;
   runCalculation();

   return 0;
}
