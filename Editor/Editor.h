//
//  Editor.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#pragma once

#include "ZoneBase.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

#include "ImGuiInterfaceDrawer.h"
#include "ImGuiInputHandler.h"

#include "Polygon.h"


namespace editor
{
    class ImGuiRenderer;
    class UserInputController;

    class EditorZone : public mono::ZoneBase
    {
    public:

        EditorZone(const mono::IWindowPtr& window, mono::EventHandler& event_handler, const char* file_name);
        virtual ~EditorZone();

        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();

        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);

        void AddPolygon(const std::shared_ptr<editor::PolygonEntity>& polygon);

        mono::IEntityPtr FindEntityFromPosition(const math::Vector2f& position);
        void SelectEntity(const mono::IEntityPtr& entity);

        void OnContextMenu(int index);
        void OnTextureRepeate(float repeate);
        void OnTextureChanged(int texture_index);
        void OnDeletePolygon();

        void EditorMenuCallback(EditorMenuOptions index);
        void ToolsMenuCallback(ToolsMenuOptions index);

        mono::IWindowPtr m_window;
        mono::EventHandler& m_eventHandler;
        editor::ImGuiInputHandler m_inputHandler;
        const char* m_fileName;
        mono::ICameraPtr m_camera;

        editor::UIContext m_context;
        std::shared_ptr<editor::ImGuiInterfaceDrawer> m_interfaceDrawer;
        std::shared_ptr<editor::ImGuiRenderer> m_guiRenderer;
        std::shared_ptr<editor::UserInputController> m_userInputController;

        std::shared_ptr<editor::PolygonEntity> m_selected_polygon;
        std::vector<std::shared_ptr<editor::PolygonEntity>> m_polygons;

        mono::EventToken<event::SurfaceChangedEvent> m_surfaceChangedToken;
    };
}
