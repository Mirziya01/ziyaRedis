/*
1. Command-Line parsing
    -h (host) default: 127.0.0.1; -p (port) 6379

    If no args, launch REPL mode

2. OOP
    redisClient, commandHandler, responseParser, CLI

3. Make TCP Connection to Redis() (redisClient)
    BSD sockets
    IPv4, and IPv6 "getaddrinfo"

4. Parsing and Command Formatting (commandHandler)
    Split user input
    Convert commands into RESP format:
    """
    *3\r\n
    $3\r\nSET\r\n
    $3\r\nkey\r\n
    $5\r\nvalue\r\n
    """

5. Handle REDIS responses (responseParser)
    Read server Responses and parse RESP types
    +OK, -Error, :100, $5\r\nhello -> Bulk string, *2\r\n$3\r\nfoo\r\n$3\r\nbar -> Array response

6. Implement interactive REPL (CLI)
    Run loop: User Input, valid redis commands, send commands to the server, display parsed response
    Support: help, quit

7. main.cpp: parse command-line arguments, instantiate CLI and launch in REPL mode.

Socket Programming, Protocol Handling, OOP, CLI development
*/

#include "CLI.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string host = "127.0.0.1";
    int port = 6379;
    int i = 1;

    //Parse command-line args for -h, and -p
    while(i < argc) {
        std::string arg = argv[i];
        if(arg == "-h" && i + 1 < argc) {
            host = argv[++i];
        } else if(arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else {
            break;
        }
        ++i;
    }

    // Handle REPL and one-shot command modes
    CLI cli(host, port);
    cli.run();


    return 0;
}