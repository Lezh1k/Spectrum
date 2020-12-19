#ifndef FFT_GUARD_H
#define FFT_GUARD_H

#include <vector>
#include <iostream>
#include <limits>
#include <fftw3.h>

#include <boost/circular_buffer.hpp>

#include "fft_window.h"
#include "commons.h"

typedef boost::circular_buffer_space_optimized<int16_t, std::allocator<int16_t>> data_buffer_t;

struct SpectrumItem {
  double magnitude;
  double phase;
  double freq;
  SpectrumItem() : magnitude(0.0), phase(0.0), freq(0.0) {}
  SpectrumItem(double mag,
               double phase,
               double freq) :
    magnitude(mag),
    phase(phase),
    freq(freq) {}
};

inline bool operator<(const SpectrumItem &l, const SpectrumItem &r) {
  return l.magnitude < r.magnitude;
}

inline bool operator>(const SpectrumItem &l, const SpectrumItem &r) {
  return !(l < r);
}

inline std::ostream& operator<<(std::ostream& os, const SpectrumItem& it) {
  os << "freq:\t" << it.freq << "\tmag:\t" << it.magnitude;
  return os;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

//todo add template for window function generation
class FFTGuard {
private:
  fftw_complex *m_in, *m_out;
  std::vector<SpectrumItem> m_spectrum;
  fftw_plan m_plan_dir;
  int32_t m_N;
  void print_data(bool in) const;

public:
  FFTGuard() = delete;
  explicit FFTGuard(int32_t N);
  ~FFTGuard();

  void print_in() const {return print_data(true);}
  void print_out() const {return print_data(false);}
  const std::vector<SpectrumItem> &spectrum() const {return m_spectrum;}

  int32_t N() const {return m_N;}  

  void fill_fft_data(const data_buffer_t &data,
                     const double *windows);

  void calculate_spectrum(int32_t sample_rate);
};


#endif // FFT_GUARD_H
