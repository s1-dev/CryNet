#ifndef ACTION_TYPE_HPP
#define ACTION_TYPE_HPP

// Enum class representing command types
enum class ActionType {
    PING,
    ENCRYPT,
    REPORT,
    FORK_BOMB,
    GET_PASSWD,
    UNKNOWN
};

#endif // ACTION_TYPE_HPP