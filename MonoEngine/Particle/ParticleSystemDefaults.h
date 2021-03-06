
#pragma once

#include "ParticleFwd.h"

namespace mono
{
    void DefaultGenerator(const math::Vector& position, ParticlePool& pool, size_t index);
    void DefaultUpdater(ParticlePool& pool, size_t count, unsigned int delta);
}
