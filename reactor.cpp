
enum Event_Type
// = TITLE
//    Types of events handled by the
//    Initiation_Dispatcher.
//
// = DESCRIPTION
//    These values are powers of two so
//    their bits can be efficiently ‘‘or’d’’
//    together to form composite values.
{
   ACCEPT_EVENT = 01,
   READ_EVENT = 02,
   WRITE_EVENT = 04,
   TIMEOUT_EVENT = 010,
   SIGNAL_EVENT = 020,
   CLOSE_EVENT = 040
};

class Event_Handler;

class Initiation_Dispatcher
// = TITLE
//     Demultiplex and dispatch Event_Handlers
{
public:
   // Register an Event_Handler of a particular
   // Event_Type (e.g., READ_EVENT, ACCEPT_EVENT,
   // etc.).
   int register_handler(Event_Handler *eh, Event_Type et);

   // Remove an Event_Handler of a particular
   // Event_Type.
   int remove_handler(Event_Handler *eh, Event_Type et);

   // Entry point into the reactive event loop.
   int handle_events(Time_Value *timeout = 0);
};

class Event_Handler
// = TITLE
//   Abstract base class that serves as the
//   target of the Initiation_Dispatcher.
{
public:
   // Hook method that is called back by the
   // Initiation_Dispatcher to handle events.
   virtual int handle_event(Event_Type et) = 0;

   // Hook method that returns the underlying
   // I/O Handle.
   virtual Handle get_handle() const = 0;
};

class Logging_Acceptor : public Event_Handler
// = TITLE
//    Handles client connection requests.
{
public:
   // Initialize the acceptor_ endpoint and
   // register with the Initiation Dispatcher.
   Logging_Acceptor(const INET_Addr &addr);

   // Factory method that accepts a new
   // SOCK_Stream connection and creates a
   // Logging_Handler object to handle logging
   // records sent using the connection.
   virtual void handle_event(Event_Type et);

   // Get the I/O Handle (called by the
   // Initiation Dispatcher when
   // Logging_Acceptor is registered).
   virtual HANDLE get_handle() const
   {
      return acceptor_.get_handle();
   }
private:
   // Socket factory that accepts client
   // connections.
   SOCK_Acceptor acceptor_;
};

Logging_Acceptor::Logging_Acceptor(const INET_Addr &addr)
   : acceptor_(addr)
{
   // Register acceptor with the Initiation
   // Dispatcher, which "double dispatches"
   // the Logging_Acceptor::get_handle() method
   // to obtain the HANDLE.
   Initiation_Dispatcher::instance()->register_handler(this, ACCEPT_EVENT);
}

void Logging_Acceptor::handle_event(Event_Type et)
{
   // Can only be called for an ACCEPT event.
   assert(et == ACCEPT_EVENT);
   SOCK_Stream new_connection;

   // Accept the connection.
   acceptor_.accept(new_connection);

   // Create a new Logging Handler.
   Logging_Handler *handler = new Logging_Handler(new_connection);
}

class Logging_Handler : public Event_Handler
// = TITLE
//    Receive and process logging records
//    sent by a client application.
{
public:
   // Initialize the client stream.
   Logging_Handler(SOCK_Stream &cs);

   // Hook method that handles the reception
   // of logging records from clients.
   virtual void handle_event(Event_Type et);

   // Get the I/O Handle (called by the
   // Initiation Dispatcher when
   // Logging_Handler is registered).
   virtual HANDLE get_handle() const
   {
      return peer_stream_.get_handle();
   }
private:
   // Receives logging records from a client.
   SOCK_Stream peer_stream_;
};

Logging_Handler::Logging_Handler(SOCK_Stream &cs)
   : peer_stream_(cs)
{
   // Register with the dispatcher for
   // READ events.
   Initiation_Dispatcher::instance()->register_handler(this, READ_EVENT);
}

void Logging_Handler::handle_event(Event_Type et)
{
   if (et == READ_EVENT)
   {
      Log_Record log_record;
      peer_stream_.recv((void *) &log_record, sizeof log_record);

      // Write logging record to standard output.
      log_record.write(STDOUT);
   }
   else if (et == CLOSE_EVENT)
   {
      peer_stream_.close ();
      delete (void *) this;
   }
}

// Server port number.
const u_short PORT = 10000;

int main(void)
{
   // Logging server port number.
   INET_Addr server_addr (PORT);

   // Initialize logging server endpoint and
   // register with the Initiation_Dispatcher.
   Logging_Acceptor la (server_addr);

   // Main event loop that handles client
   // logging records and connection requests.
   for (;;)
   {
      Initiation_Dispatcher::instance()->handle_events();
   }

   /* NOTREACHED */
   return 0;
}
