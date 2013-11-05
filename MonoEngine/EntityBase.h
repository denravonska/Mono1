/*
 *  DrawableBase.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IEntity.h"
#include "MonoFwd.h"
#include "Vector2f.h"
#include <vector>

namespace mono
{

    class EntityBase : public IEntity
    {
    public:
    
        virtual const math::Vector2f& Position() const;
        virtual math::Quad BoundingBox() const;
        virtual bool RemoveMe() const;
        
        void AddChild(IEntityPtr child);
        void RemoveChild(IEntityPtr child);
    
    protected:
        
        EntityBase();

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;

        virtual void doDraw(IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);
		
        math::Vector2f mPosition;
        math::Vector2f mBasePoint;
        math::Vector2f mScale;
        float mRotation;
        
        std::vector<IEntityPtr> mChildren;
    };
    
}

