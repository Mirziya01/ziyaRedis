#include "ResponseParser.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>

static bool readChar(int sock_fd, char &c) {
    ssize_t r = recv(sock_fd, &c, 1, 0);
    if (r <= 0) return false; // Handle errors/closure
    return true;
}

static std::string readLine(int sock_fd) {
    std::string line;
    char c;
    while(readChar(sock_fd, c)) {
        if(c == '\r') {
            //Expect "\n" next; read and break
            readChar(sock_fd, c);
            break;
        }
        line.push_back(c);
    }
    return line;
}

std::string ResponseParser::parseResponse(int sock_fd) {
    char prefix;
    if(!readChar(sock_fd, prefix)) {
        return (("(Error) No response or connection closed."));
    }
    switch(prefix) {
        case '+' : return parseSimpleString(sock_fd);
        case '-' : return parseSimpleError(sock_fd);
        case ':' : return parseInteger(sock_fd);
        case '$' : return parseBulkString(sock_fd);
        case '*' : return parseArray(sock_fd);
        default: return "(Error) Unknown reply type.";
    }
}

std::string ResponseParser::parseSimpleString(int sock_fd) {
    return readLine(sock_fd);
}
std::string ResponseParser::parseSimpleError(int sock_fd) {
    return "(Error)  " + readLine(sock_fd);
}
std::string ResponseParser::parseInteger(int sock_fd) {
    return readLine(sock_fd);
}
std::string ResponseParser::parseBulkString(int sock_fd) {
    std::string lenStr = readLine(sock_fd);
    int length = std::stoi(lenStr);
    if(length == -1) {
        return "(null)";
    }
    std::string bulk;
    bulk.resize(length);
    int totalRead = 0;

    //Loop to read the bulk data from the socket
    while(totalRead < length) {
        ssize_t r = recv(sock_fd, &bulk[totalRead], length - totalRead, 0);
        if(r <= 0) {
            return "(Error) Incomplete bulk data.";
        }
        totalRead += r; //Update bytes read
    }
    //Consume trailing CRLF
    char dummy;
    readChar(sock_fd, dummy); //Read CR
    readChar(sock_fd, dummy); //Read LF

    return bulk;
}
std::string ResponseParser::parseArray(int sock_fd) {
    std::string countStr = readLine(sock_fd);
    int count = std::stoi(countStr);
    if(count == -1){
        return "(null)";
    }
    std::ostringstream oss;
    for(int i = 0; i < count; ++i) {
        oss << parseResponse(sock_fd);
        if(i != (count - 1)) {
            oss << "\n";
        }
    }
    return oss.str();
}