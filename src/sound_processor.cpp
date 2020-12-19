#include <cstdint>
#include <math.h>

#include "commons.h"
#include "sound_processor.h"

double
SoundCallbackProcessor::align(double angle,
                              double period) {
  int32_t qpd = static_cast<int32_t>(angle/period);
  int32_t c = qpd >= 0 ? 1 : -1;
  qpd += c * (qpd & 1);
  angle -= period * qpd;
  return angle;
}
///////////////////////////////////////////////////////

spectrum_set
SoundCallbackProcessor::try_to_find_tone_1(const FFTGuard &fftg0,
                                           const FFTGuard &fftg1,
                                           const PcmFormat &fmt,
                                           int32_t shift_per_frame) {
  double frame_time = static_cast<double>(fftg0.N()) / fmt.sample_rate();
  double shift_time = frame_time / shift_per_frame;
  double bin_2_freq = 1.0 / frame_time;
  spectrum_set max_elements;

  auto it0 = fftg0.spectrum().cbegin();
  auto it1 = fftg1.spectrum().cbegin();
  for (int i = 1; it0 != fftg0.spectrum().cend(); ++it0, ++it1, ++i) {
    double omega_expected = M_2_PI * i * bin_2_freq;
    double omega_actual = (it1->phase - it0->phase) / shift_time;
    double omega_delta = align(omega_actual - omega_expected, M_2_PI);
    double bin_delta = omega_delta / (M_2_PI * bin_2_freq);
    double freq_act = (i + bin_delta) * bin_2_freq;
    double mag = (it0->magnitude + it1->magnitude) * (0.5 + std::abs(bin_delta));

    if (max_elements.size() < 3) {
      max_elements.insert(SpectrumItem(mag, omega_actual, freq_act));
      continue;
    }

    auto min_in_max = std::prev(max_elements.end());
    if (mag <= min_in_max->magnitude)
      continue;

    max_elements.insert(SpectrumItem(mag, omega_actual, freq_act));
    max_elements.erase(std::prev(max_elements.end()));
  }
  return max_elements;
}
///////////////////////////////////////////////////////

SoundCallbackProcessor::SoundCallbackProcessor(size_t shift) :
  m_fftg0(SoundCallbackProcessor::N),
  m_fftg1(SoundCallbackProcessor::N),
  m_ptrFFTG0(&m_fftg0),
  m_ptrFFTG1(&m_fftg1),
  m_wk(),
  m_buff(N),
  m_is_first_filled(false),
  m_is_second_filled(false),
  m_shift(nearest_power_of_2(shift))
{
}
///////////////////////////////////////////////////////

int
SoundCallbackProcessor::data_accumulation(const int16_t *data,
                                          size_t len) {
  while (m_buff.size() < N && len) {
    m_buff.push_back(*data++);
    --len;
  }
  return len;
}
///////////////////////////////////////////////////////

void
SoundCallbackProcessor::handle_new_peace_of_sound(const int16_t *data,
                                                  size_t len) {
  if (data == nullptr || len == 0)
    return; //cause of recursion. if len == 0 -> we need to wait new peace of data

  if (m_buff.size() < N) {
    int remain = data_accumulation(data, len);
    return handle_new_peace_of_sound(&data[len-remain], remain);
  }

  if (!m_is_first_filled) {
    m_ptrFFTG0->fill_fft_data(m_buff, m_wk.windows());
    m_ptrFFTG0->calculate_spectrum(m_fmt.sample_rate());
    m_is_first_filled = true;
    for (size_t i = 0; i < m_shift; ++i)
      m_buff.pop_front();
    return handle_new_peace_of_sound(data, len); // HAHAHA!!! Hell awaits.
  }

  if (!m_is_second_filled) {
    m_ptrFFTG1->fill_fft_data(m_buff, m_wk.windows());
    m_ptrFFTG1->calculate_spectrum(m_fmt.sample_rate());
    m_is_second_filled = true;
  }

  for (; len >= m_shift; len -= m_shift) {
    auto frequencies = try_to_find_tone_1(*m_ptrFFTG0, *m_ptrFFTG1, m_fmt, m_shift);
    for (auto it : frequencies)
      std::cout << it << std::endl;
    std::cout << "*************" << std::endl;

    for (size_t j = 0; j < m_shift; ++j) {
      m_buff.pop_front();
      m_buff.push_back(*data++);
    }

    std::swap(m_ptrFFTG0, m_ptrFFTG1);
    m_fftg0.fill_fft_data(m_buff, m_wk.windows());
    m_fftg0.calculate_spectrum(m_fmt.sample_rate());
  }

  // and here we just set state to not initialized second fftg
  for (size_t i = 0; i < m_shift; ++i)
    m_buff.pop_front();
  m_is_second_filled = false;
  return handle_new_peace_of_sound(data, len); // mother of god %)
}
///////////////////////////////////////////////////////

SoundCallbackProcessor &
SoundCallbackProcessor::instance() {
  static SoundCallbackProcessor inst_(128);
  return inst_;
}
///////////////////////////////////////////////////////
