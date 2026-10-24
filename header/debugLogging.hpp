//
// Created by tobias on 23.10.26.
//

#ifndef STRATEGIE_GAME_DEBUGLOGGING_HPP
#define STRATEGIE_GAME_DEBUGLOGGING_HPP

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define GREEN   "\033[1;32m"
#define BLUE    "\033[1;34m"
#define PINK    "\033[1;35m"
#define CYAN    "\033[1;36m"

#include <iostream>
#include <chrono>
#include <format>
#include <source_location>

namespace ch = std::chrono;

#include "glm/glm.hpp"


// Loging & Debugging
namespace LOG {

    // template for the loggers
    inline void messageBuilder(
        std::string color,
        std::string prefix,
        std::string &msg,
        const std::source_location loc = std::source_location::current()) {

        auto time = ch::floor<ch::seconds>(ch::system_clock::now());

        std::string rawFunc = loc.function_name();
        size_t bracketPos = rawFunc.find('(');
        std::string ploh = (bracketPos != std::string::npos) ? rawFunc.substr(0, bracketPos) : rawFunc;

        // cout templace
        std::cout   << color
                    << "[ " << prefix << " | "
                    << std::format("{:%T}", time)
                    << " | Func: " << ploh << "()"
                    << " | Line: " << loc.line()
                    << " ] : "
                    << RESET
                    << msg
                    << std::endl;
    }

    // Informative
    inline void INFO(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(GREEN, "INFO ", msg, loc);
    }
    inline void DEBUG(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(BLUE, "DEBUG  ", msg, loc);
    }
    inline void INIT_SUCCESS(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(CYAN, "INIT SUCCESS 󰇚", msg, loc);
    }

    // Warning
    inline void WARN(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(YELLOW, "WARNING ", msg, loc);
    }
    inline void ERR(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(RED, "ERROR ", msg, loc);
    }
    inline void INIT_ERR(std::string msg, const std::source_location& loc = std::source_location::current()) {
        messageBuilder(PINK, "INIT ERROR 󱂰", msg, loc);
    }

}



#endif //STRATEGIE_GAME_DEBUGLOGGING_HPP
