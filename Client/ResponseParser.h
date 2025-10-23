#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H

#include <string>

class ResponseParser {
    public:
        //Read from the socked and return parsed string. Returns "" on error.
        static std::string parseResponse(int sock_fd);


    private:
        //Redis Serialization Protocol 2
        static std::string parseSimpleString(int sock_fd);
        static std::string parseSimpleError(int sock_fd);
        static std::string parseInteger(int sock_fd);
        static std::string parseBulkString(int sock_fd);
        static std::string parseArray(int sock_fd);

};

#endif