#include "parser.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

bool apply_rule(Rule rule, std::string_view& input, std::ostream& stream) {
    if (rule.input != "_" && !input.starts_with(rule.input)) {
        return false;
    }

    input.remove_prefix(rule.input.size());

    if (rule.output != "_") {
        stream << rule.output << "\n";
    }
    return true;
}

bool execute(const FiniteStateMachine& fsm, std::string_view input, std::ostream& stream) {
    Token current = fsm.start;
    Rule current_rule {};

    std::cout << "current = " << current.string << "\n";
    while (current != fsm.accept) {
        auto it = fsm.rules.find(current);
        if (it == fsm.rules.end()) {
            it = fsm.rules.find(Token {TokenKind::Underscore, "_"});
            if (it == fsm.rules.end()) {
                return false;
            }
        }
        current_rule = it->second;

        if (!apply_rule(current_rule, input, stream)) {
            return false;
        }
        current = current_rule.next;
        // std::cout << "current = " << current.string << "\n";
    }
    return input.size() == 0;
}
    
