#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "analyzer.h"
#include "comparator.h"



using namespace std;
class Processor {
public:
    Processor();
    ~Processor();
    void process(int16_t* samples, const int& sampleCount, const int& sampleRate);
private:    
    float prevAmp;
    kiss_fftr_cfg cfg;

    vector<float> floatBuffer;
    vector<kiss_fft_cpx> spectrumBuffer;
    vector<vector<float>> activeChunk;
    vector<int> memoryHash;

    Comparator comparator;
};

#endif // PROCESSOR_H