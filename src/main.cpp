#include <fstream>
#include <iostream>
#include <sstream>

#include "parser.h"
#include "execute.h"

int main (int argc, char *argv[]) {   
    if (argc <= 1) {
        std::cerr << "No input file(s) provided\n";
        return 1;
    }
    
    std::ifstream file {argv[1]};
    std::cout << "Reading file " << argv[1] << "\n";
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source {buffer.str()};
    std::cout << "Read file of " << source.size() << " bytes\n";

    Parser p {source};
    FiniteStateMachine fsm = p.parse();

    std::cout << "Start State: " << fsm.start.string << "\n";
    std::cout << "Accept State: " << fsm.accept.string << "\n\n";
    for (auto i: fsm.rules) {
        std::cout << "Current: " << ((i.first.kind == TokenKind::None)? "_": i.first.string) << "\n";
        std::cout << i.second << "\n";
    }

    if (execute(fsm, "abcdefgh", std::cout)) {
        std::cout << "Accepted\n";
    } else {
        std::cout << "Not accepted\n";
    }

    return 0;
}
