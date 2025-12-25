#define MINIAUDIO_IMPLEMENTATION
#include "../libs/miniaudio.h"

#include "analyzer.h"
#include "processor.h"

#include <string>
#include <sstream>

//build with
//mingw32-make
//YOU NEED TO BE IN BUILD FOLDER cd build
//because cmake is being a little bitch ass piece of shit rn
Processor p = Processor();

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    int16_t* samples = (int16_t*)pInput; 
    int sampleCount = frameCount * pDevice->capture.channels;
    p.process(samples,sampleCount,pDevice->sampleRate);
}
int get_input_index() {   
    std::cout << "Select a device" << std::endl;
    std::string input;
    std::cin >> input;
    std::stringstream ss(input);

    int index;
    ss >> index;
    if (ss.fail()) {
        std::cout << "Index is not an integer" << std::endl;
        return get_input_index();
    }
    return index;
}
int get_capture_mode() {
    std::cout << "Select how to capture audio" << std::endl;
    std::string input;
    std::cin >> input;
    std::stringstream ss(input);

    int index;
    ss >> index;
    if (ss.fail()) {
        std::cout << "Index is not an integer" << std::endl;
        return get_capture_mode();
    }
    return index;
}
int main() {
    ma_context context;

    ma_result result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS) {
        return -1;
    }

    ma_device_info* playbackInfos;
    ma_uint32 playbackCount;
    ma_device_info* captureInfos;
    ma_uint32 captureCount;
    
    if (ma_context_get_devices(&context, &playbackInfos, &playbackCount, &captureInfos, &captureCount) != MA_SUCCESS) {
        return -1;
    }

    //mode selection
    std::cout << "0 for capture, 1 for playback" << std::endl;

    ma_device_config config;
    int captureMode = get_capture_mode();

    //device selection
    if (captureMode == 0) {
        for (ma_uint32 iDevice = 0; iDevice < captureCount; iDevice += 1)
            printf("%d - %s\n", iDevice, captureInfos[iDevice].name);

        int deviceIndex = get_input_index();
        
        config = ma_device_config_init(ma_device_type_capture);
        config.capture.pDeviceID   = &captureInfos[deviceIndex].id;
        config.capture.format      = ma_format_s16;
        config.capture.channels    = 0; //default
        config.sampleRate          = 0; //default
        config.dataCallback        = data_callback;
    } else {
        for (ma_uint32 iDevice = 0; iDevice < playbackCount; iDevice += 1)
            printf("%d - %s\n", iDevice, playbackInfos[iDevice].name);
        
        int deviceIndex = get_input_index();
        
        config = ma_device_config_init(ma_device_type_loopback);
        config.capture.pDeviceID    = &playbackInfos[deviceIndex].id;
        config.capture.format       = ma_format_s16;
        config.capture.channels     = 0;
        config.sampleRate           = 0;
        config.dataCallback         = data_callback;
    }
    
    //device setup
    ma_device device;
    if (ma_device_init(&context,&config,&device) != MA_SUCCESS) {
        return -1;
    }
    //start
    if (ma_device_start(&device) != MA_SUCCESS) {
        ma_device_uninit(&device);
        ma_context_uninit(&context);
        return -1;
    }

    
    //exit upon entering anything
    std::string temp;
    std::cin >> temp;
    ma_context_uninit(&context);
    return 0;
}