//
//  CMShapeFactory.h
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"
#include "MathFwd.h"
#include <vector>

namespace mono
{            
    class PhysicsFactory
    {
    public:
        
        //! Static Body
        static IBodyPtr CreateStaticBody();

        //! Body
        //! Inertia is the body's resistance to change in rotation, pass in INFINITY
        //! if no rotation is wanted.
        static IBodyPtr CreateBody(float mass, float inertia);
        
        //! Circle shape
        static IShapePtr CreateShape(IBodyPtr body, float radius, const math::Vector2f& offset);    
        
        //! Box shape
        static IShapePtr CreateShape(IBodyPtr body, float width, float height);

        //! Segment shape
        static IShapePtr CreateShape(IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius);
        
        //! Polygon shape
        static IShapePtr CreateShape(IBodyPtr body, const std::vector<math::Vector2f>& vertices, const math::Vector2f& offset);
    };
}



