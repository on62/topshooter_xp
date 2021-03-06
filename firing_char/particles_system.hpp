#ifndef PARTICLES_SYSTEM_HPP
#define PARTICLES_SYSTEM_HPP

#include <cstdint>

#include "xmath.hpp"
#include "xsdl.hpp"
#include "graphical.hpp"

namespace game {

struct ParticlesBatchSetup {
  xmath::Float2 start_position;
  float center_out_angle;
  float spread_angle;
  float ms_min_vel;
  float ms_max_vel;
  xsdl::Color color;
  Uint32 ms_start;
  Uint32 ms_duration;
  gral::Image *img;
};

class ParticlesSystem {
public:
  ParticlesSystem() noexcept;

  void
  add_batch(const ParticlesBatchSetup& setup) noexcept;

  void
  update_and_render(gral::Screen *screen, uint32_t ms_now);

private:
  enum {
    PARTICLE_BATCHES_MAX = 1 << 8,
    PARTICLES_PER_BATCH = 30,
  };

  struct ParticlesBatch {
    gral::Image *img;
    xmath::Float2 start_position;
    uint32_t ms_duration, ms_start;
    xsdl::Color color;
    struct {
      xmath::Float2 vel;
      float angle;
    } particles[PARTICLES_PER_BATCH];
  };

  ParticlesBatch batches[PARTICLE_BATCHES_MAX];
  int batches_used;
};

}

#endif
