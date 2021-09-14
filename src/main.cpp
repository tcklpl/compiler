#include <iostream>
#include <memory>
#include "filereader.h"
#include "lexical/token.h"
#include "lexical/lexical.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "[ERROR] Please specify an input file.\n";
        exit(-1);
    }

    try {
        std::shared_ptr<FileReader> file_reader(new FileReader(argv[1], 20));
        
        LexicalAnalyzer la(file_reader);
        std::shared_ptr<Token> t = nullptr;
        while ((t = la.next_token()) != nullptr && t.get()->type() != TokenType::lng_Fim)
        {
            std::cout << *t.get() << std::endl;
        }
        
        
    } catch (const char *msg) {
        std::cout << "[ERROR] " << msg << std::endl;
    } catch (std::string msg) {
        std::cout << "[ERROR] " << msg << std::endl;
    }

}
