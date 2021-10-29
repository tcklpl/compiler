#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

struct RecognizedArg {
    std::string arg;
    bool has_next;
};

struct FilledArg : RecognizedArg {
    std::string info;
};

class ArgParser {
    private:
        const std::vector<struct RecognizedArg> recognized_args {
            {"-h", false},
            {"-l", true},
        };

        std::vector<struct FilledArg> filled_args;
        std::vector<std::string> str_args;

        bool is_recognized(std::string arg, struct RecognizedArg *out);

    public:
        ArgParser(int &argc, char **argv);

        bool exists(const std::string arg) const;
        const std::string& get(const std::string arg) const;

        void execute_if_arg(const std::string arg, const std::function<void(std::string)> callback);
        std::vector<std::string> get_str_args() const;
};