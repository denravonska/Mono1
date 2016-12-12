
#pragma once

#include "IDrawable.h"
#include "IUpdatable.h"
#include "System/SysTypes.h"
#include "Math/MathFwd.h"

namespace mono
{
    struct IEntity : public IDrawable, public IUpdatable
    {
        virtual ~IEntity()
        { }

        virtual const math::Vector2f& Position() const = 0;
        virtual const math::Vector2f& BasePoint() const = 0;
        virtual const math::Vector2f& Scale() const = 0;

        virtual void SetPosition(const math::Vector2f& position) = 0;
        virtual void SetScale(const math::Vector2f& scale) = 0;

        // Rotation in radians
        virtual float Rotation() const = 0;
        virtual void SetRotation(float rotation) = 0;

        virtual math::Matrix Transformation() const = 0;

        // Unique id
        virtual uint Id() const = 0;

        virtual void SetProperty(uint property) = 0;
        virtual bool HasProperty(uint property) const = 0;
    };
}
