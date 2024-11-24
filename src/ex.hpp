#include <string>
#include <iostream>

#ifndef EX_HPP
#define EX_HPP

#define MINT (signed int)
#define TINT unsigned int

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

#define BOLD       "\033[1m"
#define UNDERLINE  "\033[4m"

typedef std::string String;
enum Bool {
    True,
    False
};

enum StringType {
    SINGLE,
    DOUBLE,
    LITERAL
};

String UNEXPECTED_END_OF_ARGUMENTS = "Unexpected end of input";
String UNEXPECTED_END_OF_FILE = "Unexpected end of file";
String UNEXPECTED_TOKEN = "Unexpected token: ";
String UNEXPECTED_ARGUMENT = "Unexpected argument: ";
String UNEXPECTED_FUNCTION_ARGUMENT = "Unexpected token in function argument";
String UNEXPECTED_IMPORT_ARGUMENT = "Unexpected token in import argument";
String FILE_NOT_FOUND = "File not found: ";
String MISSING_IDENTIFIER_AFTER_TYPE = "Expected identifier after type";
String EXPECTED_INPUT_FILE = "Expected an input file";
String EXPECTED_OUTPUT_FILE = "Expected an output file";

String DEBUG_MAGIC_SEMICOLON = "SEMICOLON NOT CAUGHT";

String END_OF_ERROR = "!";

int isTrue(Bool value) {
    if (value == True) {
        return 1;
    }
    return 0;
};

void Zerrord(String message, const String* stack) {
    std::cout << RED BOLD "   - ERROR: " RESET << '\n' << "     " << message + END_OF_ERROR + '\n';
    if (stack != NULL)
    std::cout << RED BOLD "   - TRACE: " RESET << '\n' << "     " << *stack + " <- " RED BOLD "HERE" RESET + '\n';
}

void Zerrord(String message, const String* stack, int line, int pos) {
    std::cout << RED BOLD "   - ERROR: " RESET << '\n' << "     " << message + END_OF_ERROR + '\n';
    if (stack != NULL)
    std::cout << RED BOLD "   - TRACE: " RESET << '\n' << "     " << *stack + " <- " RED BOLD "HERE " + RESET << line << ":" << pos << '\n';
}

void Zerror(String message, const String* stack) {
    Zerrord(message, stack);
    std::exit(1);
}

void Zerror(String message, const String* stack, int line, int pos) {
    Zerrord(message, stack, line, pos);
    std::exit(1);
}

String Zdestring(const String& str) {
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

#endif