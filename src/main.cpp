#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include "clock.h"
#include "logger.h"
#include "arg_parser.h"
#include "filereader.h"
#include "lexical/token.h"
#include "lexical/lexical.h"
#include "syntactic/syntactic.h"

void show_help();

int main(int argc, char **argv) {

    ArgParser *args;
    LogSeverity log_severity = LogSeverity::SUCCESS;
    std::shared_ptr<Logger> logger(new Logger(log_severity));

    try {
        args = new ArgParser(argc, argv);
    } catch (std::string msg) {
        logger->severe(msg);
        exit(-1);
    }
    
    args->execute_if_arg("-l", [&log_severity, &logger](std::string loglvl) {
        if (loglvl == "info")
            log_severity = LogSeverity::INFO;
        else if (loglvl == "success")
            log_severity = LogSeverity::SUCCESS;
        else if (loglvl == "warn")
            log_severity = LogSeverity::WARN;
        else if (loglvl == "error")
            log_severity = LogSeverity::ERROR;
        else if (loglvl == "severe")
            log_severity = LogSeverity::SEVERE;
        else {
            logger->severe("Invalid log severity '" + loglvl + "'");
            exit(-1);
        }
    });

    
    if (log_severity != LogSeverity::SUCCESS)
        logger.reset(new Logger(log_severity));

    if (args->exists("-h")) {
        show_help();
        exit(0);
    }

    std::string file_path;

    std::vector<std::string> str_args = args->get_str_args();
    // Args count is less than two, that means that there was not a single argument, no input file provided.
    if (str_args.size() == 0) {
        logger->severe("Please specify an input file.");
        exit(-1);
    // Only one or more arguments provided.
    } else {
        if (str_args.size() > 1) {
            logger->warn("More than one input file, using the first one (" + str_args[0] + ")");
        }
        
        file_path = str_args[0];
    }

    try {
        Clock c = Clock();

        std::shared_ptr<FileReader> file_reader(new FileReader(file_path, 20));
        std::shared_ptr<LexicalAnalyzer> lexical_analyzer(new LexicalAnalyzer(file_reader));
        std::shared_ptr<SyntacticAnalyzer> semantic_analyzer(new SyntacticAnalyzer(lexical_analyzer, logger));

        semantic_analyzer->analyze();

        c.stop();

        std::stringstream finalmsg;
        finalmsg << "OK " << std::setprecision(2) << c.get_ms() << "ms";
        logger->success(finalmsg.str());     
        
    } catch (const char *msg) {
        logger->error(msg);
    } catch (std::string msg) {
        logger->error(msg);
    }

}

void show_help() {
    std::cout <<
    "Usage:\n\t-l\tSet log level (info, success, warn, error, severe), defaults to success\n\t-h\tShow this message\n"    
    << std::endl;
}
