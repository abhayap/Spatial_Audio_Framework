/*
 * Copyright 2019 Leo McCormack
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file: saf_filters.h
 * @brief Contains a collection of filter design equations
 *
 * @author Leo McCormack
 * @date 01.03.2019 
 */

#ifndef SAF_FILTERS_H_INCLUDED
#define SAF_FILTERS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>

/* ========================================================================== */
/*                                    Enums                                   */
/* ========================================================================== */

/**
 * Bi-quadratic (second-order) IIR filter design options.
 */
typedef enum _BIQUAD_FILTER_TYPES {
    BIQUAD_FILTER_LPF,       /**< low-pass filter */
    BIQUAD_FILTER_HPF,       /**< high-pass filter */
    BIQUAD_FILTER_PEAK,      /**< peaking filter */
    BIQUAD_FILTER_LOW_SHELF, /**< low-shelving filter */
    BIQUAD_FILTER_HI_SHELF   /**< high-shelving filter */
    
}BIQUAD_FILTER_TYPES;

/**
 * Finite Impulse Response (FIR) filter design options.
 */
typedef enum _FIR_FILTER_TYPES {
    FIR_FILTER_LPF, /**< low-pass filter */
    FIR_FILTER_HPF, /**< high-pass filter */
    FIR_FILTER_BPF, /**< band-pass filter */
    FIR_FILTER_BSF  /**< band-stop filter */
    
}FIR_FILTER_TYPES;

/**
 * Windowing function types.
 *
 * Symmetric if winlength is odd, and asymmetric if winlength is even. Windows
 * are evaluated: 0 <= n < winlength. Largely taken from:
 * https://en.wikipedia.org/wiki/Window_function
 */
typedef enum _WINDOWING_FUNCTION_TYPES {
    WINDOWING_FUNCTION_RECTANGULAR,      /**< Rectangular */
    WINDOWING_FUNCTION_HAMMING,          /**< Hamming */
    WINDOWING_FUNCTION_HANN,             /**< Hann */
    WINDOWING_FUNCTION_BARTLETT,         /**< Bartlett */
    WINDOWING_FUNCTION_BLACKMAN,         /**< Blackman */
    WINDOWING_FUNCTION_NUTTALL,          /**< Nuttall */
    WINDOWING_FUNCTION_BLACKMAN_NUTTALL, /**< Blackman-Nuttall */
    WINDOWING_FUNCTION_BLACKMAN_HARRIS   /**< Blackman-Harris */
    
}WINDOWING_FUNCTION_TYPES;


/* ========================================================================== */
/*                               Misc. Functions                              */
/* ========================================================================== */

/**
 * Computes the weights of a specific windowing function.
 *
 * Weights are symmetric if winlength is odd, and are asymmetric if winlength is
 * even.
 *
 * i.e. if winlength is even:
 *  - index "winlength/2" = 1, and first value!=last value
 *
 * if odd:
 *  - index "(winlength-1)/2" = 1, and first value==last value
 *
 * @param[in]  type      See 'WINDOWING_FUNCTION_TYPES' enum
 * @param[in]  winlength Window length in samples
 * @param[out] win       Windowing function; winlength x 1
 */
void getWindowingFunction(WINDOWING_FUNCTION_TYPES type,
                          int winlength,
                          float* win);

/**
 * Converts octave band CENTRE frequencies into CUTOFF frequencies.
 *
 * The lower and upper CENTRE frequencies only have their upper and lower
 * CUTOFF frequencies computed, respectively. e.g.:
 *  - centreFreqs[6] = { 125, 250, 500, 1000, 2000, 4000 },
 *
 * becomes:
 *  - cutoffFreqs[5] = { 176, 354, 707, 1410, 2830 }
 *
 * Passing cutoffFreqs[5] to FIRFilterbank(), will give filter coefficients for
 * the following:
 *  - Band1: LPF @ 176Hz
 *  - Band2: BFP @ 176-354Hz
 *  - Band3: BFP @ 354-707Hz
 *  - Band4: BFP @ 707-1410Hz
 *  - Band5: BFP @ 1410-2830Hz
 *  - Band6: HPF @ 2830Hz
 *
 * (Basically, band 125Hz also encapsulates everything down to DC, and band 4kHz
 * also encapsulates everything up to Nyquist)
 *
 * @note cutoffFreqs vector is shorter than centreFreqs by 1 element.
 *
 * @param[in]  centreFreqs  Centre frequencies (octave bands); nCentreFreqs x 1
 * @param[in]  nCentreFreqs Number of centre frequencies
 * @param[out] cutoffFreqs  Cutoff frequencies, which encapsulate the specified
 *                          centre frequencies by 1 octave; (nCentreFreqs-1) x 1
 */
