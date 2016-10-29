
#include "Animator.h"
#include "MutableSprite.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"
#include "Events/SurfaceChangedEvent.h"

#include "ICamera.h"
#include "IWindow.h"

#include "Math/Vector2f.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"
#include "System/SysKeycodes.h"

#include "InterfaceDrawer.h"

#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Sprite/AnimationSequence.h"

#include "Texture/ITexture.h"
#include "Texture/TextureFactory.h"
#include "WriteSpriteFile.h"

using namespace animator;

namespace
{
    void SetupIcons(UIContext& context, std::unordered_map<unsigned int, mono::ITexturePtr>& textures)
    {
        mono::ITexturePtr texture = mono::CreateTexture("textures/animator_tools.png"); 
        textures.insert(std::make_pair(texture->Id(), texture));

        const mono::ISpritePtr save = mono::CreateSprite("sprites/save.sprite");

        context.tools_texture_id = texture->Id();
        context.save_icon = save->GetTextureCoords();
    }
}

Animator::Animator(const mono::IWindowPtr& window, mono::EventHandler& eventHandler, const char* sprite_file)
    : m_eventHandler(eventHandler),
      m_spriteFile(sprite_file),
      m_inputHandler(eventHandler)
{
    using namespace std::placeholders;
    
    const event::KeyDownEventFunc& func = std::bind(&Animator::OnDownUp, this, _1);
    const event::SurfaceChangedEventFunc& surface_func = std::bind(&Animator::OnSurfaceChanged, this, _1);
    const event::MouseWheelEventFunc& mouse_wheel = std::bind(&Animator::OnMouseWheel, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&Animator::OnMultiGesture, this, _1);

    m_keyDownToken = eventHandler.AddListener(func);
    m_surfaceChangedToken = eventHandler.AddListener(surface_func);
    m_mouseWheelToken = m_eventHandler.AddListener(mouse_wheel);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);

    std::unordered_map<unsigned int, mono::ITexturePtr> textures;
    SetupIcons(m_context, textures);

    m_guiRenderer = std::make_shared<ImGuiRenderer>("animator_imgui.ini", window->Size(), textures);
    mono::CreateSprite(m_sprite, sprite_file);
    
    AddEntity(std::make_shared<MutableSprite>(m_sprite), 0);
    AddDrawable(m_guiRenderer, 2);
    AddUpdatable(std::make_shared<InterfaceDrawer>(m_context));

    // Setup UI callbacks
    m_context.on_loop_toggle      = std::bind(&Animator::OnLoopToggle, this, _1);
    m_context.on_add_animation    = std::bind(&Animator::OnAddAnimation, this);
    m_context.on_delete_animation = std::bind(&Animator::OnDeleteAnimation, this, _1);
    m_context.on_add_frame        = std::bind(&Animator::OnAddFrame, this);
    m_context.on_delete_frame     = std::bind(&Animator::OnDeleteFrame, this, _1);
    m_context.on_save             = std::bind(&Animator::SaveSprite, this);

    m_context.max_frame_id = m_sprite.GetUniqueFrames() -1;
    m_context.active_frame = m_sprite.GetActiveAnimation();

    SetAnimation(m_sprite.GetActiveAnimation());
}

Animator::~Animator()
{
    m_eventHandler.RemoveListener(m_keyDownToken);
    m_eventHandler.RemoveListener(m_surfaceChangedToken);
    m_eventHandler.RemoveListener(m_mouseWheelToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
}

void Animator::OnLoad(mono::ICameraPtr camera)
{
    m_camera = camera;
    camera->SetPosition(math::zeroVec);
}

void Animator::OnUnload()
{
    m_camera = nullptr;
    SaveSprite();
}

void Animator::SetAnimation(int animation_id)
{
    const int animations = m_sprite.GetDefinedAnimations();
    if(animation_id < animations)
    {
        m_sprite.SetAnimation(animation_id);
        UpdateUIContext(animation_id);
    }
}

void Animator::UpdateUIContext(int animation_id)
{
    mono::AnimationSequence& sequence = m_sprite.GetSequence(animation_id);

    m_context.animation_id = animation_id;
    m_context.display_name = "hello";
    m_context.loop_animation = sequence.IsLooping();
    m_context.frames = &sequence.GetFrames();
}

bool Animator::OnDownUp(const event::KeyDownEvent& event)
{
    int animation = -1;
    
    if(event.key == Key::ENTER || event.key == Key::SPACE)
    {
        m_sprite.RestartAnimation();
        return true;
    }
    else if(event.key == Key::LEFT || event.key == Key::DOWN)
    {
        int id = m_sprite.GetActiveAnimation();
        --id;

        animation = std::max(id, 0);
    }
    else if(event.key == Key::RIGHT || event.key == Key::UP)
    {
        int id = m_sprite.GetActiveAnimation();
        ++id;

        animation = std::min(id, m_sprite.GetDefinedAnimations() -1);
    }
    else
    {
        if(event.key == Key::ZERO)
            animation = 0;
        else if(event.key == Key::ONE)
            animation = 1;
        else if(event.key == Key::TWO)
            animation = 2;
        else if(event.key == Key::THREE)
            animation = 3;
        else if(event.key == Key::FOUR)
            animation = 4;
        else if(event.key == Key::FIVE)
            animation = 5;
        else if(event.key == Key::SIX)
            animation = 6;
        else if(event.key == Key::SEVEN)
            animation = 7;
        else if(event.key == Key::EIGHT)
            animation = 8;
        else if(event.key == Key::NINE)
            animation = 9;
    }

    if(animation != -1)
    {
        SetAnimation(animation);
        return true;
    }

    return false;
}

bool Animator::OnMouseWheel(const event::MouseWheelEvent& event)
{
    const float multiplier = (event.y < 0.0f) ? 1.0f : -1.0f;
    Zoom(multiplier);
    return true;
}

bool Animator::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(std::fabs(event.distance) < 1e-3)
        return false;

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    Zoom(multiplier);
    return true;
}

bool Animator::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    if(event.width > 0 && event.height > 0)
        m_guiRenderer->SetWindowSize(math::Vector2f(event.width, event.height));

    return false;
}

void Animator::OnLoopToggle(bool state)
{
    const int current_id = m_sprite.GetActiveAnimation();
    mono::AnimationSequence& sequence = m_sprite.GetSequence(current_id);
    sequence.SetLooping(state);

    m_sprite.RestartAnimation();
}

void Animator::OnAddAnimation()
{ }

void Animator::OnDeleteAnimation(int id)
{ }

void Animator::OnAddFrame()
{
    const int current_id = m_sprite.GetActiveAnimation();
    m_sprite.GetSequence(current_id).AddFrame(0, 100);
}

void Animator::OnDeleteFrame(int id)
{
    const int current_id = m_sprite.GetActiveAnimation();
    m_sprite.GetSequence(current_id).RemoveFrame(id);
}

void Animator::Zoom(float multiplier)
{
    math::Quad quad = m_camera->GetViewport();

    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    m_camera->SetTargetViewport(quad);
}

void Animator::SaveSprite()
{
    WriteSpriteFile(m_spriteFile, m_sprite.GetAnimations());
}

