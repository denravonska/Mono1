
#include "ZoneBase.h"
#include "IUpdater.h"
#include "Rendering/IRenderer.h"
#include "Entity/IEntity.h"
#include "Utils.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include <cstdio>

using namespace mono;

namespace
{
    bool DrawableSortFunc(const std::pair<int, IDrawablePtr>& first, const std::pair<int, IDrawablePtr>& second)
    {
        return first.first < second.first;
    }
}

void ZoneBase::Accept(IRenderer& renderer)
{
    for(auto& pair : mDrawables)
        renderer.AddDrawable(pair.second);
}

void ZoneBase::Accept(mono::IUpdater& updater)
{
    DoPreAccept();

    for(auto& updatable : mUpdatables)
        updater.AddUpdatable(updatable);
}

void ZoneBase::DoPreAccept()
{
    for(const auto& task : m_preFrameTasks)
        task();

    m_preFrameTasks.clear();
}

void ZoneBase::AddEntity(const IEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);

    mEntities.push_back(entity);
}

void ZoneBase::RemoveEntity(const IEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);

    const bool result = mono::remove(mEntities, entity);
    if(!result)
        std::printf("ZoneBase - Unable to remove entity with id %u\n", entity->Id());
}

void ZoneBase::AddUpdatable(const IUpdatablePtr& updatable)
{
    mUpdatables.push_back(updatable);
}

void ZoneBase::RemoveUpdatable(const IUpdatablePtr& updatable)
{
    const bool result = mono::remove(mUpdatables, updatable);
    if(!result)
        std::printf("ZoneBase - Unable to remove updatable\n");
}

void ZoneBase::AddDrawable(const IDrawablePtr& drawable, int layer)
{
    mDrawables.push_back(std::make_pair(layer, drawable));

    // Keep the drawable vector sorted so that we draw everything
    // in the correct order according to layers
    std::sort(mDrawables.begin(), mDrawables.end(), DrawableSortFunc);
}

void ZoneBase::RemoveDrawable(const IDrawablePtr& drawable)
{
    const auto func = [drawable](const std::pair<int, IDrawablePtr>& pair) {
        return pair.second == drawable;
    };

    const bool removed = mono::remove_if(mDrawables, func);
    if(!removed)
        std::printf("ZoneBase - Unable to remove drawable\n");
}

void ZoneBase::SetDrawableLayer(const IDrawablePtr& drawable, int new_layer)
{
    const auto func = [drawable](const std::pair<int, IDrawablePtr>& pair) {
        return pair.second == drawable;
    };

    auto it = std::find_if(mDrawables.begin(), mDrawables.end(), func);
    if(it != mDrawables.end())
    {
        it->first = new_layer;
        std::sort(mDrawables.begin(), mDrawables.end(), DrawableSortFunc);        
    }    
}

mono::IEntityPtr ZoneBase::FindEntityFromId(unsigned int id) const
{
    const auto find_func = [id](const mono::IEntityPtr& entity) {
        return id == entity->Id();
    };

    const auto& it = std::find_if(mEntities.begin(), mEntities.end(), find_func);
    if(it != mEntities.end())
        return *it;

    return nullptr;
}

mono::IEntityPtr ZoneBase::FindEntityFromPoint(const math::Vector& point) const
{
    const auto find_func = [&point](const mono::IEntityPtr& entity) {
        const math::Quad& bb = entity->BoundingBox();
        return math::PointInsideQuad(point, bb);
    };

    const auto& it = std::find_if(mEntities.begin(), mEntities.end(), find_func);
    if(it != mEntities.end())
        return *it;

    return nullptr;
}

void ZoneBase::SchedulePreFrameTask(const std::function<void ()>& task)
{
    m_preFrameTasks.push_back(task);
}

