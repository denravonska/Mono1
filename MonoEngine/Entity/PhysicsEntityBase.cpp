
#include "PhysicsEntityBase.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Physics/IBody.h"
#include "Rendering/IRenderer.h"
#include "System/UID.h"
#include "Utils.h"

using namespace mono;

PhysicsEntityBase::PhysicsEntityBase()
    : m_uid(System::CreateUID()),
      m_properties(0),
      m_scale(1.0f, 1.0f),
      m_rotation(0.0f)
{ }

const math::Vector& PhysicsEntityBase::Position() const
{
    return m_position;
}

const math::Vector& PhysicsEntityBase::BasePoint() const
{
    return math::ZeroVec;
}

float PhysicsEntityBase::Rotation() const
{
    return m_rotation;
}

const math::Vector& PhysicsEntityBase::Scale() const
{
    return m_scale;
}

void PhysicsEntityBase::SetPosition(const math::Vector& position)
{
    m_position = position;
    m_physics.body->SetPosition(position);
}

void PhysicsEntityBase::SetRotation(float rotation)
{
    m_rotation = rotation;
    m_physics.body->SetAngle(rotation);
}

void PhysicsEntityBase::SetBasePoint(const math::Vector& base_point)
{ }

void PhysicsEntityBase::SetScale(const math::Vector& scale)
{
    m_scale = scale;
}

math::Quad PhysicsEntityBase::BoundingBox() const
{
    const math::Vector& halfScale = m_scale / 2.0f;
    math::Quad thisbb(m_position - halfScale, m_position + halfScale);

    for(const auto& child : m_children)
    {
        math::Quad childbb = (child->BoundingBox() * m_scale);
        childbb.mA += m_position;
        childbb.mB += m_position;
        thisbb |= childbb;
    }
            
    return thisbb;
}

mono::PhysicsData& PhysicsEntityBase::GetPhysics()
{
    return m_physics;
}

unsigned int PhysicsEntityBase::Id() const
{
    return m_uid;
}

void PhysicsEntityBase::SetProperty(unsigned int property)
{
    m_properties |= property;
}

bool PhysicsEntityBase::HasProperty(unsigned int property) const
{
    return m_properties & property;
}

void PhysicsEntityBase::doDraw(IRenderer& renderer) const
{
    const math::Matrix& transform = renderer.GetCurrentTransform() * Transformation();

    for(const auto& child : m_children)
    {
        renderer.PushNewTransform(transform);
        child->doDraw(renderer);
    }

    renderer.PushNewTransform(transform);

    Draw(renderer);
}

void PhysicsEntityBase::doUpdate(unsigned int delta)
{
    m_position = m_physics.body->GetPosition();
    m_rotation = m_physics.body->GetAngle();

    for(auto& child : m_children)
        child->doUpdate(delta);

    Update(delta);
}

math::Matrix PhysicsEntityBase::Transformation() const
{
    math::Matrix translation;
    math::Translate(translation, m_position);

    math::Matrix rotation;
    math::RotateZ(rotation, m_rotation);

    math::Matrix scale;
    math::ScaleXY(scale, m_scale);

    math::Matrix transform;
    transform *= translation;
    transform *= rotation;
    transform *= scale;

    return transform;
}

void PhysicsEntityBase::AddChild(const IEntityPtr& child)
{
    m_children.push_back(child);
}

void PhysicsEntityBase::RemoveChild(const IEntityPtr& child)
{
    mono::remove(m_children, child);
}

