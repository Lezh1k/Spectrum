#ifndef FFT_WINDOW_H
#define FFT_WINDOW_H

#include <array>

enum WindowFunctions {
  WF_Gaussian,
  WF_Rectangle,
  WF_Sinus,
  WF_Sinc,
  WF_Treangle,
  WF_Hann,
  WF_Hamming,
  WF_Blackman,
  WF_Nutall,
};

typedef double (*lp_window_func)(double n, double N);
class FFTWindow {
private:
  static double aux_sinc(double x);
  static double aux_gauss(double n, double N, double sigm);

  static double fn_gauss(double n, double N);
  static double fn_rect(double n, double N);
  static double fn_sin(double n, double N);
  static double fn_sinc(double n, double N);
  static double fn_treangle(double n, double N);
  static double fn_hann(double n, double N);
  static double fn_hamming(double n, double N);
  static double fn_blackmann(double n, double N);
  static double fn_nutall(double n, double N);

public:
  //n - номер отсчета. N - количество отсчетов.
  static lp_window_func fn_window(WindowFunctions wf) {
    static const lp_window_func funcs[] = {
      fn_gauss, fn_rect, fn_sin,
      fn_sinc, fn_treangle, fn_hann,
      fn_hamming, fn_blackmann, fn_nutall
    };
    return funcs[wf];
  }
};
///////////////////////////////////////////////////////

template<int N, WindowFunctions wf = WF_Rectangle>
class FFTWindowKeeper {
private:
  std::array<double, N> val_ = [] {
    std::array<double, N> res = {};
    auto fn = FFTWindow::fn_window(wf);
    for (int i = 0; i < N; ++i)
      res[i] = fn(i, N);
    return res;
  }();
public:
  const double *windows() const {return val_.data();}
};
///////////////////////////////////////////////////////

#endif // FFT_WINDOW_H
