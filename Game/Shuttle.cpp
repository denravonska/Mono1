//
//  Shuttle.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Shuttle.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "Meteor.h"
#include "FireBullet.h"
#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"
#include "IRenderer.h"

#include "MathFunctions.h"

using namespace game;

namespace constants
{    
    enum
    {
        IDLE = 0,
        THRUSTING
    };
}


Shuttle::Shuttle(float x, float y)
    : mSprite("shuttle.sprite"),
      mController(this)
{
    mPosition = math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(15.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);
    mPhysicsObject.body->SetCollisionHandler(this);

    //cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 5.0f, math::Vector2f(0.0f, 0.0f), false);
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, mScale, mScale);
    shape->SetElasticity(0.1f);
    
    mPhysicsObject.shapes.push_back(shape);    
    
    mSprite.SetAnimation(constants::THRUSTING);
}

void Shuttle::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void Shuttle::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Shuttle::OnCollideWith(cm::IBodyPtr body)
{

}

void Shuttle::Fire()
{
    const math::Vector2f unit(-std::sin(math::DegToRad(mRotation)), std::cos(math::DegToRad(mRotation)));
    const math::Vector2f position = mPosition + (unit * 15.0f);
    const math::Vector2f speed = unit * 200.0f;
    const game::SpawnEntityEvent event(mono::IEntityPtr(new FireBullet(position, mRotation, speed)));

    //std::tr1::shared_ptr<Meteor> entity(new Meteor(mPosition.mX, mPosition.mY));
    //const game::SpawnPhysicsEntityEvent event(entity);  
    //entity->mPhysicsObject.body->ApplyForce(math::Vector2f(200.0f, 200.0f), math::Vector2f(0.0, 0.0));
    
    mono::EventHandler::DispatchEvent(event);
}



