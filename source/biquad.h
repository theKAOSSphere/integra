//
// biquad.h - Simple biquad filter implementation used by Integra
// Copyright (C) 2025 theKAOSSphere
// SPDX-License-Identifier: GPL-3.0-or-later
//

#ifndef BIQUAD_H
#define BIQUAD_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum FilterType { LowPass, HighPass, Peaking, LowShelf, HighShelf };

class Biquad {
public:
    Biquad() { reset(); }

    void reset() {
        x1 = x2 = y1 = y2 = 0.0;
        b0 = 1.0; // Start as a passthrough
        b1 = b2 = a1 = a2 = 0.0;
    }

    void calculateCoefficients(FilterType type, double fs, double freq, double Q, double dbGain) {
        double omega = 2.0 * M_PI * freq / fs;
        double cs = cos(omega);
        double sn = sin(omega);
        double A = pow(10.0, dbGain / 40.0);
        double alpha = sn / (2.0 * Q);

        double a0 = 1.0; // Normalization term

        switch (type) {
            case LowPass:
                b0 = (1.0 - cs) / 2.0;
                b1 = 1.0 - cs;
                b2 = (1.0 - cs) / 2.0;
                a0 = 1.0 + alpha;
                a1 = -2.0 * cs;
                a2 = 1.0 - alpha;
                break;
            case HighPass:
                b0 = (1.0 + cs) / 2.0;
                b1 = -(1.0 + cs);
                b2 = (1.0 + cs) / 2.0;
                a0 = 1.0 + alpha;
                a1 = -2.0 * cs;
                a2 = 1.0 - alpha;
                break;
            case Peaking:
                b0 = 1.0 + alpha * A;
                b1 = -2.0 * cs;
                b2 = 1.0 - alpha * A;
                a0 = 1.0 + alpha / A;
                a1 = -2.0 * cs;
                a2 = 1.0 - alpha / A;
                break;
            case LowShelf:
            {
                double sqrtA = sqrt(A);
                b0 = A * ((A + 1.0) - (A - 1.0) * cs + 2 * sqrtA * alpha);
                b1 = 2 * A * ((A - 1.0) - (A + 1.0) * cs);
                b2 = A * ((A + 1.0) - (A - 1.0) * cs - 2 * sqrtA * alpha);
                a0 = (A + 1.0) + (A - 1.0) * cs + 2 * sqrtA * alpha;
                a1 = -2 * ((A - 1.0) + (A + 1.0) * cs);
                a2 = (A + 1.0) + (A - 1.0) * cs - 2 * sqrtA * alpha;
                break;
            }
            case HighShelf:
            {
                double sqrtA = sqrt(A);
                b0 = A * ((A + 1.0) + (A - 1.0) * cs + 2 * sqrtA * alpha);
                b1 = -2 * A * ((A - 1.0) + (A + 1.0) * cs);
                b2 = A * ((A + 1.0) + (A - 1.0) * cs - 2 * sqrtA * alpha);
                a0 = (A + 1.0) - (A - 1.0) * cs + 2 * sqrtA * alpha;
                a1 = 2 * ((A - 1.0) - (A + 1.0) * cs);
                a2 = (A + 1.0) - (A - 1.0) * cs - 2 * sqrtA * alpha;
                break;
            }
        }

        // Normalize coefficients by a0
        double invA0 = 1.0 / a0;
        b0 *= invA0;
        b1 *= invA0;
        b2 *= invA0;
        a1 *= invA0;
        a2 *= invA0;
    }

    inline float process(float in) {
        double out = b0 * in + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        
        // Anti-denormalization
        if (std::abs(out) < 1.0e-20) out = 0.0;

        x2 = x1;
        x1 = in;
        y2 = y1;
        y1 = out;

        return (float)out;
    }

private:
    double b0, b1, b2, a1, a2;
    double x1, x2, y1, y2;
};

#endif // BIQUAD_H