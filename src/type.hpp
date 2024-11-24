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

typedef struct ZScope ZScope;

typedef struct ZFunction {
    String name;
    String type;
    ZScope* scope;
} ZFunction;

typedef struct {
    String name;
    String type;
    Bool asynchronous;
    vector<String> parameters;
} ZFunctionCall;

typedef struct {
    String value;
    StringType type;
} ZStringLiteral;

struct ZScope {
    vector<ZVariable> variables;
    vector<ZFunction> functions;
    String name;
};

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

#endif