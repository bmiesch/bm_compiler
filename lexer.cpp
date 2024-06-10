#include <string>
#include <iostream>


// TokenType is the enum defining the tokens that the language supports
enum TokenType {
    Number, //
    Identifier, //
    Equals, //
    OpenParen, // 
    ClosedParen, //
    BinaryOperator,
    Let // 
};

struct Token {
    std::string value;
    TokenType type;
};

bool isNumber(const std::string &str) {
    for(char c : str) {
        if(!isdigit(c)) return false;
    }
    return true;
}

bool isAlpha(const std::string &str) {
    for(char c : str) {
        if(!isalpha(c)) return false;
    }
    return true;
}


std::vector<std::string> splitString(std::string &words) {
    std::vector<std::string> ret;
    std::string curWord;
    for(int i = 0; i < words.size(); i++) {

        if(words[i] != ' ') {
            curWord += words[i];
        }
        else if(!curWord.empty()) {
            ret.push_back(curWord);
            curWord.clear();
        }
    }

    if (!curWord.empty()){
        ret.push_back(curWord);
    }

    return ret;
}


std::vector<Token> tokenize(std::string &sourceCode) {

    // Split code into words
    std::vector<std::string> words = splitString(sourceCode);

    // Iterate through words and create tokens
    std::vector<Token> tokens;
    while(!words.empty()) {
        std::string word = words.front();
        words.erase(words.begin());

        if (word == "(") {
            tokens.push_back(Token{word, TokenType::OpenParen});
        } else if (word == ")") {
            tokens.push_back(Token{word, TokenType::ClosedParen});
        } else if (word == "=") {
            tokens.push_back(Token{word, TokenType::Equals});
        } else if (word == "let") {
            tokens.push_back(Token{word, TokenType::Let});
        } else if (isNumber(word)) {
            tokens.push_back(Token{word, TokenType::Number});
        } else if (isAlpha(word)) {
            tokens.push_back(Token{word, TokenType::Identifier});
        } else {
            std::cout << "Unrecognizable character found: " << word << std::endl;
        }
    }

    return tokens;
}