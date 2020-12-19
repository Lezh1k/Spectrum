#ifndef PCM_FORMAT_H
#define PCM_FORMAT_H

#include <cstdint>

class PcmFormat {
private:
  int32_t m_sample_size;
  int32_t m_sample_rate;
  int32_t m_channel_count;

public:
  PcmFormat() = default;
  PcmFormat(int32_t sample_size,
            int32_t sample_rate,
            int32_t channel_count):
    m_sample_size(sample_size),
    m_sample_rate(sample_rate),
    m_channel_count(channel_count) {}

  int32_t sample_size() const {return m_sample_size;}
  int32_t sample_rate() const {return m_sample_rate;}
  int32_t channel_count() const {return m_channel_count;}

  int64_t audio_len(uint64_t duration_us) const {
    int64_t result = (m_sample_rate * m_channel_count * (m_sample_size / 8))
                     * duration_us / 1000000;
    result -= result % (m_channel_count * m_sample_size);
    return result;
  }
};
///////////////////////////////////////////////////////

#endif // PCM_FORMAT_H
