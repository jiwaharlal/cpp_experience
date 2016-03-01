#pragma once

#include <dlfcn.h>
#include <exception>
#include <sstream>
#include <string.h>

class eLibraryLoadException: public std::exception
{
public:
   eLibraryLoadException(const char* libName, const char* reason = NULL)
   {
      std::stringstream ss;
      ss << "Error loading library: " << libName;
      if (reason && strlen(reason))
      {
         ss << " Reason: " << reason;
      }
      mWhat = ss.str();
   }

   virtual ~eLibraryLoadException() throw() {}

   virtual const char* what() const throw()
   {
      return mWhat.c_str();
   }

private:
   std::string mWhat;
};

class eFunctionLoadException: public std::exception
{
public:
   eFunctionLoadException(const char* functionName)
   {
      std::stringstream ss;
      ss << "Error loading function: " << functionName;
      mWhat = ss.str();
   }

   virtual ~eFunctionLoadException() throw() {}

   virtual const char* what() const throw()
   {
      return mWhat.c_str();
   }

private:
   std::string mWhat;
};

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
      throw eLibraryLoadException(libName, dlerror());
   }
   loadFunc("create", mCreate);
   loadFunc("destroy", mDestroy);
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
      throw eFunctionLoadException(functionName);
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
