#include "fft_guard.h"

#include <iostream>
#include <math.h>
#include <assert.h>

void FFTGuard::print_data(bool in) const {
  const fftw_complex *p_data = in ? m_in : m_out;
  for (int i = 0; i < m_N; ++i)
    std::cout << p_data[i][0] << " " << p_data[i][1] << std::endl;
}

FFTGuard::FFTGuard(int32_t N) :
  m_in(nullptr),
  m_out(nullptr),
  m_spectrum(N/2),
  m_N(N)
{
  m_in = fftw_alloc_complex(N);
  m_out = fftw_alloc_complex(N);
  m_plan_dir = fftw_plan_dft_1d(N, m_in, m_out, FFTW_FORWARD, FFTW_PATIENT);
}
///////////////////////////////////////////////////////

FFTGuard::~FFTGuard() {
  fftw_destroy_plan(m_plan_dir);
}
///////////////////////////////////////////////////////

void
FFTGuard::fill_fft_data(const data_buffer_t &data,
                        const double *windows) {
  assert(windows != nullptr);
  assert(data.size() == static_cast<size_t>(m_N));
  // todo use exceptions instead of assert. or even use some error codes here.
  fftw_complex *dst = m_in;
  for (auto it = data.begin(); it != data.end(); ++it, ++windows, ++dst) {
    (*dst)[0] = *windows * (static_cast<double>(*it) / 0x8000u); //todo delete division by 0x8000u
    (*dst)[1] = 0.0;
  }
}
///////////////////////////////////////////////////////

void
FFTGuard::calculate_spectrum(int32_t sample_rate) {
  fftw_execute(m_plan_dir);
  double fs = static_cast<double>(sample_rate) / m_N;
  double freq = 0.0;
  for (int i = 0; i < m_N / 2; ++i, freq += fs) {
    double real, imag;
    real = m_out[i][0];
    imag = m_out[i][1];
    m_spectrum[i] = SpectrumItem(std::hypot(real, imag),
                                 std::atan2(real, imag),
                                 freq);
  }
}
///////////////////////////////////////////////////////

