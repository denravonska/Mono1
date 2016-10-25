
#pragma once

#include "Sprite/AnimationSequence.h"
#include "Math/Quad.h"
#include <vector>
#include <functional>

namespace animator
{
    enum class Action
    {
        ZOOM_IN,
        ZOOM_OUT,
        SAVE
    };

    struct UIContext
    {
        int tools_texture_id;
        math::Quad zoom_in_icon;
        math::Quad zoom_out_icon;
        math::Quad save_icon;

        int animation_id;
        const char* display_name;
        bool loop_animation;
        std::vector<mono::Frame>* frames;

        std::function<void (bool)> on_loop_toggle;
        
        std::function<void ()>    on_add_animation;
        std::function<void (int)> on_delete_animation;
        std::function<void ()>    on_add_frame;
        std::function<void (int)> on_delete_frame;

        std::function<void (Action)> on_tool_callback;
    };
}
