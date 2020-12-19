#ifndef SOUND_PROCESSOR_H
#define SOUND_PROCESSOR_H

#include <set>
#include <vector>
#include <boost/circular_buffer.hpp>

#include "fft_guard.h"
#include "fft_window.h"
#include "pcm_format.h"

typedef std::set<SpectrumItem, std::greater<SpectrumItem> > spectrum_set;


class SoundCallbackProcessor {
public:
  static constexpr int N = 4096;

private:
  double align(double angle,
               double period);
  spectrum_set try_to_find_tone_1(const FFTGuard& fftg0,
                                  const FFTGuard& fftg1,
                                  const PcmFormat& fmt,
                                  int32_t shift_per_frame);
  int data_accumulation(const int16_t *data,
                        size_t len);

  explicit SoundCallbackProcessor(size_t shift);
  ///////////////////////////////////////////////////////

  FFTGuard m_fftg0, m_fftg1;
  FFTGuard *m_ptrFFTG0, *m_ptrFFTG1;
  FFTWindowKeeper<N, WF_Hann> m_wk;
  data_buffer_t m_buff;
  bool m_is_first_filled;
  bool m_is_second_filled;

  size_t m_shift;
  PcmFormat m_fmt;

  //copy and assignment are prohibited
  SoundCallbackProcessor() = delete;
  SoundCallbackProcessor(const SoundCallbackProcessor&) = delete;
  SoundCallbackProcessor& operator=(const SoundCallbackProcessor) = delete;


public:
  void set_fmt(const PcmFormat &fmt) {m_fmt = fmt;}
  const PcmFormat &fmt() const {return m_fmt;} //I don't think we need this
  static SoundCallbackProcessor &instance();
  void handle_new_peace_of_sound(const int16_t *data,
                                 size_t len);
};

#endif // SOUND_PROCESSOR_H
