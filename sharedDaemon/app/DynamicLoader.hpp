#pragma once

#include <dlfcn.h>
#include <exception>
#include <sstream>
#include <string.h>

#include "CDynamicLoaderException.hpp"
#include "../calculatorCommon/ILogger.hpp"
#include "LoggerInstance.hpp"

template <class LoadedType>
class DynamicLoader
{
public:
   DynamicLoader(const char* libName);
   ~DynamicLoader();

   LoadedType* create();
   void destroy(LoadedType* obj);

private:
   template <class FunctionType> void loadFunc(const char* functionName, FunctionType& func);

private:
   void* mLibraryHandler;

   typedef LoadedType* (*tCreateFn)();
   typedef void (*tDestroyFn)(LoadedType*);
   typedef ILogger* (*tRegisterLoggerFn)(ILogger*);

   tCreateFn mCreate;
   tDestroyFn mDestroy;
};

template <class LoadedType>
DynamicLoader<LoadedType>::DynamicLoader(const char* libName)
   : mLibraryHandler(NULL)
   , mCreate(NULL)
   , mDestroy(NULL)
{
   mLibraryHandler = dlopen(libName, RTLD_NOW);
   if (!mLibraryHandler)
   {
      //throw eLibraryLoadException(libName, dlerror());
      throw CDynamicLoaderException(libName, dlerror());
   }
   loadFunc("create", mCreate);
   loadFunc("destroy", mDestroy);

   tRegisterLoggerFn registerLogger;
   loadFunc("registerLogger", registerLogger);
   registerLogger(LoggerInstance::instance());
}

template <class LoadedType>
DynamicLoader<LoadedType>::~DynamicLoader()
{
   if (mLibraryHandler)
   {
      dlclose(mLibraryHandler);
   }
}

template <class LoadedType>
template <class FunctionType>
void DynamicLoader<LoadedType>::loadFunc(const char* functionName, FunctionType& func)
{
   func = reinterpret_cast<FunctionType>(dlsym(mLibraryHandler, functionName));
   if (!func)
   {
      //throw eFunctionLoadException(functionName);
      throw CDynamicLoaderException(functionName, dlerror());
   }
}

template <class LoadedType>
LoadedType* DynamicLoader<LoadedType>::create()
{
   return mCreate();
}

template <class LoadedType>
void DynamicLoader<LoadedType>::destroy(LoadedType* obj)
{
   mDestroy(obj);
}
