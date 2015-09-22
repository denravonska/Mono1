/*
 *  MonolithFwd.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

namespace mono
{
    struct IWindow;
    struct ICamera;
    struct IRenderer;
    struct IDrawable;
    struct IEntity;
    struct IPhysicsEntity;
    struct IUpdatable;
    struct IInputHandler;
    struct IZone;
    struct ITexture;
    struct IAction;
    struct IPath;
    
    class EventHandler;
    class Sprite;

    namespace Color
    {
        struct RGBA;
        struct HSL;
    }
}
