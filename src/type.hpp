#include "ex.hpp"

#ifndef TYPE_HPP
#define TYPE_HPP

enum ZClassPropertyType {
    ZPublic,
    ZPrivate,
    ZStatic,
};

typedef struct {
    String label;
    String type;
    ZClassPropertyType classPropertyType;
} ZValue;

typedef struct {
    vector<ZValue> values;
} ZStruct;

typedef struct {
    String name;
    String alias;
    ZStruct structure;
} ZType;

typedef struct {
    String name;
    ZStruct structure;
} ZClass;

typedef struct {
    String name;
    String type;
    Bool constant;
    Bool parameter;
} ZVariable;

typedef struct {
    vector<String> values;
    vector<String> symbols;
} ZExpression;

typedef struct {
    String name;
    String type;
    ZExpression expression;
} ZInitialize;

typedef struct {
    String name;
    String type;
    ZExpression expression;
} ZSet;


typedef struct ZFunction {
    String name;
    String type;
} ZFunction;

typedef struct ZParameter {
    String name;
    String type;
} ZParameter;

typedef struct {
    String name;
    vector<ZParameter> parameters;
} ZFunctionCall;

typedef struct {
    String value;
    StringType type;
} ZStringLiteral;

typedef struct {
    String source;
    String import;
} ZImport;

enum ZPieceType {
    ZType_t,
    ZVariable_t,
    ZFunction_t,
    ZFunctionCall_t,
    ZClass_t,
    ZInitialize_t,
    ZStringLiteral_t,
    ZSet_t,
    ZImport_t
};

typedef struct {
    ZType type;
    ZVariable variable;
    ZFunction function;
    ZFunctionCall functionCall;
    ZClass class_;
    ZInitialize initalize;
    ZStringLiteral stringLiteral;
    ZSet set;
    ZImport import;

    int depth;
    ZPieceType ptype;
} ZPiece;



using namespace std;

// Helper function to print ZClassPropertyType
string getClassPropertyType(ZClassPropertyType type) {
    switch (type) {
        case ZPublic: return "Public";
        case ZPrivate: return "Private";
        case ZStatic: return "Static";
        default: return "Unknown";
    }
}

// Function to print a vector of strings
void printStringVector(const vector<String>& vec, const string& label) {
    cout << label << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// Print function for ZPiece
void printZPiece(const ZPiece& piece) {
    cout << "ZPiece {" << endl;
    cout << "  Depth: " << piece.depth << endl;

    // Handle the specific type of ZPiece
    switch (piece.ptype) {
        case ZType_t:
            cout << "  Type: ZType" << endl;
            cout << "  Name: " << piece.type.name << endl;
            cout << "  Alias: " << piece.type.alias << endl;
            cout << "  Structure: {" << endl;
            for (const auto& value : piece.type.structure.values) {
                cout << "    Label: " << value.label
                     << ", Type: " << value.type
                     << ", ClassPropertyType: " << getClassPropertyType(value.classPropertyType)
                     << endl;
            }
            cout << "  }" << endl;
            break;

        case ZVariable_t:
            cout << "  Type: ZVariable" << endl;
            cout << "  Name: " << piece.variable.name << endl;
            cout << "  Variable Type: " << piece.variable.type << endl;
            cout << "  Constant: " << (piece.variable.constant ? "True" : "False") << endl;
            cout << "  Parameter: " << (piece.variable.parameter ? "True" : "False") << endl;
            break;

        case ZFunction_t:
            cout << "  Type: ZFunction" << endl;
            cout << "  Name: " << piece.function.name << endl;
            cout << "  Return Type: " << piece.function.type << endl;
            break;

        case ZFunctionCall_t:
            cout << "  Type: ZFunctionCall" << endl;
            cout << "  Name: " << piece.functionCall.name << endl;
            cout << "  Parameters: {" << endl;
            for (const auto& param : piece.functionCall.parameters) {
                cout << "    Name: " << param.name << ", Type: " << param.type << endl;
            }
            cout << "  }" << endl;
            break;

        case ZClass_t:
            cout << "  Type: ZClass" << endl;
            cout << "  Name: " << piece.class_.name << endl;
            cout << "  Structure: {" << endl;
            for (const auto& value : piece.class_.structure.values) {
                cout << "    Label: " << value.label
                     << ", Type: " << value.type
                     << ", ClassPropertyType: " << getClassPropertyType(value.classPropertyType)
                     << endl;
            }
            cout << "  }" << endl;
            break;

        case ZInitialize_t:
            cout << "  Type: ZInitialize" << endl;
            cout << "  Name: " << piece.initalize.name << endl;
            cout << "  Variable Type: " << piece.initalize.type << endl;
            printStringVector(piece.initalize.expression.values, "  Expression Values");
            printStringVector(piece.initalize.expression.symbols, "  Expression Symbols");
            break;

        case ZStringLiteral_t:
            cout << "  Type: ZStringLiteral" << endl;
            cout << "  Value: " << piece.stringLiteral.value << endl;
            cout << "  String Type: " << piece.stringLiteral.type << endl;
            break;

        case ZSet_t:
            cout << "  Type: ZSet" << endl;
            cout << "  Name: " << piece.set.name << endl;
            cout << "  Variable Type: " << piece.set.type << endl;
            printStringVector(piece.set.expression.values, "  Expression Values");
            printStringVector(piece.set.expression.symbols, "  Expression Symbols");
            break;

        case ZImport_t:
            cout << "  Type: ZImport" << endl;
            cout << "  Source: " << piece.import.source << endl;
            cout << "  Import: " << piece.import.import << endl;
            break;

        default:
            cout << "  Unknown ZPiece type." << endl;
            break;
    }

    cout << "}" << endl;
}


#endif