#include <iostream>

#include <boost/random.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/program_options/options_description.hpp>
#include <string>
#include <stdio.h>
#include <unistd.h>

//shm_open
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

#include "../calculator/ICalculator.hpp"
#include "../calculator/CalculatorFactory.hpp"

namespace ip = boost::interprocess;
namespace po = boost::program_options;

const char* SHM_NAME = "carPositionProvider2_shared_memory_object";

boost::condition_variable executionCondition;
ip::shared_memory_object shm;

bool tryCreateSharedID()
{
   try
   {
      ip::shared_memory_object newShm(ip::create_only, SHM_NAME, ip::read_write);
      newShm.truncate( sizeof(int));
      ip::mapping_handle_t mappingHandle = newShm.get_mapping_handle();
      int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_WRITE, MAP_SHARED, mappingHandle.handle, 0));
      if ( pId == MAP_FAILED )
      {
         std::cout << "Mapping error" << std::endl;
         return false;
      }
      *pId = getpid();
      if ( munmap(pId, sizeof(int)) != 0 )
      {
         std::cout << "Unmapping error" << std::endl;
         return false;
      }
      newShm.swap(shm);
   } 
   catch(ip::interprocess_exception& e)
   {
      // executable is already running
      std::cout << e.what() << std::endl;
      return false; 
   }

   return true;
}

bool tryStopService()
{
   try 
   {
      ip::shared_memory_object newShm(ip::open_only, SHM_NAME, ip::read_only);
      ip::offset_t size;
      newShm.get_size(size);
      if ( size != sizeof(int) )
      {
         std::cout << "Wrong size of shared momory object." << std::endl;
         return false;
      }
      ip::mapping_handle_t mappingHandle = newShm.get_mapping_handle();
      int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, mappingHandle.handle, 0));
      if ( pId == MAP_FAILED )
      {
         std::cout << "Mapping error" << std::endl;
         return false;
      }
      
      std::cout << "Process id is " << *pId << ", sending termination signal..." << std::endl;
      kill(*pId, SIGINT);
      std::cout << "Done." << std::endl;

      if ( munmap(pId, sizeof(int)) != 0 )
      {
         std::cout << "Unmapping error" << std::endl;
         // since program will terminate on a next step, not critical
      }
   }
   catch(ip::interprocess_exception& e)
   {
      std::cout << e.what() << std::endl;
      return false;
   }
   catch(...)
   {
      std::cout << "Unknown error" << std::endl;
      return false;
   }

   return true;
}

bool handleArguments(int argc, char** argv, po::variables_map& vm)
{
   po::options_description desc("Valid options");

   desc.add_options()
         ("help,?", "Produce help message")
         ("daemon,d", "Start a deamon")
         ("stop,k", "Stop daemon")
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
      return false;
   }

   if (vm.count("help"))
   {
      std::cout << desc << std::endl;
      return false;
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

void runCalculation()
{
   boost::mutex executionMutex;
   boost::unique_lock<boost::mutex> lock(executionMutex);

   boost::random::mt19937 rng;
   boost::random::uniform_int_distribution<> rndInt(-1000, 1000);

   ICalculator* calc = CalculatorFactory::create();

   while (!isStop())
   {
      int v1 = rndInt(rng);
      int v2 = rndInt(rng);
      int result = calc->add( v1, v2 );

      std::cout << v1 << " + " << v2 << " = " << result << std::endl;

      (void) result;

      executionCondition.wait_for(lock, boost::chrono::milliseconds(1000), &isStop);
   }
}

int main(int argc, char** argv)
{
   po::variables_map vm;
   handleArguments(argc, argv, vm);

   if (vm.count("daemon")) // run daemon 
   {
      if (!startDaemon())
      {
         exit(EXIT_FAILURE);
      } 
   }
   else if (vm.count("stop")) // stop daemon
   {
      if ( tryStopService() )
      {
         std::cout << "Service stopped." << std::endl;
         exit(EXIT_SUCCESS);
      }
      else
      {
         std::cout << "Could not stop service." << std::endl;
         exit(EXIT_FAILURE);
      }
   }
   else
   {
      std::cout << "Running in console mode" << std::endl;
   }

   if ( tryCreateSharedID() )
   {
      std::cout << "ID created." << std::endl;
   }
   else
   {
      std::cout << "Could not create ID, seems like the service is already running." << std::endl;
      exit(EXIT_FAILURE);
   }

   setupInteruptionHandler();

   runCalculation();

   try
   {
      shm.remove(SHM_NAME);
      std::cout << "Shared memory object removed, terminating program" << std::endl;
   }
   catch(...)
   {
      std::cout << "Error removing shared memory object" << std::endl;
   }

   return 0;
}
