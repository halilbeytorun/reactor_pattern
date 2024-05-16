# Reactor Pattern

### Abstract

Implementation of Reactor Pattern paper written by Douglas C. Schmidt.

The main code is the reactor library consisting three main actors: 
- **InitiationDispatcher:** Reactor class. Responsible for holding EventHandlers inside of it. Mutex protected. After events are registered to it, calling `handle_events(int)` function will wait on the `handles` given by the Concrete EventHandler classes. After one of the handles are `triggered`, Reactor class will wake up and call the corresponding handle_event function of the Concrete Event Handler class.
- **LoggingAcceptor:** One of the two implemented Concrete EventHandler class. This class is responsible for accepting new connections sent from potantial clients. After accepting the connections, it will create a LoggingHandler Concrete class to read from these connections.
- **LoggingHandler:** One of the two implemented Concrete EventHandler class. This class is responsible for reading the messages sent from client sockets or close them if client side closed it.  


### Platform
Works in Linux platform. Since platform dependent sockets and poll function is used.