void getOctaveBandCutoffFreqs(float* centreFreqs,
                              int nCentreFreqs,
                              float* cutoffFreqs);

/**
 * Equalises input sequence by its minimum phase form, in order to bring its
 * magnitude response to unity.
 *
 * @param[in,out] x   Input; len x 1
 * @param[in]     len Length of input
 */
void flattenMinphase(float* x,
                     int len);


/* ========================================================================== */
/*                              Bi-Quad Functions                             */
/* ========================================================================== */

/**
 * Calculates 2nd order IIR filter coefficients [1]
 *
 * @param[in]  filterType See 'BIQUAD_FILTER_TYPES' enum
 * @param[in]  fc         Centre frequency, Hz
 * @param[in]  fs         Sampling frequency, Hz
 * @param[in]  Q          Q-factor
 * @param[in]  gain_dB    Gain, dB
 * @param[out] b          b filter coefficients; 3 x 1
 * @param[out] a          a filter coefficients; 3 x 1
 *
 * @see [1] Zo"lzer, U. (Ed.). (2011). DAFX: digital audio effects. John Wiley
 *          & Sons.
 */
void biQuadCoeffs(/* input arguments */
                  BIQUAD_FILTER_TYPES filterType,
                  float fc,
                  float fs,
                  float Q,
                  float gain_dB,
                  /* output arguments */
                  float b[3],
                  float a[3]);

/**
 * Applies biQuad filter to an input signal using the direct form II difference
 * equation: https://en.wikipedia.org/wiki/Digital_biquad_filter
 *
 * @note input 'signal' is filtered in place (i.e. it becomes the output signal)
 *
 * @param[in]      b        b filter coefficients; 3 x 1
 * @param[in]      a        a filter coefficients; 3 x 1
 * @param[in,out]  w_z_12   Previous 2 wn samples (init as 0s); 2 x 1
 * @param[in,out]  signal   Signal to be filtered/filtered signal; nSamples x 1
 * @param[in]      nSamples Number of samples in the signal
 */
void applyBiQuadFilter(/* Input arguments */
                       float b[3],
                       float a[3],
                       float w_z_12[2],
                       float* signal,
                       int nSamples);

/**
 * Evaluates the 2nd order IIR transfer function at one or more frequencies,
 * returning its magnitude and/or phase response
 *
 * @param[in]  b            b filter coefficients; 3 x 1
 * @param[in]  a            a filter coefficients; 3 x 1
 * @param[in]  freqs        Frequencies at which to evaluate, Hz; nFreqs x 1
 * @param[in]  nFreqs       Number of frequencies at which to avaluate
 * @param[in]  fs           Sampling frequency, Hz
 * @param[out] magnitude_dB Magnitude, dB, at each frequency (set to NULL of not
 *                          wanted); nFreqs x 1
 * @param[out] phase_rad    Phase, radians, at each frequency (set to NULL of
 *                          not wanted); nFreqs x 1
 */
void evalBiQuadTransferFunction(/* Input arguments */
                                float b[3],
                                float a[3],
                                float* freqs,
                                int nFreqs,
                                float fs,
                                /* Output arguments */
                                float* magnitude_dB,
                                float* phase_rad);


/* ========================================================================== */
/*                            FIR Filter Functions                            */
/* ========================================================================== */

