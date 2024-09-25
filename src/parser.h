#pragma once

#include <string>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class TokenKind {
    None,
    Start,
    Accept,
    Identifier,
    Arrow,
    Equals,
    Underscore,
    String
};

inline std::ostream& operator<<(std::ostream& os, TokenKind& kind) {
    using tk = TokenKind;
    switch (kind) {
        case tk::None: os << "None"; break;
        case tk::Start: os << "Start"; break;
        case tk::Accept: os << "Accept"; break;
        case tk::Identifier: os << "Identifier"; break;
        case tk::Arrow: os << "Arrow"; break;
        case tk::Equals: os << "Equals"; break;
        case tk::Underscore: os << "Underscore"; break;
        case tk::String: os << "String"; break;
    }
    return os;
}

struct Token {
    TokenKind kind {};
    std::string_view string {};
    bool operator==(const Token&) const = default;
};

template <typename T>
inline void hash_combine(std::size_t& s, const T& v) {
    std::hash<T> h;
    s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
}

struct HashToken {
    std::size_t operator() (Token const& t) const {
        std::size_t res = 0;
        hash_combine(res, t.kind);
        hash_combine(res, t.string);
        return res;
    }
};

template <size_t SIZE>
using TokenArray = std::array<Token, SIZE>;
using TokenKindList = std::initializer_list<std::pair<TokenKind, TokenKind>>;

class Lexer {
public:
    Lexer(std::string_view source): 
        source {source}, front {0}, end{0} {}

    std::pair<Token, bool> next_token();
    std::pair<Token, bool> match(TokenKind kind);

    template <size_t SIZE>
    bool match_many(TokenArray<SIZE>& tokens, TokenKindList to_match);

private:
    std::string_view source {};
    size_t front {};
    size_t end {};

    char peek();
    char next();

    void reset(size_t old_front, size_t old_end);
    Token next_identifier();
    std::string_view next_string();
};

struct Rule {
    Token next;
    std::string_view input;
    std::string_view output;
};

inline std::ostream& operator<<(std::ostream& os, Rule& rule) {
    os << "Next: " << ((rule.next.kind == TokenKind::None)? "_": rule.next.string) << "\n";
    os << "Input: " << rule.input << "\n";
    os << "Output: " << rule.output << "\n";
    return os;
}

struct FiniteStateMachine {
    Token start {};
    Token accept {};
    std::unordered_map<Token, std::vector<Rule>, HashToken> rules {};
};

class Parser {
public:
    Parser(std::string_view source): 
        lexer {source}, start{}, accept{}, rules{} {}
    bool advance();
    FiniteStateMachine parse();

private:
    Token start {};
    Token accept {};
    std::unordered_map<Token, std::vector<Rule>, HashToken> rules {};
    Lexer lexer;
    void add_transition(std::array<Token, 5>& tokens);
};




