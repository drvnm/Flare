#include "expressions.hpp"
#include "../visitors/visitor.hpp"

ACCEPT_VISITOR_METHOD(IntExpression, llvm::Value*)
ACCEPT_VISITOR_METHOD(BinaryExpression, llvm::Value*)
ACCEPT_VISITOR_METHOD(VarExpression, llvm::Value*)
