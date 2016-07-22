#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/shared_ptr.hpp>

class CActor;

class CBoard
{
public:
   template <typename MessageType>
   void subscribe(CActor& actor);

   template <typename MessageType>
   void unsubscribe(CActor& actor);

   template <typename MessageType>
   void publish(boost::shared_ptr<MessageType> message);
};

class CActor
{
public:
   CActor(CBoard& board);

   void run();
   void stop();

   template <typename MessageType>
   void post(boost::shared_ptr<MessageType> message);

   void on(boost::shared_ptr<MessageType>& msg);
};

   //void run()
   //{
      //using namespace boost::asio;

      //io_service service;
      //mWork.reset(new io_service::work(service));
      //boost::asio::io_service::work work(service);
   //}

   //void stop()
   //{
      //mWork.reset();
   //}
//private:
   //boost::shared_ptr<boost::asio::io_service::work> mWork;
//};

class Actor
{
   void run();
   void stop();
   void on(shared_ptr<IMessage> msg);
}

   virtual ~Actor() {}

   void run()
   {
   }

   void stop()
   {
   }
};

int main(int, char**)
{
   return 0;
}