/**
 * Computes FIR filter coefficients by windowing.
 *
 * When using the Hamming window, and scalingFLAG=1, the function is numerically
 * identical to the default 'fir1' function in Matlab (when using it in single
 * precision mode) [1].
 *
 * @note Input argument 'order' cannot be odd valued.
 *
 * Some guidelines regarding the approx order (N) for certain filters. i.e.
 * the orders where you actually get the expected -6dB attenuation at the cutoff
 * frequency specified (fs=48kHz, Hamming window, scalingFLAG=1).
 * (Use these figures only just to get a rough idea)
 *  - LPF @ 100Hz - N~1400
 *  - LPF @ 250Hz - N~550
 *  - LPF @ 1kHz  - N~150
 *  - LPF @ 4kHz  - N~40
 *  - BPF @ 88-176Hz   - N~2500
 *  - BPF @ 176-354Hz  - N~1600
 *  - BPF @ 707-1410Hz - N~400
 *  - HPF @ 200Hz - N~450
 *  - HPF @ 4kHz  - N~60
 *
 * @param[in]  filterType  See 'FIR_FILTER_TYPES' enum
 * @param[in]  order       Filter order (N). Must be even.
 * @param[in]  cutoff1     Filter1 cutoff in Hz, for LPF/HPF, and lower cutoff
 *                         for BPF/BSF
 * @param[in]  cutoff2     Filter2 cutoff in Hz, not needed for LPF/HPF, this is
 *                         the upper cutoff for BPF/BSF
 * @param[in]  sampleRate  Sampling rate in Hz
 * @param[in]  windowType  See 'WINDOWING_FUNCTION_TYPES' enum
 * @param[in]  scalingFLAG '0' none, '1' scaling applied to ensure passband is
 *                         at 0dB
 * @param[out] filter      Filter coefficients/weights/taps; (order+1) x 1
 *
 * @see [1] "Programs for Digital Signal Processing", IEEE Press John Wiley &
 *          Sons, 1979, pg. 5.2-1.
 */
void FIRCoeffs(/* Input arguments */
               FIR_FILTER_TYPES filterType,
               int order,
               float cutoff1,
               float cutoff2,
               float sampleRate,
               WINDOWING_FUNCTION_TYPES windowType,
               int scalingFLAG,
               /* Output arguments */
               float* filter);

/**
 * Computes a bank of FIR filter coefficients required to divide a signal into
 * frequency bands.
 *
 * Provided the order is sufficient, the sum of the bands
 * should recontruct the original (although, shifted in time due to group delay)
 * e.g fc[1] = { 1000 };
 *  - Band1, &filter[0*(order+1)] : LPF @ 1kHz
 *  - Band2, &filter[1*(order+1)] : HPF @ 1kHz
 *
 * e.g fc[3] = { 1000, 2000, 4000 };
 *  - Band1, &filter[0*(order+1)] : LPF @ 1kHz
 *  - Band2, &filter[1*(order+1)] : BPF @ 1-2kHz
 *  - Band3, &filter[2*(order+1)] : BPF @ 2-4kHz
 *  - Band4, &filter[3*(order+1)] : HPF @ 4kHz
 *
 * @param[in]  order        Filter order. Must be even.
 * @param[in]  fc           Vector of cutoff frequencies; nCutoffFreqs x 1
 * @param[in]  nCutoffFreqs Number of cutoff frequencies in vector 'fc'.
 * @param[in]  sampleRate   Sampling rate in Hz
 * @param[in]  windowType   See 'WINDOWING_FUNCTION_TYPES' enum
 * @param[in]  scalingFLAG  '0' none, '1' scaling applied to ensure passbands
 *                          are at 0dB
 * @param[out] filterbank   Filter coefficients/weights/taps;
 *                          FLAT: (nCutoffFreqs+1) x (order+1)
 */
void FIRFilterbank(/* Input arguments */
                   int order,
                   float* fc,
                   int nCutoffFreqs,
                   float sampleRate,
                   WINDOWING_FUNCTION_TYPES windowType,
                   int scalingFLAG,
                   /* Output arguments */
                   float* filterbank);


#ifdef __cplusplus
}/* extern "C" */
#endif /* __cplusplus */

#endif /* SAF_FILTERS_H_INCLUDED */