
#pragma once

#include "Particle/ParticleFwd.h"

#include "IUpdatable.h"
#include "Math/Vector.h"

namespace mono
{
    class ParticleEmitter : public mono::IUpdatable
    {
    public:

        struct Configuration
        {
            math::Vector position;
            float duration = 5.0f;      // seconds, negative value means infinite
            float emit_rate = 10.0f;    // particles per second
            bool burst = false;         // if true, one burst only and will emit duration * emit_rate particles
            ParticleGenerator generator = nullptr;
        };

        ParticleEmitter(const Configuration& config, ParticlePool* pool);
        void SetPosition(const math::Vector& position);
        void Stop();
        bool IsDone() const;

        void doUpdate(unsigned int delta) override;

    private:
    
        Configuration m_config;
        bool m_burst_emitted;
        float m_duration;
        float m_carry_over;

        math::Vector m_position;
        ParticlePool* m_pool;
    };
}
