//
//  CMSpace.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CMSpace.h"
#include "CMFactory.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "Vector2f.h"
#include "Utils.h"

#include "chipmunk.h"

#include <stdexcept>

using namespace cm;

namespace
{
    BodyFunc bodyFunction;
    std::vector<IBodyPtr>* bodyCollection;
    
    void CMBodyFunction(cpBody* body, void* data)
    {
        for(std::vector<IBodyPtr>::iterator it = bodyCollection->begin(), end = bodyCollection->end(); it != end; ++it)
        {
            IBodyPtr bodyPtr = *it;
            if(body == bodyPtr->Body())
                bodyFunction(bodyPtr);
        }
    }
    
    std::tr1::function<bool (cpArbiter*, void*)> beginFunc;
    cpBool CMCollisionBegin(cpArbiter *arb, cpSpace *space, void *data)
    {
        return beginFunc(arb, data);
    }
}

Space::Space(const math::Vector2f& gravity)
{
    mSpace = cpSpaceNew();
    cpSpaceSetGravity(mSpace, cpv(gravity.mX, gravity.mY));
    
    using namespace std::tr1::placeholders;
    beginFunc = std::tr1::bind(&Space::OnCollision, this, _1, _2);
    cpSpaceAddCollisionHandler(mSpace, 0, 0, CMCollisionBegin, 0, 0, 0, 0);
}

Space::~Space()
{
    cpSpaceDestroy(mSpace);
}

void Space::Tick(float delta)
{
    cpSpaceStep(mSpace, delta);
}

void Space::AddBody(IBodyPtr body)
{
    if(!body->IsStatic())
        cpSpaceAddBody(mSpace, body->Body());
    mBodies.push_back(body);
}
void Space::RemoveBody(IBodyPtr body)
{
    if(!body->IsStatic())
        cpSpaceRemoveBody(mSpace, body->Body());
    
    const bool result = mono::FindAndRemove(mBodies, body);
    if(!result)
        throw std::runtime_error("Unable to remove body from collection");
}

void Space::AddShape(IShapePtr shape)
{
    cpSpaceAddShape(mSpace, shape->Shape());
}
void Space::RemoveShape(IShapePtr shape)
{
    cpSpaceRemoveShape(mSpace, shape->Shape());
}

void Space::ForEachBody(const BodyFunc& func)
{
    bodyFunction = func;
    bodyCollection = &mBodies;
    
    cpSpaceEachBody(mSpace, CMBodyFunction, 0);
}

bool Space::OnCollision(cpArbiter* arb, void* data)
{
    cpBody* b1 = 0;
    cpBody* b2 = 0;
    cpArbiterGetBodies(arb, &b1, &b2);
    
    IBodyPtr first;
    IBodyPtr second;
    
    for(std::vector<IBodyPtr>::iterator it = mBodies.begin(), end = mBodies.end(); it != end; ++it)
    {
        IBodyPtr body = *it;
        if(body->Body() == b1)
            first = body;
        else if(body->Body() == b2)
            second = body;
        
        if(first && second)
            break;
    }
    
    if(first && second)
    {
        first->OnCollideWith(second);
        second->OnCollideWith(first);
    }
    
    return true;
}

