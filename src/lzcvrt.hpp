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
                for (TINT j = 0; i < piece.type.structure.values.size(); i++) {
                    type->addProperty(piece.type.structure.values[i].label, builder->TypeGet(piece.type.structure.values[i].type));
                }
                type->lock();
            }
            case ZFunction_t: {
                AssemblyBuilder::Function *function = builder->function(piece.function.name, piece.function.type);
                int currentDepth = piece.depth;
                vector<ZPiece> functionPieces;
                int j = i + 1;
                while (pieces[j].depth > currentDepth && j <= pieces.size() - 1) {
                    functionPieces.push_back(pieces[j]);
                    j++;
                }
                AssemblyBuilder *mini = LZConvert(functionPieces, level + 1, builder);
                mini->subEnd();
                builder->push(mini->subGet());
                i += j;
                function->end();
                function->lock();
            }
        }
    }
    if (level == 0) builder->end();
    cout << builder->get();
    return builder;
};

#endif
