#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "ex.hpp"

#ifndef LEXER_HPP
#define LEXER_HPP

using namespace std;

enum class TokenType {
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    COMMA,
    PERIOD,
    COLON,
    SEMICOLON,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    TEXT,
    AT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_CURLY_BRACK,
    RIGHT_CURLY_BRACK,
    EQUAL,
    NOT,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    String value;
    int line;
    int pos;

    Token(TokenType type, const String& value) : type(type), value(value) {}
};

class Lexer {
public:
    Lexer(const String& text) : text(text), pos(0), line_pos(0), line(1) {
        current_char = text[pos];
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (current_char != '\0') {
            if (isspace(current_char)) {
		advance();
                skipWhitespace();
            } else if (isalpha(current_char)) {
                tokens.push_back(identifier());
            } else if (isdigit(current_char)) {
                tokens.push_back(number());
            } else if (current_char == '+') {
                tokens.push_back(Token(TokenType::PLUS, "+"));
                advance();
            } else if (current_char == '-') {
                tokens.push_back(Token(TokenType::MINUS, "-"));
                advance();
            } else if (current_char == '*') {
                tokens.push_back(Token(TokenType::MULTIPLY, "*"));
                advance();
            } else if (current_char == '/') {
                if (advance(1) == '/') {
                    skipLine();
                    continue;
                }
                tokens.push_back(Token(TokenType::DIVIDE, "/"));
                advance();
            } else if (current_char == ',') {
                tokens.push_back(Token(TokenType::COMMA, ","));
                advance();
            } else if (current_char == '.') {
                tokens.push_back(Token(TokenType::PERIOD, "."));
                advance();
            } else if (current_char == ':') {
                tokens.push_back(Token(TokenType::COLON, ":"));
                advance();
            } else if (current_char == ';') {
                tokens.push_back(Token(TokenType::SEMICOLON, ";"));
                advance();
            } else if (current_char == '"') {
                advance();
                String content;
                char prev_char = '%';
                char prev_prev_char = '%';
                tokens.push_back(Token(TokenType::DOUBLE_QUOTE, "\""));
                while (current_char != '"' || (prev_char == '\\' && prev_prev_char != '\\')) {
                    if (current_char == '\0') {
                        Zerror(UNEXPECTED_END_OF_FILE, NULL);
                    }
                    content += current_char;
                    prev_prev_char = prev_char;
                    prev_char = current_char;
                    advance();
                }
                tokens.push_back(Token(TokenType::TEXT, content));
                tokens.push_back(Token(TokenType::DOUBLE_QUOTE, "\""));
                advance();
            } else if (current_char == '\'') {
                tokens.push_back(Token(TokenType::SINGLE_QUOTE, "'"));
                advance();
            } else if (current_char == '(') {
                tokens.push_back(Token(TokenType::LEFT_PAREN, "("));
                advance();
            } else if (current_char == ')') {
                tokens.push_back(Token(TokenType::RIGHT_PAREN, ")")); 
                advance();
            } else if (current_char == '{') {
                tokens.push_back(Token(TokenType::LEFT_CURLY_BRACK, "{"));
                advance();
            } else if (current_char == '}') {
                tokens.push_back(Token(TokenType::RIGHT_CURLY_BRACK, "}"));
                advance();
            } else if (current_char == '@') {
                tokens.push_back(Token(TokenType::AT, "@"));
                advance();
            } else if (current_char == '=') {
                tokens.push_back(Token(TokenType::EQUAL, "="));
                advance();
            } else                          {
                tokens.push_back(Token(TokenType::UNKNOWN, String(1, current_char)));
                advance();
            }
            /*if (!isspace(current_char)) { 
                Token token = tokens.back();
                token.line = line;
                token.pos = line_pos;
                cout << token.value + " ";
            }*/
        }

        tokens.push_back(Token(TokenType::END_OF_FILE, ""));
        return tokens;
    }

private:
    String text;
    size_t pos;
    size_t line_pos;
    size_t line;
    char current_char;

    char advance(size_t count = 1) {
        pos += count;
        line_pos += count;
        current_char = (pos < text.length()) ? text[pos] : '\0';
        return current_char;
    }

    void skipLine() {
        while (current_char != '\n') {
            if (current_char == '\0') {
                break;
            }
            advance();
        }
    }

    String getStringText(char end_char) {
        Bool complete = False;
        Bool escape = False;
        String text = "";
        while (!isTrue(complete)) {
            if (current_char == '\\' && !isTrue(escape)) {
                escape = True; // Set escape flag for the next character
                continue;
            }

            if (current_char == end_char && !isTrue(escape)) {
                complete = True; // End of the string
                continue;
            }

            if (isTrue(escape)) {
                switch (current_char) {
                    case '\'': {
                        text += '\'';
                        break;
                    }
                    case '"': {
                        text += '"';
                        break;
                    }
                    case '\\': {
                        text += current_char;
                        break;
                    }
                    case 'n': {
                        text += '\n';
                        break;
                    }
                    case 't': {
                        text += '\t';
                        break;
                    }
                    case 'r': {
                        text += '\r';
                        break;
                    }
                    case 'x': {
                        cout<<"xd";
                        char hex1 = text[pos+1];
                        char hex2 = text[pos+2];
                        
                        if (!isxdigit(hex1) || !isxdigit(hex2)) {
                            throw std::runtime_error("Invalid hexadecimal escape sequence");
                        }

                        int value = (std::tolower(hex1) - '0') + ((std::tolower(hex1) >= 'a') ? (10 + std::tolower(hex1) - 'a') : 0);
                        value = (value << 4) + ((std::tolower(hex2) - '0') + ((std::tolower(hex2) >= 'a') ? (10 + std::tolower(hex2) - 'a') : 0));
                        text += static_cast<char>(value);
                        escape = False;
                        continue;
                    }
                    default: {
                        text += '\\';
                        text += current_char;
                        break;
                    }
                }
                escape = False;
            } else {
                text += current_char;
            }
        }
        return text;
    }

    void skipWhitespace() {
        while (current_char != '\0' && isspace(current_char)) {
            if (current_char == '\n') {
                line++;
                line_pos = 0;
            }
            advance();
        }
    }

    Token identifier() {
        String result;
        while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
            result += current_char;
            advance();
        }
        return Token(TokenType::IDENTIFIER, result);
    }

    Token number() {
        String result;
        while (current_char != '\0' && isdigit(current_char)) {
            result += current_char;
            advance();
        }
        return Token(TokenType::NUMBER, result);
    }
};


#endif
