#include "CLI.h"

static std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    if(start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start +1);
}

void printHelp() {
    std::cout << "redis_cli 1.0.0\n"
              << "Usage:\n"
              << "      With arguments:             ./redis_cli -h <host> -p <port>\n"
              << "      Default Host (127.0.0.1):   ./redis_cli -h <host>\n"
              << "      Default Port (6379):        ./redis_cli -p <port>\n"
              << "      One-shot execution:         ./redis_cli <command> [arguments]\n"
              << "\n"
              << "Interactive Mode (REPL): \n"
              << "      ./redis_cli\n"
              << "      Type Redis commands directly.\n"
              << "\n"
              << "To get help about Redis commands type:\n"
              << "      \"help\" to display this help message\n"
              << "";
}

CLI::CLI(const std::string &host, int port) 
    : host(host), port(port), redisClient(host, port){}

void CLI::run(const std::vector<std::string> &commandArgs) {
    if(!redisClient.connectToServer()) {
        return;
    }

    if(!commandArgs.empty()) {
        executeCommand(commandArgs);
    }


    std::cout << "Connected to Redis at " << redisClient.getSocketFd() << "\n";

    while(true) {
        std::cout << host << ":" << port << "> ";
        std::cout.flush();
        std::string line;
        if(!std::getline(std::cin, line)) break;
        line = trim(line);
        if(line.empty()) continue;
        if (line == "quit" || line == "exit"){
            std::cout << "GoodBye.\n";
            break;
        }

        if(line == "help") {
            std::cout << "Displaying help: \n";
            continue;
        }

        //Split command into tokens
        std::vector<std::string> args = CommandHandler::splitArgs(line);
        if(args.empty()) continue;

        // for(const auto &arg : args) {
        //     std::cout << arg << "\n";
        // }

        std::string command = CommandHandler::buildRESPcommand(args);
        if(!redisClient.sendCommand(command)) {
            std::cerr << "(Error) Failed to send Command.\n";
            break;
        }
        //Parse and print response
        std::string response = ResponseParser::parseResponse(redisClient.getSocketFd());
        std::cout << response << "\n";
    }
    redisClient.disconnect();
}

void CLI::executeCommand(const std::vector<std::string> &args) {
    if(args.empty()) return;
    std::string command = CommandHandler::buildRESPcommand(args);
    if(!redisClient.sendCommand(command)) {
        std::cerr << "(Error) Failed to send command.\n";
        return;
    }

    std::string response = ResponseParser::parseResponse(redisClient.getSocketFd());
    std::cout << response << "\n";    
}
