#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_index.hpp>
#include <map>
#include <set>
#include <string>

#include "THandlerBase.hpp"

struct HandlerListBase {};

template <typename MessageType>
struct HandlerList: public HandlerListBase
{
   std::set<THandlerBase<MessageType> > mHandlers;
};

class CBoard
{
public:
   template <typename MessageType>
   void subscribe(THandlerBase<MessageType>& handler)
   {
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      boost::shared_ptr<HandlerListBase> hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new HandlerList<MessageType>());
      }
      std::set<THandlerBase<MessageType> >& handlers = static_cast<HandlerList<MessageType>* >(hl.get())->mHandlers;
      handlers.insert(handler);
   }

   template <typename MessageType>
   void unsubscribe(THandlerBase<MessageType>& handler);

   template <typename MessageType>
   void publish(MessageType message)
   {
      typedef std::set<THandlerBase<MessageType> > tHandlerSet;
      std::string typeName = boost::typeindex::type_id<MessageType>().raw_name();
      boost::shared_ptr<HandlerListBase> hl = mHandlerMap[typeName];
      if (!hl)
      {
         hl.reset(new HandlerList<MessageType>());
      }
      std::set<THandlerBase<MessageType> >& handlers = static_cast<HandlerList<MessageType>* >(hl.get())->mHandlers;
      BOOST_FOREACH(typename tHandlerSet::reference handler, handlers)
      {
         handler.post(message);
      }
   }
private:
   typedef std::map<std::string, boost::shared_ptr<HandlerListBase> > tHandlerMap;
   tHandlerMap mHandlerMap;
};
