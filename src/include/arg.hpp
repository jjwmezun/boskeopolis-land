#ifndef ARG_H
#define ARG_H

#include <string>
#include <variant>
#include <unordered_map>

namespace BSL
{
    typedef std::variant<bool, int, float, std::string> Arg;
    typedef std::unordered_map<std::string, Arg> ArgList;
}

#endif // ARG_H