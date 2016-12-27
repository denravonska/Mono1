//
//  CMSpace.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include <vector>
#include <functional>

struct cpSpace;
struct cpArbiter;

namespace mono
{
    class Space
    {
    public:
        
        Space(const math::Vector& gravity, float damping);
        ~Space();
        
        void Tick(float delta);
        
        void Add(const IBodyPtr& body);
        void Add(const IShapePtr& shape);
        void Add(const IConstraintPtr& constraint);
        
        void Remove(const IBodyPtr& body);
        void Remove(const IShapePtr& shape);
        void Remove(const IConstraintPtr& constraint);
        
        void ForEachBody(const BodyFunc& func);
        IBodyPtr QueryFirst(const math::Vector& start, const math::Vector& end);
        
    private:
        
        bool OnCollision(cpArbiter* arb);
        void OnPostStep(cpArbiter* arb);
        
        void DoForEachFuncOnBody(cpBody* body);

        BodyFunc mForEachFunc;
        
        cpSpace* mSpace;
        std::vector<IBodyPtr> mBodies;        
    };
    
}
