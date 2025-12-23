#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "analyzer.h"

class Processor {
public:
    Processor();
    ~Processor();
    void process(int16_t* samples, const int& sampleCount, const int& sampleRate);
private:    
    float prevAmplitude;
    kiss_fftr_cfg cfg;
    std::vector<float> float_buffer;
    std::vector<kiss_fft_cpx> spectrum_buffer;
};

#endif // PROCESSOR_H