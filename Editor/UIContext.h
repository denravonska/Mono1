
#pragma once

#include "MainMenuOptions.h"
#include "Math/Quad.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    struct Notification
    {
        Notification()
        { }

        Notification(const math::Quad& icon, const std::string& text, int time_left)
            : icon(icon),
              text(text),
              time_left(time_left)
        { }

        math::Quad icon;
        std::string text;
        int time_left;
    };

    struct UIContext
    {
        int tools_texture_id = 0;
        math::Quad default_icon;

        bool showContextMenu = false;
        std::vector<std::string> contextMenuItems;

        const char** texture_items = nullptr;
        int texture_items_count = 0;

        bool has_selection = false;
        float polygon_x = 0.0f;
        float polygon_y = 0.0f;
        float polygon_rotation = 0.0f;
        float texture_repeate = 0.0f;
        int texture_index = 0;

        int active_tool_index = 0;

        std::function<void (float)> texture_repeate_callback;
        std::function<void (int)> texture_changed_callback;
        std::function<void ()> delete_callback;
        std::function<void (int)> contextMenuCallback;
        std::function<void (EditorMenuOptions option)> editorMenuCallback;
        std::function<void (ToolsMenuOptions option)> toolsMenuCallback;

        std::vector<Notification> notifications;
    };
}
