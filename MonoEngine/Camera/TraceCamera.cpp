//
//  TraceCamera.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TraceCamera.h"
#include "IEntity.h"
#include "Vector2f.h"
#include "MathFunctions.h"

#include <cmath>

using namespace mono;

namespace constants
{
    constexpr float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : mController(this),
      mViewport(0.0f, 0.0f, width, height),
      mTargetViewport(mViewport)
{ }

void TraceCamera::Update(unsigned int delta)
{
    const float change = (mTargetViewport.mB.mX - mViewport.mB.mX);
    const float xzero = std::floor(std::abs(change));

    if(xzero != 0.0f)
    {
        const float aspect = mViewport.mB.mX / mViewport.mB.mY;
        math::ResizeQuad(mViewport, change * 0.1f, aspect);
    }
    
    if(mEntity)
    {
        const math::Vector2f& targetPosition = mEntity->Position() - (mViewport.mB * 0.5f);
        const math::Vector2f& diff = targetPosition - mViewport.mA;
    
        const math::Vector2f& move = diff * (delta * constants::SPEED);
        mViewport.mA += move;
    }
}

void TraceCamera::Follow(const mono::IEntityPtr entity)
{
    mEntity = entity;
}

void TraceCamera::Unfollow()
{
    mEntity = nullptr;
}

const math::Quad& TraceCamera::GetViewport() const
{
    return mViewport;
}

math::Quad& TraceCamera::GetViewport()
{
    return mViewport;
}

void TraceCamera::SetTargetViewport(const math::Quad& target)
{
    mTargetViewport = target;
}

void TraceCamera::SetPosition(const math::Vector2f& position)
{
    const math::Vector2f& xy = position - (mViewport.mB * 0.5f);
    mViewport.mA = xy;
}



