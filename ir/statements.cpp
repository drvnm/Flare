#include "statements.hpp"
#include "../visitors/visitor.hpp"

ACCEPT_VISITOR_METHOD(ExpressionStatement, llvm::Value*)
ACCEPT_VISITOR_METHOD(PrintStatement, llvm::Value*)
ACCEPT_VISITOR_METHOD(LetStatement, llvm::Value*)
ACCEPT_VISITOR_METHOD(BlockStatement, llvm::Value*)
ACCEPT_VISITOR_METHOD(IfStatement, llvm::Value*)
ACCEPT_VISITOR_METHOD(WhileStatement, llvm::Value*)