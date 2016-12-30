//
//  CubeSwarm.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 24/03/15.
//
//

#include "CubeSwarm.h"
#include "IRenderer.h"
#include "Color.h"
#include "Utils.h"

#include <functional>

using namespace game;


CubeSwarm::CubeSwarm()
{
    mCubes.resize(100);

    auto func = std::bind(&CubeSwarm::GenerateCube, this);
    std::generate(mCubes.begin(), mCubes.end(), func);
}

void CubeSwarm::Draw(mono::IRenderer& renderer) const
{
    for(const Cube& cube : mCubes)
        renderer.DrawQuad(cube.quad, mono::Color::ToRGBA(cube.color), 2.0f);
}

void CubeSwarm::Update(unsigned int delta)
{
    // Reset the bounding box values, will get updated inside the for-loop
    m_bounds.mA.x = -99e99;
    m_bounds.mA.y = 99e99;
    m_bounds.mB.x = -99e99;
    m_bounds.mB.y = 99e99;

    for(Cube& cube : mCubes)
    {
        const float add_x = mono::Random(-1.0f, 1.0f);
        const float add_y = mono::Random(-1.0f, 1.0f);
        const math::Vector add(add_x, add_y);

        cube.quad.mA += add;
        cube.quad.mB += add;

        m_bounds |= cube.quad;

        cube.color.hue += delta * 0.0005f;
        if(cube.color.hue > 1.0f)
            cube.color.hue = 0.0f;
    }
}

math::Quad CubeSwarm::BoundingBox() const
{
    return m_bounds;
}

CubeSwarm::Cube CubeSwarm::GenerateCube()
{
    const float random_x = mono::Random(-50.0f, 50.0f);
    const float random_y = mono::Random(-50.0f, 50.0f);
    const float random_size = mono::Random(0.0f, 10.0f);

    const mono::Color::HSL color(mono::Random(), 0.7f, 0.5f);

    return CubeSwarm::Cube(math::Vector(random_x, random_y), random_size, color);
}
