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

    // std::cout << "current = " << current.string << "\n";
    while (current != fsm.accept) {
        auto it = fsm.rules.find(current);
        if (it == fsm.rules.end()) {
            it = fsm.rules.find(Token {TokenKind::Underscore, "_"});
            if (it == fsm.rules.end()) {
                return false;
            }
        }
        // TODO: if multiple rules match, it continues on with the first of such matches
        // Could show an error if this case happens, or could support non-determinism
        bool matched = false;
        for (const auto& rule: it->second) {
            if (apply_rule(rule, input, stream)) {
                current = rule.next;
                matched = true;
                break;
            }
        }
        // fallback to _ rule (implicitly declared rules)
        if (!matched) {
            it = fsm.rules.find(Token {TokenKind::Underscore, "_"});
            if (it == fsm.rules.end()) {
                return false;
            }
            for (const auto& rule: it->second) {
                if (apply_rule(rule, input, stream)) {
                    current = rule.next;
                    matched = true;
                    break;
                }
            }
        }
        if (!matched) {
            return false;
        }
        // std::cout << "current = " << current.string << "\n";
    }
    return input.size() == 0;
}
    
