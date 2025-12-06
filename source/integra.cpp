//
// integra.cpp - Integra & Pulverize LV2 Plugins
// Copyright (C) 2025 theKAOSSphere
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include <cstdlib>
#include <cmath>
#include "biquad.h"

// Dynamic URI based on build target
#ifdef PULVERIZE_MODE
#define PLUGIN_URI "https://github.com/theKAOSSphere/integra#pulverize"
#else
#define PLUGIN_URI "https://github.com/theKAOSSphere/integra"
#endif

// Constants
const double ANALOG_LPF_FREQ = 18000.0f;
const float MAX_GAIN = 17.78279f; // 25 dB in linear scale

enum portIndex {
    PORT_INPUT  = 0, 
    PORT_OUTPUT = 1, 
    #ifdef PULVERIZE_MODE
    // Pulverize only has one knob (Volume)
    PORT_VOLUME  = 2
    #else
    // Integra has 3 knobs
    PORT_TREBLE = 2,
    PORT_BASS   = 3, 
    PORT_VOLUME  = 4
    #endif
};

// This struct contains the stuff needed for the plugin instance
typedef struct integra
{
    // Port buffers
    const float* input;
    float*       output;

    // Control port pointers
    #ifndef PULVERIZE_MODE
    const float* treble;
    const float* bass;
    #endif
    const float* volume;

    // DSP state
    Biquad low_shelf;
    Biquad high_shelf;
    Biquad analog_lpf;
    #ifdef PULVERIZE_MODE
    Biquad peak; // Additional peaking filter for Pulverize
    #endif

    double sampleRate;

    // Cached control values to avoid unnecessary recalculations
    float last_treble;
    float last_bass;
} Integra;

// This function creates an instance of the LV2 plugin
static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               bundle_path,
                              const LV2_Feature* const* features)
{
    Integra* self = new Integra();
    if (!self) return NULL;

    self->sampleRate = rate;
    self->analog_lpf.calculateCoefficients(LowPass, rate, ANALOG_LPF_FREQ, 0.707, 0.0);
    
    #ifdef PULVERIZE_MODE
    // More aggressive cut than Integra
    self->low_shelf.calculateCoefficients(LowShelf1, rate, 650.0, 0, -22.0); 
    // Treble shelf moved closer to accentuate the mid hump
    self->high_shelf.calculateCoefficients(HighShelf1, rate, 1800.0, 0.0, -4); // -3.5 map
    // Mild peaking filter for midrange hump
    self->peak.calculateCoefficients(Peaking, rate, 1000.0, 0.707, 1.0);
    #else
    // Integra initialization: Initialize cached values to NAN to force a calculation on the first run
    self->last_treble = NAN;
    self->last_bass = NAN;
    #endif

    return (LV2_Handle)self;
}

// Below function is used to connect the respective ports
static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    Integra* self = (Integra*)instance;
    switch (port)
    {
        case PORT_INPUT:    self->input  = (const float*)data; break;
        case PORT_OUTPUT:   self->output = (float*)data;       break;
        #ifdef PULVERIZE_MODE
        case PORT_VOLUME:   self->volume = (const float*)data; break;
        #else
        case PORT_TREBLE:   self->treble = (const float*)data; break;
        case PORT_BASS:     self->bass   = (const float*)data; break;
        case PORT_VOLUME:   self->volume = (const float*)data; break;
        #endif
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
    Integra* self = (Integra*)instance;

    // Map Volume 0..10 to -90..+25 dB
    // Cubic taper to replicate log pot behavior
    float vol_norm = *self->volume * 0.1f;
    float vol_taper = vol_norm * vol_norm * vol_norm;
    const float volume_lin = vol_taper * MAX_GAIN;

    #ifndef PULVERIZE_MODE
    // --- INTEGRA SLIDING LOGIC  ---
    
    if (*self->treble != self->last_treble) 
    {
        float treble_db = *self->treble * 1.5f; // -10..+10 -> -15..+15
        // Knob Range: -10 to +10
        // Normalized Turn: 0.0 to 1.0 (Magnitude of turn)
        double turn = std::abs(*self->treble) / 10.0;

        // Target:
        // 0.0 (Center) -> 600 Hz 
        // 0.2 (Meshuggah) -> ~2200 Hz 
        // 1.0 (Max) -> 8600 Hz 
        // Linear Interpolation: 600 + (Turn * 8000)
        double dynFreq = 600.0 + (turn * 8000.0); 

        self->high_shelf.calculateCoefficients(HighShelf1, self->sampleRate, dynFreq, 0.0, treble_db);
        self->last_treble = *self->treble;
    }

    if (*self->bass != self->last_bass)
    {
        float bass_db = *self->bass * 1.5f; // -10..+10 -> -15..+15
        double turn = std::abs(*self->bass) / 10.0;

        // Target:
        // 0.0 (Center) -> 80 Hz
        // 1.0 (Max Cut) -> 600 Hz
        // Linear Interpolation: 80 + (Turn * 520)
        double dynFreq = 80.0 + (turn * 520.0);

        self->low_shelf.calculateCoefficients(LowShelf1, self->sampleRate, dynFreq, 0.0, bass_db);
        self->last_bass = *self->bass;
    }
    #endif

    // Main processing loop
    for (uint32_t i = 0; i < n_samples; i++)
    {
        float sample = self->input[i];
        
        sample = self->low_shelf.process(sample);
        sample = self->high_shelf.process(sample);
        #ifdef PULVERIZE_MODE
        sample = self->peak.process(sample);
        #endif
        sample = self->analog_lpf.process(sample);
        
        self->output[i] = sample * volume_lin;
    }
}

// Used to cleanup plugin instance
static void cleanup(LV2_Handle instance)
{
    delete (Integra*)instance;
}

// This is the plugin descriptor
static const LV2_Descriptor descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    nullptr, // activate
    run,
    nullptr, // deactivate
    cleanup,
    nullptr  //extension_data
};

// Entry point for LV2
LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    return index == 0 ? &descriptor : NULL;
}
