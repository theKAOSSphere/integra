//
// integra.cpp - Integra LV2 Plugin
// Copyright (C) 2025 theKAOSSphere
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include <cstdlib>
#include <cmath>
#include "biquad.h"

#define INTEGRA_URI "https://github.com/theKAOSSphere/integra"

const double lowShelfFreq  = 154.0f;
const double highShelfFreq = 7200.0f;
const double shelfQ        = 0.707;

enum portIndex {
    PORT_INPUT = 0, 
    PORT_OUTPUT = 1, 
    PORT_BASS = 2, 
    PORT_TREBLE = 3, 
    PORT_VOLUME = 4
};

// This struct contains the stuff needed for the plugin instance
typedef struct integra
{
    // Port buffers
    const float* input;
    float*       output;

    // Control port pointers
    const float* bass;
    const float* treble;
    const float* volume;

    // DSP state
    Biquad low_shelf;
    Biquad high_shelf;

    double sampleRate;

    // Cached control values to avoid unnecessary recalculations
    float last_bass;
    float last_treble;
} Integra;

// This function creates an instance of the LV2 plugin
static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               bundle_path,
                              const LV2_Feature* const* features)
{
    Integra* self =  new Integra();
    if (!self) return NULL;

    self->sampleRate = rate;

    // Initialize cached values to NAN to force a calculation on the first run
    self->last_bass = NAN;
    self->last_treble = NAN;

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
        case PORT_BASS:     self->bass   = (const float*)data; break;
        case PORT_TREBLE:   self->treble = (const float*)data; break;
        case PORT_VOLUME:   self->volume = (const float*)data; break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
    Integra* self =  (Integra*)instance;

    // Parameter change check
    if (*self->bass != self->last_bass)
    {
        self->low_shelf.calculateCoefficients(LowShelf, self->sampleRate, lowShelfFreq, shelfQ, *self->bass);
        self->last_bass = *self->bass; // Update cache
    }

    if (*self->treble != self->last_treble)
    {
        self->high_shelf.calculateCoefficients(HighShelf, self->sampleRate, highShelfFreq, shelfQ, *self->treble);
        self->last_treble = *self->treble; // Update cache
    }

    const float volume  = powf(10.0f, *self->volume / 20.0f);

    for (uint32_t i = 0; i < n_samples; i++)
    {
        float sample = self->input[i];
        sample = self->low_shelf.process(sample);
        sample = self->high_shelf.process(sample);
        self->output[i] = sample * volume;
    }
}

// Used to cleanup plugin instance
static void cleanup(LV2_Handle instance)
{
    delete (Integra*)instance;
}

// This is the plugin descriptor
static const LV2_Descriptor descriptor = {
    INTEGRA_URI,
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
