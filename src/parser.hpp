#include <unordered_set>
#include <vector>
#include "ex.hpp"
#include "type.hpp"

#ifndef PARSER_HPP
#define PARSER_HPP

using namespace std;

vector<ZPiece> PIECES;

int CURLY_BRACK_COUNT = 0;
int PAREN_COUNT = 0;
int SAVED_CURLY_BRACK_COUNT = 0;
int SAVED_PAREN_COUNT = 0;

vector<ZPiece> parse(const vector<Token>& tokens, int level) {
    ZPiece piece;
    for (size_t i = 0; i < tokens.size(); i++) {
        auto makeString = [&](StringType stringType) {
            ZStringLiteral stringLiteral;
            piece.ptype = ZPieceType::ZStringLiteral_t;
            i++;
            switch (stringType) {
                case StringType::DOUBLE: {
                    stringLiteral.value = tokens[i].value;
                }
                default: {
                    // ??? Never should get here in production
                }
            }
            i+=2;
            piece.stringLiteral = stringLiteral;
            piece.depth = level;
            PIECES.push_back(piece);
            cout << "\n String \n Value: " + (piece.stringLiteral.value) + '\n';
        };

        auto makeVariable = [&](Bool isParameter) {
            i++;
            ZVariable variable;
            int offset = 2;
            variable.constant = (tokens[i].value == "const") ? True : False;
            variable.parameter = isParameter;
            if (isTrue(variable.constant)) {
                i++;
            } else {
                //offset++;
            }
            variable.type = tokens[i].value;
            variable.name = tokens[i+1].value;
            piece.variable = variable;
            piece.ptype = ZPieceType::ZVariable_t;
            piece.depth = level;
            PIECES.push_back(piece);
            i+=offset-1;
            cout << "\n Variable \n Name: " + (piece.variable.name) + "\n Type: " + (piece.variable.type) + "\n Const: " << isTrue(piece.variable.constant) << '\n';
            cout << tokens[i+1].value + "\n";
            if (tokens[i+1].type == TokenType::EQUAL) {
                i--;
                ZInitialize initialize;
                i++;
                if (tokens[i].type == TokenType::DOUBLE_QUOTE) {
                    makeString(StringType::DOUBLE);
                }
            }
            i++;
        };

        auto makeFunction = [&] () {
            ZFunction function;
            i++;
            function.type = tokens[i].value;
            function.name = tokens[i+1].value;
	    i++;            
            piece.function = function;
            piece.depth = level;
            piece.ptype = ZPieceType::ZFunction_t;
            PIECES.push_back(piece);
            vector<Token> functionTokens;
            PAREN_COUNT++;
            cout << "\n Function \n Name: " + (piece.function.name) + "\n Type: " + (piece.function.type) + '\n';
            while (true) {
                cout << tokens[i].value + "\n";
                if (tokens[i].type == TokenType::RIGHT_PAREN) {
                    PAREN_COUNT--;
                    if (CURLY_BRACK_COUNT == 0) {
                        break;
                    }
                    i++;
                    continue;
                }
                if (tokens[i].type == TokenType::LEFT_PAREN) {
                    PAREN_COUNT++;
                    i++;
                    continue;
                }
                if (i > tokens.size()) {
                    Zerror(UNEXPECTED_END_OF_FILE, NULL);
                }
                if (tokens[i].type != TokenType::IDENTIFIER && tokens[i].type != TokenType::COMMA) {
                    Zerror(UNEXPECTED_FUNCTION_ARGUMENT, &tokens[i].value, tokens[i].line,tokens[i].pos);
                }
                i++;
            }
            PAREN_COUNT--;
            i+=2;
            if (i < tokens.size() && tokens[i].type != TokenType::LEFT_CURLY_BRACK) {
                SAVED_CURLY_BRACK_COUNT = CURLY_BRACK_COUNT;
                
                while (i < tokens.size()) {
                    if (tokens[i].type == TokenType::LEFT_CURLY_BRACK) {
                        CURLY_BRACK_COUNT++;
                        i++;
                        continue;
                    }
                    if (tokens[i].type == TokenType::RIGHT_CURLY_BRACK) {
                        CURLY_BRACK_COUNT--;
                        i++;
                        if (CURLY_BRACK_COUNT == 0) {
                            break;
                        }
                        continue;
                    }
                    functionTokens.push_back(tokens[i]);
                    i++;
                }
                parse(functionTokens, level + 1);  // Recursively parse the function body
                i++;
            }
        };

        auto makeClass = [&]() {
            i++;
            ZClass class_;
            class_.name = tokens[i].value;
            piece.class_ = class_;
            piece.depth = level;
            piece.ptype = ZPieceType::ZClass_t;
            PIECES.push_back(piece);
            vector<Token> classTokens;
            i++;
            cout << "\n Class \n Name: " + (piece.class_.name) + '\n';
            while (i < tokens.size()) {
                if (tokens[i].type == TokenType::LEFT_CURLY_BRACK) {
                    CURLY_BRACK_COUNT++;
                    if (tokens[i+1].type == TokenType::AT && tokens[i+2].type == TokenType::RIGHT_CURLY_BRACK) {
                        i+=2;
                        //makeFunction(class_);
                    }
                }
                if (tokens[i].type == TokenType::RIGHT_CURLY_BRACK) {
                    CURLY_BRACK_COUNT--;
                    if (CURLY_BRACK_COUNT == 0) {
                        break;
                    }
                }
                classTokens.push_back(tokens[i]);
                i++;
            }
        };
        

        if (i < tokens.size() && tokens[i].type == TokenType::SEMICOLON) {
            continue;
        }
        
        if (tokens[i].type == TokenType::DOUBLE_QUOTE) {
            makeString(StringType::DOUBLE);
            continue;
        }

        if (i < tokens.size() && tokens[i].type == TokenType::IDENTIFIER) {
            if (tokens[i].value == "import") {
                ZImport import;
                i++;
                import.source = tokens[i].value;
                i+=2;
                if (tokens[i].type == TokenType::IDENTIFIER && tokens[i].value == "as") {
                    i++;
                    Token content = tokens[i];
                    if (content.type == TokenType::IDENTIFIER) {
                        //import.import = content.value;
                    } else if (content.type == TokenType::MULTIPLY) {
                        //import.imports = &content.value;
                    } else {
                        Zerror(UNEXPECTED_IMPORT_ARGUMENT, &tokens[i].value);
                    }
                }
            }
            if (i < tokens.size() && tokens[i].type == TokenType::IDENTIFIER && tokens[i].value == "func") {
                makeFunction();
                continue;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::IDENTIFIER && tokens[i].value == "var") {
                makeVariable(False);
                continue;
            }
            if (i < tokens.size() && tokens[i].type == TokenType::IDENTIFIER && tokens[i].value == "class") {
                makeClass();
                continue;
            }
        } else {
            if (tokens[i-1].type == TokenType::SEMICOLON) {
                const String trace[1] = {
                    ";"
                };
                //Zerror(DEBUG_MAGIC_SEMICOLON, trace, tokens[i-1].line, tokens[i-1].pos);
                continue;
            } 
          if (tokens[i-1].type == TokenType::IDENTIFIER)
            Zerror(MISSING_IDENTIFIER_AFTER_TYPE, &tokens[i-1].value, tokens[i-1].line, tokens[i-1].pos);
            else
          Zerror(UNEXPECTED_TOKEN + '"' + tokens[i-1].value + '"',  &tokens[i-1].value, tokens[i-1].line, tokens[i-1].pos);
        }
    }
    return PIECES;
}

#endif
