#include <cstdint>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fftw3.h>
#include <iostream>
#include <chrono>
#include <boost/circular_buffer.hpp>

#include "tone_generator.h"
#include "sound_processor.h"

int main(int argc,
         char **argv) {
  (void) argc;
  (void) argv;

  PcmFormat fmt_pcm(16, 8000, 1);
  Tone tone(500, 500, 1.0);

  std::vector<int16_t> pcm_data;
  int32_t data_len = fmt_pcm.audio_len(3000 * 1000);
  pcm_data.resize(data_len);
  tone.generate_tone(fmt_pcm, pcm_data);
  //  //end of input data

  auto st = std::chrono::system_clock::now();
  SoundCallbackProcessor::instance().set_fmt(fmt_pcm);
  for (int i = 0; i < 5; ++i) {
    SoundCallbackProcessor::instance().handle_new_peace_of_sound(pcm_data.data() + 960*i, 960);
  }
  auto end = std::chrono::system_clock::now();
  // callback
  auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - st);
  std::cout << "dur: " << dur.count() << std::endl;
  return 0;
}
///////////////////////////////////////////////////////

