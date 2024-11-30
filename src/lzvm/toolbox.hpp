#include "../gen/files.h"
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <variant>

// Part of the external project LLAMA Z VIRTUAL MACHINE

#ifndef ZXVK_HPP
#define ZXVK_HPP


using namespace std;

#define ZREG(pos) REG(pos)
#define ZBREG(byte) REG((int)byte)

/*
 *	Type Definitions
 */
using LZString = std::string;

class SUPERPAIRS {
  public:
	void add(LZString label) {
		position++;
		readable.push_back(label);
		formal.push_back(position);
	};
	int position;
	vector<LZString> readable;
	vector<int> formal;
};

enum ListSizeType { SIZED, UNLIMITED, NONE };

enum ClassPropertyType { PUBLIC, PRIVATE, STATIC };

enum Operation { ADD, SUB, MUL, DIV, EXP };

enum StorageType { TYPE, CLASS };

LZString classpropertytypestring(ClassPropertyType classPropertyType) {
	switch (classPropertyType) {
	case PUBLIC:
		return "public";

	case PRIVATE:
		return "private";

	case STATIC:
		return "static";
	};
	return "";
}

/*
 *  Useful Stuff
 */

template <typename T> LZString inthex(T i) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex
	       << i;
	return stream.str();
};

/*
 *  Assembly Builder
 */

class AssemblyBuilder {
  public:
	AssemblyBuilder(LZString file, int top) {
		if (top == 0) {
			this->head += RULES_H__H "\n"
			                         "#include \"zs.h\"\n";
			this->bss += ".bss"
			             "\n";
			this->data += ".data"
			              "\n";
			this->evaluated += ".text"
			                   "\n";
		}
		this->psize = 0;
	};
	class Value;
	class Operations;
	class ListType {
	  public:
		ListType(ListSizeType listType, int length = 0) {
			switch (listType) {
			case SIZED:
				size = length;
				type = SIZED;
				break;

			case UNLIMITED:
				type = UNLIMITED;
				break;
			case NONE:
				size = 1;
				type = NONE;
				break;
			}
		};
		int size;
		ListSizeType type;
	};
	class Type {
	  public:
		Type(LZString name, AssemblyBuilder *ctx) {
			if (name == "byte") {
				size = 1;
				direct = true;
			} else {
				size = 0;
				direct = false;
			}

			this->name = name;
			this->builder = ctx;
		};

		Type *addProperty(LZString label, Type *type) {
			size++;
			this->SuperPairProperties.add(label);
			this->SuperPairTypes.add(type->name);
			return this;
		};

		void lock() {
			locked = true;
			LZString text = "	TYPEDEF(" + name + ") = " + inthex(size) + "\n";
			builder->data += text;
		};

		void relock() {
			if (!locked) {
				this->lock();
			}
		};

		SUPERPAIRS getProperties() {
			if (!locked) {
				cout << "Attempted to use type before type was locked!\n";
				exit(1);
			}
			return SuperPairProperties;
		};

		bool locked;
		bool direct;
		LZString name;
		Type *alias;
		int size;

	  private:
		SUPERPAIRS SuperPairProperties;
		SUPERPAIRS SuperPairTypes;
		AssemblyBuilder *builder;
	};
	class TypeStore;
	Type *TypeGet(LZString name) {
		for (auto i = 0; i < (signed)types.size(); i++) {
			if (types[i]->name == name) {
				return types[i];
			}
		}
		return TypeGet("void");
	};
	class Variable {
	  public:
		Variable(TypeStore *type, LZString name, AssemblyBuilder *ctx) {
			this->type = type;
			this->name = name;
			this->builder = ctx;
			this->pid = ++this->builder->psize;
		};

		LZString name;
		TypeStore *type;
		int pid;
	  private:
		AssemblyBuilder *builder;
	};
	Variable *VarGet(LZString name) {
		for (auto i = 0; i < (signed)variables.size(); i++) {
			if (variables[i]->name == name) {
				return variables[i];
			}
		}
		return NULL;
	};
	class Class {
	  public:
		Class(LZString name, AssemblyBuilder *ctx) {
			this->name = name;
			builder = ctx;
		};
		Class *addProperty(LZString label, Type *type,
		                   ClassPropertyType propertyType) {
			size++;
			SuperPairProperties.add(label);
			SuperPairTypes.add(type->name);
			SuperPairPropertyTypes.add(classpropertytypestring(propertyType));
			return this;
		};
		LZString name;
		int size;

