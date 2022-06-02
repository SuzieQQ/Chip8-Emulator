//https://github.com/qxxxb/sdl2-beeper
#include <iostream>
#include <string>
#include <math.h>

#include "Audio.hpp"

SDL_AudioDeviceID Audio::m_audioDevice;
SDL_AudioSpec Audio::m_obtainedSpec;
double Audio::m_frequency = 392.00;
int Audio::m_pos;

void (*Audio::m_writeData)(BYTE *ptr, double data);

int (*Audio::m_calculateOffset)(int sample, int channel);

// Calculate the offset in bytes from the start of the audio stream to the
// memory address at `sample` and `channel`.
// Channels are interleaved.
int calculateOffset_s16(int sample, int channel) {
    return (sample * sizeof(SWORD) * Audio::m_obtainedSpec.channels) + (channel * sizeof(SWORD));
}

// Convert a normalized data value (range: 0.0 .. 1.0) to a data value matching
// the audio format.
void writeData_s16(BYTE *ptr, double data) {
    auto *ptrTyped = (SWORD *) ptr;
    double range = (double) INT16_MAX - (double) INT16_MIN;
    double dataScaled = data * range / 2.0;
    *ptrTyped = dataScaled;
}

// Generate audio data. This is how the waveform is generated.
double Audio::getData() {
    auto sampleRate = (double) (m_obtainedSpec.freq);

    // Units: samples
    double period = sampleRate / m_frequency;

    // Reset m_pos when it reaches the start of a period so it doesn't run off
    // to infinity (though this won't happen unless you are playing sound for a
    // very long time)
    if (m_pos % (int) period == 0) {
        m_pos = 0;
    }

    double pos = m_pos;
    double angular_freq = (1.0 / period) * 2.0 * M_PI;

    return sin(pos * angular_freq);
}

void Audio::audioCallback(void *userdata, BYTE *stream, int len) {
    // Unused parameters
    (void) userdata;
    (void) len;

    // Write data to the entire buffer by iterating through all samples and
    // channels.
    for (int sample = 0; sample < m_obtainedSpec.samples; ++sample) {
        double data = getData();
        m_pos++;

        // Write the same data to all channels
        for (int channel = 0; channel < m_obtainedSpec.channels; ++channel) {
            int offset = m_calculateOffset(sample, channel);
            BYTE *ptrData = stream + offset;
            m_writeData(ptrData, data);
        }
    }
}

void Audio::open() {
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);

    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16;
    desiredSpec.samples = 512;
    desiredSpec.channels = 1;
    desiredSpec.callback = Audio::audioCallback;

    m_audioDevice = SDL_OpenAudioDevice(
            nullptr, // device (name of the device, which we don't care about)
            0, // iscapture (we are not recording sound)
            &desiredSpec, // desired
            &m_obtainedSpec, // obtained
            0 // allowed_changes (allow any changes between desired and obtained)
    );

    if (m_audioDevice == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        // TODO: throw exception
    }
    m_writeData = writeData_s16;
    m_calculateOffset = calculateOffset_s16;
}

void Audio::Beep() {
    //TODO: Separate thread to not lag the game
    SDL_PauseAudioDevice(m_audioDevice, 0);
    //std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Sleep 16 ms
    SDL_Delay(8);
    SDL_PauseAudioDevice(m_audioDevice, 1);
}


void Audio::close() {
    SDL_CloseAudioDevice(m_audioDevice);
}
