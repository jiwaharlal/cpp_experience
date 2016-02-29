#include <iostream>

#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/program_options/options_description.hpp>
#include <string>
#include <stdio.h>

//shm_open
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

namespace ip = boost::interprocess;
namespace po = boost::program_options;
const char* SHM_NAME = "carPositionProvider2_shared_memory_object";

ip::shared_memory_object shm;

bool isRunning()
{
   int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR | O_EXCL, 0);
   return fd == -1;
}

bool tryCreateSharedID()
{
   try
   {
      //ip::shared_memory_object newShm(ip::create_only, SHM_NAME, ip::read_write);
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
         std::cout << "Wrong size" << std::endl;
         return false;
      }
      ip::mapping_handle_t mappingHandle = newShm.get_mapping_handle();
      int* pId = reinterpret_cast<int*>(mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, mappingHandle.handle, 0));
      if ( pId == MAP_FAILED )
      {
         std::cout << "Mapping error" << std::endl;
         return false;
      }
      
      std::cout << "Process id is " << *pId << std::endl;
      std::cout << "Sending terminate signal..." << std::endl;
      kill(*pId, SIGINT);
      std::cout << "Done." << std::endl;

      if ( munmap(pId, sizeof(int)) != 0 )
      {
         std::cout << "Unmapping error" << std::endl;
         return false;
      }

      //pid_t pid;
      newShm.swap(shm);
   }
   catch(ip::interprocess_exception& e)
   {
      std::cout << e.what() << std::endl;
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

int main(int argc, char** argv)
{

   po::variables_map vm;
   handleArguments(argc, argv, vm);

   bool isRunning = false;
   bool isDaemon = false;
   //if (isRunning)
   //{
      ////isDaemon = isDaemon();
      //std::cout << "Already running" << std::endl;
   //}

   if (vm.count("daemon")) // run daemon 
   {
      if ( tryCreateSharedID() )
      {
         std::cout << "ID created" << std::endl;
      }
      else
      {
         std::cout << "Could not create ID" << std::endl;
      }
   }
   else if (vm.count("stop")) // stop daemon
   {
      if ( tryStopService() )
      {
         std::cout << "Service stopped" << std::endl;
      }
      else
      {
         std::cout << "Could not stop" << std::endl;
      }
   }
   else // run in console
   {
      if ( isRunning )
      {
         //report already running
      }
      else
      {
         // run
      }
   }

   getchar();
   try
   {
      shm.remove(SHM_NAME);
   }
   catch(...)
   {
      std::cout << "Error removing shared memory object" << std::endl;
   }

   return 0;



   //isRunning = isProcessRunning(shm);
   if ( isRunning )
   {
      std::cout << "Already runnging" << std::endl;
      return 0;
   }
   else
   {
      std::cout << "Not running yet" << std::endl;
      shm.truncate(sizeof(int));
   }

   if (vm.count("daemon"))
   {
      // create a daemon
   }

   if (vm.count("kill"))
   {
      // kill a daemon
   }     

   getchar();
   // create Ctrl-C handler, run from console
   
   ip::shared_memory_object::remove(SHM_NAME);

   return 0;
}
