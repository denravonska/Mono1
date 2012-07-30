/*
 *  MOGLRenderer.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IRenderer.h"
#include "TextFunctions.h"

namespace mono
{

    class Renderer : public IRenderer
    {
    public:
        
        Renderer(ICameraPtr camera, IWindowPtr window);
        
        virtual void DrawFrame();
        virtual void Update(unsigned int milliseconds);

        virtual void AddEntity(IEntityPtr entity);
        virtual void AddDrawable(IDrawablePtr drawable);
        virtual void AddUpdatable(IUpdatablePtr updatable);
        
        virtual void DrawText(const std::string& text, const Math::Vector2f& pos, bool center, Color color);

    private:
        
        void PrepareDraw() const;
        void EndDraw() const;

        ICameraPtr mCamera;
        IWindowPtr mWindow;
        
        IDrawableCollection mDrawables;
        IUpdatableCollection mUpdatables;
        TextDefCollection mTexts;
    };

}
