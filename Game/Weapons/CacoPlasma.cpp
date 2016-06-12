//
//  CacoPlasma.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 23/12/15.
//
//

#include "CacoPlasma.h"
#include "MathFunctions.h"
#include "Vector2f.h"

#include "EntityBase.h"
#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "IRenderer.h"
#include "EventHandler.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "SpawnEntityEvent.h"
#include "RemoveEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"

#include <cmath>

namespace
{
    class CacoExplosion : public mono::EntityBase
    {
    public:

        CacoExplosion(mono::EventHandler& event_handler, const math::Vector2f& position)
            : m_sprite("cacoexplosion.sprite")
        {
            mPosition = position;
            mScale = math::Vector2f(40, 40);

            const uint id = Id();

            const auto func = [&event_handler, id]() {
                event_handler.DispatchEvent(game::RemoveEntityByIdEvent(id));
            };

            m_sprite.SetAnimation(0, func);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite.doUpdate(delta);
        }

        mono::Sprite m_sprite;
    };

    class CacoBullet : public mono::PhysicsEntityBase, public cm::ICollisionHandler
    {
    public:

        CacoBullet(const math::Vector2f& position, mono::EventHandler& eventHandler)
            : m_eventHandler(eventHandler),
              m_sprite("cacobullet.sprite")
        {
            mScale = math::Vector2f(20.0f, 20.0f);

            mPhysicsObject.body = cm::Factory::CreateBody(1.0f, 1.0f);
            mPhysicsObject.body->SetPosition(position);
            mPhysicsObject.body->SetCollisionHandler(this);

            cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 7.0f, math::Vector2f(0.0f, 0.0f));

            mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
            mPhysicsObject.shapes.push_back(shape);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite.doUpdate(delta);
        }

        virtual void OnCollideWith(const cm::IBodyPtr& body)
        {
            const game::SpawnEntityEvent event(std::make_shared<CacoExplosion>(m_eventHandler, mPosition));
            m_eventHandler.DispatchEvent(event);
            m_eventHandler.DispatchEvent(game::RemoveEntityByIdEvent(Id()));
        }

        virtual void OnPostStep()
        { }

        mono::EventHandler& m_eventHandler;
        mono::Sprite m_sprite;
    };
}

using namespace game;

CacoPlasma::CacoPlasma(mono::EventHandler& eventHandler)
    : BaseWeapon(eventHandler)
{ }

void CacoPlasma::DoFire(const math::Vector2f& position, float direction) const
{
    const math::Vector2f unit(-std::sin(direction), std::cos(direction));
    const math::Vector2f& new_position = position + (unit * 60.0f);
    const math::Vector2f& impulse = unit * 300.0f;

    auto bullet = std::make_shared<CacoBullet>(new_position, m_eventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, new_position);

    m_eventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet));
}

int CacoPlasma::RoundsPerSecond() const
{
    return 2;
}
