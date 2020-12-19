#include "fft_window.h"
#include <math.h>
#include "commons.h"

double FFTWindow::aux_sinc(double x) {
  return sin(M_PI * x) / M_PI * x;
}

double FFTWindow::aux_gauss(double n, double N, double sigm) {
  double A = (N - 1) / 2;
  double arg = pow(((n - A) / (sigm * A)), 2);
  return exp(-0.5 * arg);
}
///////////////////////////////////////////////////////

double FFTWindow::fn_rect(double n, double N) {
  UNUSED(n);
  UNUSED(N);
  return 1.0;
}
///////////////////////////////////////////////////////

double FFTWindow::fn_sin(double n, double N) {
  return sin((M_PI*n) / (N-1));
}
///////////////////////////////////////////////////////

double FFTWindow::fn_sinc(double n, double N) {
  return aux_sinc((2*n)/(N-1) - 1);
}
///////////////////////////////////////////////////////

double FFTWindow::fn_treangle(double n, double N) {
  return 1 - std::abs((2*n)/(N-1) - 1);
}
///////////////////////////////////////////////////////

double FFTWindow::fn_hann(double n, double N) {
  return 0.5 - 0.5 * cos((2*M_PI*n) / (N-1));
}
///////////////////////////////////////////////////////

double FFTWindow::fn_hamming(double n, double N) {
  return 0.54 - 0.46 * cos((2*M_PI*n) / (N-1));
}
///////////////////////////////////////////////////////

double FFTWindow::fn_blackmann(double n, double N) {
  return 0.42 - 0.5 * cos((2*M_PI*n) / (N-1)) + 0.08 * cos((4*M_PI*n) / (N-1));
}
///////////////////////////////////////////////////////

double FFTWindow::fn_nutall(double n, double N) {
  return 0.355768 -
      0.487396 * cos((2*M_PI*n) / (N-1)) +
      0.1365995 * cos((4*M_PI*n) / (N-1)) -
      0.0106411 * cos((6*M_PI*n) / (N-1)) ;
}
///////////////////////////////////////////////////////

double FFTWindow::fn_gauss(double n, double N) {
  return aux_gauss(n, N, 0.3);
}
///////////////////////////////////////////////////////

