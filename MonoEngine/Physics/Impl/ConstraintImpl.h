
#pragma once

#include "Physics/IConstraint.h"

namespace cm
{
    class ConstraintImpl : public mono::IConstraint
    {
    public:

        ConstraintImpl(cpConstraint* constraint);
        ~ConstraintImpl();

        void SetMaxForce(float force) override;
        float GetMaxForce() const override;
        void SetMaxBias(float bias) override;
        float GetMaxBias() const override;
        void SetErrorBias(float bias) override;
        float GetErrorBias() const override;
        cpConstraint* Handle() override;

    private:

        cpConstraint* m_constraint;
    };    
}
