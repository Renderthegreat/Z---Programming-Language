#include "ex.hpp"
#include "lzvm/toolbox.hpp"
#include "type.hpp"

#ifndef LZCVRT_HPP
#define LZCVRT_HPP

AssemblyBuilder* LZConvert(vector<ZPiece> pieces, int level = 0, AssemblyBuilder *parent = NULL) {
    AssemblyBuilder *builder = new AssemblyBuilder("Main.zs", level);
    if (parent) {
        builder->import(parent);
    }
    builder->defs();
    for (TINT i = 0; i < pieces.size(); i++) {
        ZPiece piece = pieces[i];
        switch (piece.ptype) {
            case ZType_t: {
                AssemblyBuilder::Type *type = builder->type(piece.type.name);
                for (TINT j = 0; i < piece.type.structure.values.size(); j++) {
                    type->addProperty(piece.type.structure.values[j].label, builder->TypeGet(piece.type.structure.values[j].type));
                }
                type->lock();
            }
            case ZFunction_t: {
                AssemblyBuilder::Function *function = builder->function(piece.function.name, piece.function.type);
                function->start();
                int currentDepth = piece.depth;
                vector<ZPiece> functionPieces;
                int j = i + 1;
                while (pieces[j].depth > currentDepth && j < pieces.size()) {
                    functionPieces.push_back(pieces[j]);
                    j++;
                }
                AssemblyBuilder *mini = LZConvert(functionPieces, level + 1, builder);
                mini->subEnd();
                builder->push(mini->subGet());
                i += j - 1;
                function->end();
                function->lock();
            }
            case ZFunctionCall_t: {
                vector<AssemblyBuilder::FunctionParameter *> functionParameters;
                for (TINT j = 0; j < piece.functionCall.parameters.size(); j++) {
                    functionParameters.push_back(new AssemblyBuilder::FunctionParameter(builder->TypeGet(piece.functionCall.parameters[j].type), new AssemblyBuilder::Value(builder->variable(builder->typeStore(builder->TypeGet("byte"), 0), piece.functionCall.name))));
                }
                AssemblyBuilder::FunctionCall *functionCall = builder->functionCall(piece.functionCall.name, functionParameters);
                if(functionCall->name != String()) {
                    functionCall->lock();
                }
            }
            case ZVariable_t: {
                
            }
        }
    }
    if (level == 0) builder->end();
    return builder;
};

#endif
