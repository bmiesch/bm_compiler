#include <string>
#include <iostream>


// TokenType is the enum defining the tokens that the language supports
enum TokenType {
    Number,
    Identifier,
    Equals, 
    OpenParen, 
    ClosedParen,
    BinaryOperator,
    Let,
    Keyword,
    BooleanLiteral,
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

bool isKeyword(const std::string &str) {
    if(str == "def") return true;
    return false;
}


std::vector<std::string> splitString(std::string &words) {
    std::vector<std::string> ret;
    std::string curWord;
    bool inComment = false;

    for(int i = 0; i < words.size(); i++) {

        // Handle Comments 
        if(inComment) {
            if(words[i] == '\n') inComment = false;
            continue;
        }
        if( i < words.size()-1 && words[i] == '/' && words[i+1] == '/') {
            inComment = true;
            i++;
            if(!curWord.empty()) {
                ret.push_back(curWord);
                curWord.clear();
            }
            continue;
        }


        if(isspace(words[i])) {
            if(!curWord.empty()) {
                ret.push_back(curWord);
                curWord.clear();
            }
        }
        else if (words[i] == '(' || words[i] == ')' || words[i] == '{' || words[i] == '}'\
                || words[i] == '+' || words[i] == '-' || words[i] == '*' || words[i] == '/') {
            if(!curWord.empty()) {
                ret.push_back(curWord);
                curWord.clear();
            }
            ret.push_back(std::string(1, words[i]));
        }
        else {
            curWord += words[i];
        }
    }

    if (!curWord.empty()){
        ret.push_back(curWord);
    }

    return ret;
}


std::vector<Token> tokenize(std::string &sourceCode) {

    std::vector<std::string> words = splitString(sourceCode);

    std::vector<Token> tokens;
    while(!words.empty()) {
        std::string word = words.front();
        words.erase(words.begin());

        if (word == "(" || word == "{") {
            tokens.push_back(Token{word, TokenType::OpenParen});
        } else if (word == ")" || word == "}") {
            tokens.push_back(Token{word, TokenType::ClosedParen});
        } else if (word == "=") {
            tokens.push_back(Token{word, TokenType::Equals});
        } else if (word == "+" || word == "-" || word == "*" || word == "/") {
            tokens.push_back(Token{word, TokenType::BinaryOperator});
        } else if (word == "let") {
            tokens.push_back(Token{word, TokenType::Let});
        } else if (isKeyword(word)) {
            tokens.push_back(Token{word, TokenType::Keyword});
        } else if (word == "True" || word == "False") {
            tokens.push_back(Token{word, TokenType::BooleanLiteral});
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