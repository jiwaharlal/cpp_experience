#pragma once

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_index.hpp>
#include <map>
#include <set>
#include <string>
#include <iostream>

#include "THandlerBase.hpp"

struct HandlerListBase {};

template <typename MessageType>
struct THandlerList: public HandlerListBase
{
   std::set<THandlerBase<MessageType>* > mHandlers;
};

class CBoard
{
public:
   template <typename MessageType>
   void subscribe(THandlerBase<MessageType>* handler)
   {
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      boost::shared_ptr<HandlerListBase>& hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new THandlerList<MessageType>());
      }
      std::set<THandlerBase<MessageType>* >& handlers = static_cast<THandlerList<MessageType>* >(hl.get())->mHandlers;
      handlers.insert(handler);
   }

   template <typename MessageType>
   void unsubscribe(THandlerBase<MessageType>& handler)
   {
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      tHandlerMap::iterator it = mHandlerMap.find(typeName);
      if (it == mHandlerMap.end())
      {
         return;
      }
      std::set<THandlerBase<MessageType>* >& handlers = static_cast<THandlerList<MessageType>* >(it->second)->mHandlers;
      handlers->erase(handler);
   }

   template <typename MessageType>
   void publish(MessageType message)
   {
      typedef std::set<THandlerBase<MessageType>* > tHandlerSet;

      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();

      tHandlerMap::iterator it = mHandlerMap.find(typeName);
      if (it == mHandlerMap.end())
      {
         std::cout << "No handler for message of type " << boost::typeindex::type_id<MessageType>().pretty_name() << std::endl;
         return;
      }

      //std::set<THandlerBase<MessageType>* >& handlers = static_cast<THandlerList<MessageType>* >(it->second.get())->mHandlers;
      tHandlerSet& handlers = static_cast<THandlerList<MessageType>* >(it->second.get())->mHandlers;
      //BOOST_FOREACH(typename tHandlerSet::reference handler, handlers)
      for (typename tHandlerSet::iterator it = handlers.begin(); it != handlers.end(); ++it)
      {
         (*it)->post(message);
         //handler->post(message);
      }
   }

private:
   typedef std::map<std::string, boost::shared_ptr<HandlerListBase> > tHandlerMap;
   tHandlerMap mHandlerMap;
};
