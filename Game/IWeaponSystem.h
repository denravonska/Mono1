//
//  IWeaponSystem.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "MathFwd.h"

namespace game
{
    class IWeaponSystem
    {
    public:

        virtual ~IWeaponSystem()
        { }

        virtual void Fire(const math::Vector2f& position, float direction) = 0;
        virtual int RoundsPerSecond() const = 0;
        virtual const char* SpriteFile() const = 0;
    };
}
