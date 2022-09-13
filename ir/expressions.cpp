#include "expressions.hpp"
#include "../visitors/visitor.hpp"



int BinaryExpression::accept(BaseVisitor& visitor)  {
    return visitor.visit(*this);
}   