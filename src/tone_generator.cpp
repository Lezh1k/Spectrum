#include "tone_generator.h"
#include <math.h>

void
Tone::generate_tone(const PcmFormat &format,
                    std::vector<int16_t> &buffer) const {
  const int channel_bytes = format.sample_size() / 8;
  const int sample_bytes = format.channel_count() * channel_bytes;
  const int num_samples = buffer.size() / sample_bytes ;
  auto ptr = buffer.begin();

  double phase = 0.0;
  const double d = 2 * M_PI / format.sample_rate();

  // We can't generate a zero-frequency sine wave
  const double start_freq = m_start_freq ? m_start_freq : 1.0;

  // Amount by which phase increases on each sample
  double phase_step = d * start_freq;

  // Amount by which phase_step increases on each sample
  // If this is non-zero, the output is a frequency-swept tone.
  const double phase_step_2 = d * (m_end_freq - start_freq) / num_samples;

  while (ptr != buffer.end()) {
    const double x = m_amplitude * std::sin(phase);
    const int16_t pcm_val = x * 0x7fff;

    for (int i = 0; i < format.channel_count() && ptr != buffer.end(); ++i) {
      // todo check, that pcm_val is in little endian
      // if not - then convert to little endian.
      *ptr++ = pcm_val;
    }

    phase += phase_step;
    while (phase > 2 * M_PI)
      phase -= 2 * M_PI;
    phase_step += phase_step_2;
  }
}

