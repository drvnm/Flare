#pragma once

#define ACCEPT_VISITOR_METHOD(CLASS, RTYPE)        \
    RTYPE CLASS::accept(BaseVisitor &visitor) \
    {                                       \
        return visitor.visit(*this);        \
    }

#define ACCEPT_VISITOR_METHOD_HEADER(RTYPE) \
    RTYPE accept(BaseVisitor &visitor) override;
