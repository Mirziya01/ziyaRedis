/*
Makes TCP connection (part 3 main.cpp)

    "getaddrinfo" -> IPv4, IPv6;

    connectToServer();
    sendCommand() -> send a command over the socket;
    disconnect();

*/


#include "RedisClient.h"

RedisClient::RedisClient(const std::string &host, int port): host(host), port(port), sock_fd(-1) {}

RedisClient::~RedisClient() {
    disconnect();
}

bool RedisClient::connectToServer() {
    struct addrinfo hints, *res = nullptr;
    std::memset(&hints, 0, sizeof(hints)); //Clear Hints
    hints.ai_family = AF_UNSPEC; //unspecified
    hints.ai_socktype = SOCK_STREAM; //TCP

    std::string portStr = std::to_string(port);//Convert to str
    int err = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res);//Resolve Addr
    if(err != 0){//Error Handling
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n";
        return false;
    }

    //Loop through address
    for(auto p = res; p != nullptr; p = p->ai_next) {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);//Make Scoket
        if(sock_fd == -1) continue;//Skip if Connection failed
        if(connect(sock_fd, p->ai_addr, p->ai_addrlen) == 0) break;//Break on success
        disconnect();
        // close(sock_fd);//Close socket if failed
        // sock_fd = -1;
    }
    freeaddrinfo(res);

    if(sock_fd == -1) {//Error Handling
        std::cerr << "Could not connect to " << host << ":" << port << "\n";
        return false;
    }
    return true;
}

void RedisClient::disconnect() {
    if(sock_fd != -1) {
        close(sock_fd);
        sock_fd = -1;
    }
}

int RedisClient::getSocketFd() const {
    return sock_fd;
}