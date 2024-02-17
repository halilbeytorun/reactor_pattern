#ifndef EventTypeHeader
#define EventTypeHeader

/// @brief Types of event handled by the InitiationDispatcher
/// @details These values are power of two, so their bits can be efficiently ''or'd'' together to form composite values
enum EventType
{
    ACCEPT_EVENT = 0x1,
    READ_EVENT = 0x2,
    WRITE_EVENT = 0x4,
    TIMEOUT_EVENT = 0x8,
    SIGNAL_EVENT = 0x10,
    CLOSE_EVENT = 0x20
};

#endif