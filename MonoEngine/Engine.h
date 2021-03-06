/*
 *  MonolithEngine.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoPtrFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"
#include "Rendering/RenderPtrFwd.h"

namespace System
{
    class IWindow;
    class IInputHandler;
};

namespace mono
{
    class Engine
    {
    public:
        Engine(System::IWindow* window, const ICameraPtr& camera, EventHandler& event_handler);
        ~Engine();

        int Run(IZonePtr zone);

    private:

        bool OnPause(const event::PauseEvent& event);
        bool OnQuit(const event::QuitEvent& event);
        bool OnApplication(const event::ApplicationEvent& event);
        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        bool OnActivated(const event::ActivatedEvent& event);
        bool OnTimeScale(const event::TimeScaleEvent& event);

        bool m_pause = false;
        bool m_quit = false;
        bool m_update_last_time = false;
        float m_time_scale = 1.0f;

        System::IWindow* m_window;
        ICameraPtr m_camera;
        EventHandler& m_event_handler;
        std::unique_ptr<System::IInputHandler> m_input_handler;

        EventToken<event::PauseEvent> mPauseToken;
        EventToken<event::QuitEvent> mQuitToken;
        EventToken<event::ApplicationEvent> mApplicationToken;
        EventToken<event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<event::ActivatedEvent> mActivatedToken;
        EventToken<event::TimeScaleEvent> mTimeScaleToken;
    };
}