	  private:
		SUPERPAIRS SuperPairProperties;
		SUPERPAIRS SuperPairTypes;
		SUPERPAIRS SuperPairPropertyTypes;
		AssemblyBuilder *builder;
	};
	class Function {
	  public:
		Function(LZString name, Type *type, AssemblyBuilder *ctx) {
			this->name = name;
			funcType = type;
			builder = ctx;
			return;
		};

		Function *start() {
			builder->push("FUNCDEF(" + this->name + "):\n"
						  "    POOL CREATE ax\n"
						  "    POOL SWAP ax\n"
			);
			return this;
		}

		Function *end() { return this; };
		void lock() {
			locked = true;
			builder->push(evaluated);
		};
		void relock() {
			if (locked) {
				builder->push(evaluated);
			}
		};
		void add(LZString text) { evaluated += text; };
		LZString name;
		Type *funcType;
		bool locked;

	  private:
		LZString evaluated;
		AssemblyBuilder *builder;
	};
	class FunctionParameter {
	  public:
		FunctionParameter(Type *type, Value *value) {
			this->type = type;
			this->value = value;
		}

		Type *type;
		Value *value;
	};
	class FunctionCall {
	  public:
		FunctionCall(LZString name, vector<FunctionParameter *> parameters,
		             AssemblyBuilder *ctx) {
			this->name = name;
			this->parameters = parameters;
			this->builder = ctx;
		};

		void lock() {
			locked = true;
			this->target = builder->FuncGet(this->name);
			if (this->target == NULL && !unstable) {
				cout << "** " + this->name + " ** is unstable!";
			}
			this->evaluated += "	PARAM CLEAR\n";
			for (TINT i = 0; i < this->parameters.size(); i++) {
				this->evaluated += "	PARAM PUSH " +
				                   this->parameters[i]->value->evaluate() +
				                   "\n";
			}

			this->evaluated += "	FUNCTION CALL " + this->name +
			                   "\n"; // Implement parameters later!
			cout << this->builder->getEvaluated();
			this->builder->push(this->evaluated);
		}

		void add(LZString text) { evaluated += text; };

		LZString name;
		vector<FunctionParameter *> parameters;
		Function *target;
		bool locked;
		bool unstable; // Remove this later!
	  private:
		LZString evaluated;
		AssemblyBuilder *builder;
	};
	Function *FuncGet(LZString name) {
		for (auto i = 0; i < (signed)functions.size(); i++) {
			if (functions[i]->name == name) {
				return functions[i];
			}
		}
		return FuncGet("void");
	};
	class TypeStore {
	  public:
		TypeStore(Type *type, ListType *list) {
			this->type = type;
			this->list = list;
			this->storageType = StorageType::TYPE;
		}
		TypeStore(Class *class_, ListType *list) {
			this->type = type;
			this->list = list;
			this->storageType = StorageType::CLASS;
		}
		Type *type;
		Class *class_;
		StorageType storageType;
		ListType *list;
	};
	using VValue = variant<Variable*, Function*>;
	class Value {
	  public:
		Value(VValue value) {
			this->values.emplace_back(value);
		}
		
