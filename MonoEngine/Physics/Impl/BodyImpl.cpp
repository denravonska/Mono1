
#include "BodyImpl.h"
#include "Math/Vector.h"

#include "chipmunk/chipmunk.h"

using namespace cm;

BodyImpl::BodyImpl(cpBody* body)
    : m_body_handle(body),
        m_collision_handler(nullptr)
{ }

BodyImpl::~BodyImpl()
{
    cpBodyFree(m_body_handle);
}

bool BodyImpl::IsStatic() const 
{
    return cpBodyGetType(m_body_handle) == CP_BODY_TYPE_STATIC;
}

void BodyImpl::SetMass(float mass) 
{
    cpBodySetMass(m_body_handle, mass);
}

float BodyImpl::GetMass() const 
{
    return cpBodyGetMass(m_body_handle);
}

void BodyImpl::SetAngle(float angle) 
{
    cpBodySetAngle(m_body_handle, angle);
}

float BodyImpl::GetAngle() const 
{
    return cpBodyGetAngle(m_body_handle);
}

void BodyImpl::SetMoment(float moment) 
{
    cpBodySetMoment(m_body_handle, moment);
}

float BodyImpl::GetMoment() const 
{
    return cpBodyGetMoment(m_body_handle);
}

void BodyImpl::SetPosition(const math::Vector& position) 
{
    cpBodySetPosition(m_body_handle, cpv(position.x, position.y));
}

math::Vector BodyImpl::GetPosition() const 
{
    const cpVect& position = cpBodyGetPosition(m_body_handle);
    return math::Vector(position.x, position.y);
}

void BodyImpl::ApplyForce(const math::Vector& force, const math::Vector& offset) 
{
    cpBodyApplyForceAtWorldPoint(m_body_handle, cpv(force.y, force.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyLocalForce(const math::Vector& force, const math::Vector& offset) 
{
    cpBodyApplyForceAtLocalPoint(m_body_handle, cpv(force.y, force.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) 
{
    cpBodyApplyImpulseAtWorldPoint(m_body_handle, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
}

void BodyImpl::ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) 
{
    cpBodyApplyImpulseAtLocalPoint(m_body_handle, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
}

void BodyImpl::SetVelocity(const math::Vector& velocity) 
{
    cpBodySetVelocity(m_body_handle, cpv(velocity.x, velocity.y));
}

void BodyImpl::ResetForces() 
{
    cpBodySetForce(m_body_handle, cpvzero);
}

void BodyImpl::SetCollisionHandler(mono::ICollisionHandler* collision_handler) 
{
    m_collision_handler = collision_handler;
}

void BodyImpl::OnCollideWith(const mono::IBodyPtr& body, unsigned int categories) 
{
    if(m_collision_handler)
        m_collision_handler->OnCollideWith(body, categories);
}

void BodyImpl::SetNoDamping() 
{
    const cpBodyVelocityFunc null_func = [](cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
    { };

    cpBodySetVelocityUpdateFunc(m_body_handle, null_func);
}

cpBody* BodyImpl::Handle() 
{
    return m_body_handle;
}
