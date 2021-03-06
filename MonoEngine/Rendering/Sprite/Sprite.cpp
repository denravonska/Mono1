
#include "Sprite.h"
#include "Rendering/Texture/ITexture.h"

#include <stdexcept>
#include <cassert>
#include <cstring>

using namespace mono;

Sprite::Sprite()
{ }

Sprite::Sprite(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
{
    Init(texture, coordinates);
}

void Sprite::Init(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
{
    m_texture = texture;
    m_textureCoordinates = coordinates;

    m_texture_quad = math::Quad(math::INF, -math::INF, -math::INF, math::INF);
    for(const math::Quad& quad : coordinates)
    {
        m_texture_quad.mA.x = std::min(m_texture_quad.mA.x, quad.mA.x);
        m_texture_quad.mA.y = std::max(m_texture_quad.mA.y, quad.mA.y);
        m_texture_quad.mB.x = std::max(m_texture_quad.mB.x, quad.mB.x);
        m_texture_quad.mB.y = std::min(m_texture_quad.mB.y, quad.mB.y);
    }
}

ITexturePtr Sprite::GetTexture() const
{
    return m_texture;
}

math::Quad Sprite::GetTextureCoords() const
{
    const AnimationSequence& anim = m_animations[m_activeAnimation];
    math::Quad coords = m_textureCoordinates.at(anim.Frame());

    if(m_flip_horizontal)
        std::swap(coords.mA.x, coords.mB.x);
    
    if(m_flip_vertical)
        std::swap(coords.mA.y, coords.mB.y);

    return coords;
}

const math::Quad& Sprite::GetFullTexureCoords() const
{
    return m_texture_quad;
}

const Color::RGBA& Sprite::GetShade() const
{
    return m_color;
}

void Sprite::SetShade(const mono::Color::RGBA& color)
{
    m_color = color;
}

void Sprite::SetHorizontalDirection(HorizontalDirection direction)
{
    m_flip_horizontal = (direction == HorizontalDirection::LEFT);
}

void Sprite::SetVerticalDirection(VerticalDirection direction)
{
    m_flip_vertical = (direction == VerticalDirection::DOWN);
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = m_animations[m_activeAnimation];
    anim.Update(delta);

    if(anim.Finished() && m_callback)
        m_callback();
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(const char* name)
{
    SetAnimation(name, nullptr);
}

void Sprite::SetAnimation(const char* name, const std::function<void ()>& func)
{
    const int index = FindAnimationByName(name);
    assert(index != -1);

    SetAnimation(index, func);
}

void Sprite::SetAnimation(int id, const std::function<void ()>& func)
{
    const bool same_id = (id == m_activeAnimation);

    m_activeAnimation = id;
    m_callback = func;

    if(!same_id)
        m_animations[id].Restart();
}

void Sprite::RestartAnimation()
{
    m_animations[m_activeAnimation].Restart();
}

int Sprite::DefineAnimation(const std::string& name, const std::vector<int>& frames, bool loop)
{
    const bool even = (frames.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");

    AnimationSequence sequence(name.c_str(), loop);

    for(auto it = frames.begin(), end = frames.end(); it != end; ++it)
    {
        const int frame = *it;
        ++it;
        const int duration = *it;

        sequence.AddFrame(frame, duration);
    }

    m_animations.push_back(sequence);
    return m_animations.size() -1;
}

int Sprite::GetDefinedAnimations() const
{
    return static_cast<int>(m_animations.size());
}

int Sprite::GetUniqueFrames() const
{
    return static_cast<int>(m_textureCoordinates.size());
}

int Sprite::GetActiveAnimation() const
{
    return m_activeAnimation;
}

const AnimationSequence& Sprite::GetSequence(int id) const
{
    return m_animations[id];
}

AnimationSequence& Sprite::GetSequence(int id)
{
    return m_animations[id];
}

const std::vector<AnimationSequence>& Sprite::GetAnimations() const
{
    return m_animations;
}

std::vector<AnimationSequence>& Sprite::GetAnimations()
{
    return m_animations;
}

int Sprite::FindAnimationByName(const char* name) const
{
    for(size_t index = 0; index < m_animations.size(); ++index)
    {
        const bool found = (std::strcmp(name, m_animations[index].GetName()) == 0);
        if(found)
            return index;
    }

    return -1;
}