		LZString evaluate() {
			AssemblyBuilder *access = new AssemblyBuilder("?", 0);
			Operations *op = new AssemblyBuilder::Operations;

			LZString evaluated;
			if (this->values.size() > 1) {
				evaluated = op->op(this->operation, this->values[0], this->values[1]);
			} else {
				evaluated = op->get(this->values[0]);
			}
			
			this->evaluated = evaluated;
		}
		Value *add(VValue value) {
			values.emplace_back(value);
			operation = Operation::ADD;
			return this;
		}
	  private:
		vector<VValue> values;
		Operation operation;
		LZString evaluated;
	};
	class Operations {
	  public:
	  	Operations() {

	  	}
		LZString evaluated;
		void reset() {
			Operations::evaluated = "OPERATION ";
		}
		LZString get(AssemblyBuilder::VValue v) {
			Operations::reset();
			visit([&](auto&& value) {
				if constexpr (is_same_v<decltype(value), AssemblyBuilder::Variable*>) {
					Operations::evaluated += "$" + to_string(value.pid);
				} else if constexpr (is_same_v<decltype(value), AssemblyBuilder::Function*>) {
					// Define later
				}
			}, v);
		}
		LZString add(AssemblyBuilder::VValue a, AssemblyBuilder::VValue b) {
			Operations::reset();
			Operations::evaluated += "ADD ";
			visit([&](auto&& value_a) {
				if constexpr (is_same_v<decltype(value_a), AssemblyBuilder::Variable*>) {
					Operations::evaluated += "$" + to_string(value_a.pid);
				} else if constexpr (is_same_v<decltype(value_a), AssemblyBuilder::Function*>) {
					// Define later
				}
			}, a);
			visit([&](auto&& value_b) {
				if constexpr (is_same_v<decltype(value_b), AssemblyBuilder::Variable*>) {
					Operations::evaluated += ", $" + to_string(value_b.pid);
				} else if constexpr (is_same_v<decltype(value_b), AssemblyBuilder::Function*>) {
					// Define later
				}
			}, b);
			return Operations::evaluated;
		}
		LZString op(Operation operation, AssemblyBuilder::VValue a, AssemblyBuilder::VValue b) {
			Operations::reset();
			switch (operation) {
				case Operation::ADD:
					return Operations::add(a, b);

			}
		}
	};
	Function *function(LZString name, Type *type) {
		functions.push_back(new Function(name, type, this));
		return functions[functions.size() - 1];
	};
	Function *function(LZString name, LZString type) {
		functions.push_back(new Function(name, TypeGet(type), this));
		return functions[functions.size() - 1];
	};
	FunctionCall *functionCall(LZString name,
	                           vector<FunctionParameter *> parameters) {
		FunctionCall *functionCall = new FunctionCall(name, parameters, this);
		return functionCall;
	};
	Type *type(LZString name) {
		types.push_back(new Type(name, this));
		return types[types.size() - 1];
	};
	TypeStore *typeStore(Type *type, int size = 0) {
		TypeStore *typeStore;
		if (size = -1) {
			typeStore =
			    new TypeStore(type, new ListType(ListSizeType::UNLIMITED));
		} else if (size == 0) {
			typeStore = new TypeStore(type, new ListType(ListSizeType::NONE));
		} else {
			typeStore =
			    new TypeStore(type, new ListType(ListSizeType::SIZED, size));
		}
		return typeStore;
	};
	Variable *variable(TypeStore *type, LZString name) {
		variables.push_back(new Variable(type, name, this));
		return variables[variables.size() - 1];
	};
	void addBSS(LZString text) { bss += text; };
	void addData(LZString text) { data += text; };
	void push(LZString text) {
		edge += text;
		cout << text;
	};
	void end() {
		evaluated += edge + ".global main"
		                    "\n"
		                    "main:"
		                    "\n"
		                    "	ZS INIT"
		                    "\n"
		                    "	FUNCTION CALL \"Main\""
		                    "\n"
		                    "	ZS END"
		                    "\n";
	};
	void subEnd() { evaluated += edge; };
	void defs() {
		type("void")->lock();
		type("byte")->lock();

		function("void", "void");
	};
	LZString get() { return head + bss + data + evaluated; };
	LZString subGet() { return edge; };
	LZString getEvaluated() { return evaluated; };
	void import(AssemblyBuilder *builder) {
		int oldFuncSize = functions.size();
		int oldTypeSize = types.size();
		functions.insert(functions.end(), builder->functions.begin(),
		                 builder->functions.end());
		types.insert(types.end(), builder->types.begin(), builder->types.end());
		for (int i = oldTypeSize; i < types.size(); i++) {
			types[i]->relock();
		}
		for (int i = oldFuncSize; i < functions.size(); i++) {
			if (functions[i]->name != "void")
				functions[i]->relock();
		}
	};

	LZString data;
	vector<Function *> functions;
	vector<Type *> types;
	vector<Variable *> variables;
	int count;
	int psize;

  private:
	LZString evaluated;
	LZString head;
	LZString bss;
	LZString edge;
};

#endif