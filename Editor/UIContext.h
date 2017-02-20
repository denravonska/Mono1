
#pragma once

#include "MainMenuOptions.h"
#include "Math/Quad.h"
#include "UINotification.h"

#include <functional>
#include <vector>
#include <string>

namespace editor
{
    struct UIEntityItem
    {
        int texture_id = 0;
        math::Quad icon;
        std::string tooltip;
    };

    struct UIContext
    {
        int tools_texture_id = 0;
        math::Quad default_icon;

        bool showContextMenu = false;
        std::vector<std::string> contextMenuItems;

        const char** texture_items = nullptr;
        int texture_items_count = 0;

        bool has_polygon_selection = false;
        bool has_path_selection = false;

        float position_x = 0.0f;
        float position_y = 0.0f;
        float rotation = 0.0f;

        float texture_repeate = 0.0f;
        int texture_index = 0;
        const char* path_name = nullptr;

        int active_tool_index = 0;

        std::string drag_name;

        std::vector<UIEntityItem> entity_items;
        std::vector<Notification> notifications;

        std::function<void (float)> texture_repeate_callback;
        std::function<void (int)> texture_changed_callback;
        std::function<void (const char*)> path_name_callback;
        std::function<void ()> delete_callback;
        std::function<void (int)> context_menu_callback;
        std::function<void (EditorMenuOptions option)> editor_menu_callback;
        std::function<void (ToolsMenuOptions option)> tools_menu_callback;
        std::function<void (const std::string& id, const math::Vector& position)> drop_callback;
    };
}
