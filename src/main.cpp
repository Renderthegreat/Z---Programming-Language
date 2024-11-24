#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"
#include "type.hpp"
#include "ex.hpp"
#include "lzcvrt.hpp"
#include "gen/files.h"

#include "lzvm/toolbox.hpp"

using namespace std;

int main(int argc, char *argv[])
{   
    cout << WHITE "   # :-- " << RED BOLD "Z#" RESET WHITE " --: #" RESET << '\n'; 
    // Get input and output files
    String input;
    String output;
    for (int i = 0; i < argc; i++) {
        const char* arg = argv[i];
        if (MINT sizeof(arg) * MINT sizeof(int) > 2)
        if (arg[0] == '-') {
            if (argc < i+2) {
                Zerror(UNEXPECTED_END_OF_ARGUMENTS, NULL);
            }
            if        (arg[1] == 'i') {
                input = argv[i+1];
            } else if (arg[1] == 'o') {
                output = argv[i+1];
            } else                       {
                Zerror(UNEXPECTED_ARGUMENT + '"' + arg + '"', NULL);
            }
        }
    }

    if (input == "") {
        Zerror(EXPECTED_INPUT_FILE, NULL);
    }

    if (output == "") {
        Zerror(EXPECTED_OUTPUT_FILE, NULL);
    }
    
    // Read input file
    ifstream file(input + "/main.zs");

    if (!file) {
        Zerror(FILE_NOT_FOUND + "'" + input + '"', NULL);
    }

    String contents;
    String line;
    while (getline(file, line)) {
        contents += line + '\n';
    }

    // Lex and Parse the code
    Lexer lexer(contents);
    vector<Token> tokens = lexer.tokenize();
    vector<ZPiece> parsed = parse(tokens, 0);
    file.close();

    // Convert to assembly
    AssemblyBuilder* builder = LZConvert(parsed);
    String code = builder->get();
    ofstream outfile(output + "/main.S");

    outfile << code;
    outfile.close();

    // Z# Header
    ofstream headerFile(output + "/zs.h");
    headerFile << HEADER_H__H;
}
