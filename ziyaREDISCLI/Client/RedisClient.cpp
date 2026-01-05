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
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    int err = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res);
    if (err != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n";
        return false;
    }

    for (auto p = res; p != nullptr; p = p->ai_next) {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock_fd == -1) continue;
        if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == 0) break;
        disconnect(); // Only this call is needed
    }
    freeaddrinfo(res);

    if (sock_fd == -1) {
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

bool RedisClient::sendCommand(const std::string &command) {
    if(sock_fd == -1) return false;
    ssize_t sent = send(sock_fd, command.c_str(), command.size(), 0);
    return (sent == (ssize_t)command.size());
}