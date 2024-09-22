#include <array>
#include <cassert>
#include <cctype>
#include <initializer_list>
#include <unordered_map>
#include <utility>

#include "parser.h"

char Lexer::peek() {
    return (end < source.size())? source[end]: '\0';
}

char Lexer::next() {
    return (end < source.size())? source[end++]: '\0';
}

void Lexer::reset(size_t old_front, size_t old_end) {
    front = old_front;
    end = old_end;
}

Token Lexer::next_identifier() {
    while (isalnum(peek())) {
        next();
    }

    std::string_view string {&source[front], end - front};

    if (string == "start") {
        return Token {TokenKind::Start, string};
    } else if (string == "accept") {
        return Token {TokenKind::Accept, string};
    } else {
        return Token {TokenKind::Identifier, string};
    }
}

std::string_view Lexer::next_string() {
    while (peek() != '\"') {
        next();
    }
    return std::string_view {&source[front + 1], end - front - 1};
}

std::pair<Token, bool> Lexer::next_token() {
    Token token {};
    bool success = false;

    while (isspace(peek())) {
        next();
    }
    char ch = peek();
    front = end;

    if (std::isalnum(ch)) {
        token = next_identifier();
        success = true;
    } else {
        switch (ch) {
            case ':':
                next();
                if (peek() == '=') {
                    token = {TokenKind::Equals, 
                        std::string_view(&source[front], end - front + 1)};
                    success = true;
                }
                break;
            case '=': 
                next();
                if (peek() == '>') {
                    token = {TokenKind::Arrow, 
                        std::string_view(&source[front], end - front + 1)};
                    success = true;
                }
                break;
            case '_': 
                token = {TokenKind::Underscore, 
                    std::string_view(&source[front], end - front + 1)};
                success = true;
                break;
            case '\"':
                next();
                std::string_view string = next_string();
                if (peek() == '\"') {
                    token = Token {TokenKind::String, string};
                    success = true;
                }
                break;
        }
    }
    next();
    return std::pair(token, success);
}

std::pair<Token, bool> Lexer::match(TokenKind kind) {
    TokenArray<1> token{};
    bool result = match_many(token, {std::pair(kind, TokenKind::None)});
    return std::pair(token.front(), result);
}

template <size_t SIZE>
bool Lexer::match_many(TokenArray<SIZE>& tokens, TokenKindList to_match) {
    size_t old_front = front;
    size_t old_end = end;

    int i = 0;
    for (auto item: to_match) {
        std::pair<Token, bool> next = this->next_token();
        if (!next.second) {
            reset(old_front, old_end);
            return false;
        }
        if (next.first.kind == item.first 
            || next.first.kind == item.second) {
            tokens[i++] = next.first;
        } else {
            reset(old_front, old_end);
            std::cerr << "Couldnt match rule: " << 
                item.first << " or " << item.second << "\n";
            return false;
        }
    }
    return true;
}


void Parser::add_transition(std::array<Token, 5>& tokens) {
    rules.emplace(std::pair(
        tokens.front(), 
        Rule{tokens.back(), tokens[2].string, tokens[3].string }
    ));
}


bool Parser::advance() {
    using tk = TokenKind;

    if (start.kind == tk::None) {
        TokenArray<3> state_tokens{};
        bool start_matched = lexer.match_many(state_tokens, {
            std::pair(tk::Start, tk::None),
            std::pair(tk::Equals, tk::None),
            std::pair(tk::Identifier, tk::None),
        });

        if (start_matched) {
            start = state_tokens.back();
            return true;
        }
    }

    if (accept.kind == tk::None) {
        TokenArray<3> state_tokens{};
        bool accept_matched = lexer.match_many(state_tokens, {
            std::pair(tk::Accept, tk::None),
            std::pair(tk::Equals, tk::None),
            std::pair(tk::Identifier, tk::None),
        });

        if (accept_matched) {
            accept = state_tokens.back();
            return true;
        }
    }

    TokenArray<5> rule_tokens{};
    bool rule_matched = lexer.match_many(rule_tokens, {
        (rules.count(Token {tk::Underscore, "_"}) == 0)? 
        std::pair(tk::Identifier, tk::Underscore): 
        std::pair(tk::Identifier, tk::None), 
        std::pair(tk::Arrow, tk::None),
        std::pair(tk::String, tk::Underscore), 
        std::pair(tk::String, tk::Underscore), 
        std::pair(tk::Identifier, tk::None)});

    if (rule_matched) {
        add_transition(rule_tokens);
        return true;
    }

    return false;
}

FiniteStateMachine Parser::parse() {
    while (advance());
    return FiniteStateMachine { start, accept, rules };
}


