#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum TokenType {
    Number,
    Identifier,
    Equals,
    OpenParen,
    ClosedParen,
    OpenBracket,
    ClosedBracket,
    Add,
    Subtract,
    Multiply,
    Divide,
    Let,
    Keyword,
    BooleanLiteral,
    End
};

struct Token {
    std::string value;
    TokenType type;
};

std::vector<Token> tokenize(std::string &sourceCode);

#endif // LEXER_H