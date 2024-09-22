#include "parser.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

bool execute(const FiniteStateMachine& fsm, std::string_view input, std::ostream& stream) {
    Token current = fsm.start;
    Rule current_rule;

    std::cout << "current = " << current.string << "\n";
    while (current != fsm.accept) {
        auto it = fsm.rules.find(current);
        if (it == fsm.rules.end()) {
            return false;
        } 
        current_rule = it->second;
        
        if (current_rule.input != "_" && !input.starts_with(current_rule.input)) {
            return false;
        }

        if (current_rule.input != "_") {
            input.remove_prefix(current_rule.input.size());
        }

        current = current_rule.next;
        std::cout << "current = " << current.string << "\n";

        if (current_rule.output != "_") {
            stream << current_rule.output << "\n";
        }
    }
    return true;
}
    
