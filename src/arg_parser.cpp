#include "arg_parser.h"

bool ArgParser::is_recognized(std::string arg, struct RecognizedArg *out) {
    return std::any_of(recognized_args.begin(), recognized_args.end(), [&arg, &out](struct RecognizedArg a){ 
        if (a.arg == arg) {
            out->arg = a.arg;
            out->has_next = a.has_next;
        }
        return a.arg == arg;
    });
}

ArgParser::ArgParser(int &argc, char **argv) {
    // i starts in 1 to skip the executable
    for (int i = 1; i < argc; i++) {
        std::string carg = argv[i];
        struct RecognizedArg rarg;
        if (is_recognized(carg, &rarg)) {
            if (exists(rarg.arg))
                throw "Duplicated argument " + rarg.arg;

            struct FilledArg farg;
            farg.arg = rarg.arg;
            farg.has_next = rarg.has_next;
            if (rarg.has_next) {
                if (i == (argc - 1))
                    throw "Argument " + rarg.arg + " expects something, but got nothing.";
                farg.info = argv[++i];
            }
            filled_args.push_back(farg);
        } else {
            str_args.push_back(argv[i]);
        }
    }
}

bool ArgParser::exists(const std::string arg) const {
    return std::any_of(filled_args.begin(), filled_args.end(), [&arg](struct FilledArg f){ return f.arg == arg; });
}

const std::string& ArgParser::get(const std::string arg) const {
   auto first = std::find_if(filled_args.begin(), filled_args.end(), [&arg](struct FilledArg ra){ return ra.arg == arg; });
    if (first != filled_args.end() && first->has_next)
        return first->info;
    static const std::string empty_string("");
    return empty_string;
}

void ArgParser::execute_if_arg(const std::string arg, const std::function<void(std::string)> callback) {
    std::string response = get(arg);
    if (response.length() > 0)
        callback(response);
}

std::vector<std::string> ArgParser::get_str_args() const {
    return str_args;
}