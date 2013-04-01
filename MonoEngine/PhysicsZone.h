//
//  PhysicsZone.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "ZoneBase.h"
#include "MathFwd.h"
#include "CMFwd.h"

namespace mono
{
    class PhysicsZone : public ZoneBase
    {
    public:

        void ForEachBody(const cm::BodyFunc& func);

    protected:
        
        PhysicsZone(const math::Vector2f& gravity);
        virtual void Accept(IRenderer& renderer);
                        
        void AddPhysicsEntityToLayer(mono::IPhysicsEntityPtr entity, LayerId layer);
        void RemovePhysicsEntity(mono::IPhysicsEntityPtr entity);
        
    private:
        
        struct PhysicsImpl;
        std::tr1::shared_ptr<PhysicsImpl> mPhysics;
    };
}

