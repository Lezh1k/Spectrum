#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

#include <vector>
#include "pcm_format.h"

class Tone {
private:
  int m_start_freq;
  int m_end_freq;
  double m_amplitude;

public:
  Tone() = delete;
  Tone(int start_freq,
       int end_freq,
       double amplitude) :
    m_start_freq(start_freq),
    m_end_freq(end_freq),
    m_amplitude(amplitude) {}

  int start_freq() const {return m_start_freq;}
  int end_freq() const {return m_end_freq;}
  double amplitude() const {return m_amplitude;}

  void generate_tone(const PcmFormat &format,
                     std::vector<int16_t> &buffer) const;
};
///////////////////////////////////////////////////////

#endif // TONE_GENERATOR_H
