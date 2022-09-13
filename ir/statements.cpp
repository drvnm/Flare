#include "statements.hpp"
#include "../visitors/visitor.hpp"

int ExpressionStatement::accept(BaseVisitor& visitor) {
    return visitor.visit(*this);
}