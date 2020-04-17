/*
 * Copyright 2017-2018 Leo McCormack
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
 * @file: binauraliser.h
 * @brief Convolves input audio (up to 64 channels) with interpolated HRTFs in
 *        the time-frequency domain.
 *
 * The HRTFs are interpolated by applying amplitude-preserving VBAP gains to the
 * HRTF magnitude responses and inter-aural time differences (ITDs)
 * individually, before being re-combined. The example also allows the user to
 * specify an external SOFA file for the convolution, and rotations of the
 * source directions to accomodate head-tracking.
 *
 * @author Leo McCormack
 * @date 25.09.2017
 */

#ifndef __BINAURALISER_H_INCLUDED__
#define __BINAURALISER_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                             Presets + Constants                            */
/* ========================================================================== */
     
/**
 * Available source configurations presets
 */
typedef enum _BINAURALISER_SOURCE_CONFIG_PRESETS{
    SOURCE_CONFIG_PRESET_DEFAULT = 1,
    SOURCE_CONFIG_PRESET_MONO,
    SOURCE_CONFIG_PRESET_STEREO,
    SOURCE_CONFIG_PRESET_5PX,
    SOURCE_CONFIG_PRESET_7PX,
    SOURCE_CONFIG_PRESET_8PX,
    SOURCE_CONFIG_PRESET_9PX,
    SOURCE_CONFIG_PRESET_10PX,
    SOURCE_CONFIG_PRESET_11PX,
    SOURCE_CONFIG_PRESET_11PX_7_4,
    SOURCE_CONFIG_PRESET_13PX,
    SOURCE_CONFIG_PRESET_22PX,
    SOURCE_CONFIG_PRESET_AALTO_MCC,
    SOURCE_CONFIG_PRESET_AALTO_MCC_SUBSET,
    SOURCE_CONFIG_PRESET_AALTO_APAJA,
    SOURCE_CONFIG_PRESET_AALTO_LR,
    SOURCE_CONFIG_PRESET_DTU_AVIL,
    SOURCE_CONFIG_PRESET_ZYLIA_LAB,
    SOURCE_CONFIG_PRESET_T_DESIGN_4,
    SOURCE_CONFIG_PRESET_T_DESIGN_12,
    SOURCE_CONFIG_PRESET_T_DESIGN_24,
    SOURCE_CONFIG_PRESET_T_DESIGN_36,
    SOURCE_CONFIG_PRESET_T_DESIGN_48,
    SOURCE_CONFIG_PRESET_T_DESIGN_60
    
}BINAURALISER_SOURCE_CONFIG_PRESETS;

typedef enum _INTERP_MODES{
    INTERP_TRI = 1  /* Triangular interpolation */
}INTERP_MODES;
    
/**
 * Current status of the codec.
 */
typedef enum _BINAURALISER_CODEC_STATUS{
    CODEC_STATUS_INITIALISED = 0, /**< Codec is initialised and ready to process
                                   *   input audio. */
    CODEC_STATUS_NOT_INITIALISED, /**< Codec has not yet been initialised, or
                                   *   the codec configuration has changed.
                                   *   Input audio should not be processed. */
    CODEC_STATUS_INITIALISING     /**< Codec is currently being initialised,
                                   *   input audio should not be processed. */

}BINAURALISER_CODEC_STATUS;

#define BINAURALISER_MAX_NUM_INPUTS ( 64 )
#define BINAURALISER_PROGRESSBARTEXT_CHAR_LENGTH 256


/* ========================================================================== */
/*                               Main Functions                               */
/* ========================================================================== */

/**
 * Creates an instance of the binauraliser
 *
 * @param[in] phBin (&) address of binauraliser handle
 */
void binauraliser_create(void** const phBin);

/**
 * Destroys an instance of the binauraliser
 *
 * @param[in] phBin (&) address of binauraliser handle
 */
void binauraliser_destroy(void** const phBin);

/**
 * Initialises an instance of binauraliser with default settings
 *
 * @param[in] hBin       binauraliser handle
 * @param[in] samplerate Host samplerate.
 */
void binauraliser_init(void* const hBin,
                       int samplerate);

/**
 * Intialises the codec variables, based on current global/user parameters
 *
 * @param[in] hBin binauraliser handle
 */
void binauraliser_initCodec(void* const hBin);

/**
 * Binauralises the input signals at the user specified directions
 *
 * @param[in] hBin      binauraliser handle
 * @param[in] inputs    Input channel buffers; 2-D array: nInputs x nSamples
 * @param[in] outputs   Output channel buffers; 2-D array: nOutputs x nSamples
 * @param[in] nInputs   Number of input channels
 * @param[in] nOutputs  Number of output channels
 * @param[in] nSamples  Number of samples in 'inputs'/'output' matrices
 */
void binauraliser_process(void* const hBin,
                          float** const inputs,
                          float** const outputs,
                          int nInputs,
                          int nOutputs,
                          int nSamples);


/* ========================================================================== */
/*                                Set Functions                               */
/* ========================================================================== */

/**
 * Sets all intialisation flags to 1; re-initialising all settings/variables
 * as binauraliser is currently configured, at next available opportunity.
 */
void binauraliser_refreshSettings(void* const hBin);
    
/**
 * Sets the panning azimuth for a specific channel index, in DEGREES
 */
void binauraliser_setSourceAzi_deg(void* const hBin,
                                   int index,
                                   float newAzi_deg);

/**
 * Sets the panning elevation for a specific channel index, in DEGREES
 */
void binauraliser_setSourceElev_deg(void* const hBin,
                                    int index,
                                    float newElev_deg);

/**
 * Sets the number of input channels/sources to binauralise.
 */
void binauraliser_setNumSources(void* const hBin, int new_nSources);

/**
 * Sets flag to dictate whether the default HRIRs in the Spatial_Audio_Framework
 * should be used (1), or a custom HRIR set loaded via a SOFA file (0).
 *
 * @note If the custom set fails to load correctly, binauraliser will revert to
 *       the defualt set. Use binauraliser_getUseDefaultHRIRsflag() to check if
 *       loading was successful.
 */
void binauraliser_setUseDefaultHRIRsflag(void* const hBin, int newState);

/**
 * Sets the file path for a .sofa file, in order to employ a custom HRIR set for
 * the decoding.
 *
 * @note If the custom set fails to load correctly, binauraliser will revert to
 *       the defualt set. Use binauraliser_getUseDefaultHRIRsflag() to check if
 *       loading was successful.
 *
 * @param[in] hBin binauraliser handle
 * @param[in] path File path to .sofa file (WITH file extension)
 */
void binauraliser_setSofaFilePath(void* const hBin, const char* path);

/**
 * Loads an input preset (see "BINAURALISER_SOURCE_CONFIG_PRESETS" enum)
 */
void binauraliser_setInputConfigPreset(void* const hBin, int newPresetID);

/**
 * Sets the flag to enable/disable (1 or 0) rotation.
 */
void binauraliser_setEnableRotation(void* const hBin, int newState);

/**
 * Sets the 'yaw' rotation angle, in DEGREES
 */
void binauraliser_setYaw(void* const hBin, float newYaw);

/**
 * Sets the 'pitch' rotation angle, in DEGREES
 */
void binauraliser_setPitch(void* const hBin, float newPitch);

/**
 * Sets the 'roll' rotation angle, in DEGREES
 */
void binauraliser_setRoll(void* const hBin, float newRoll);

/**
 * Sets a flag as to whether to "flip" the sign of the current 'yaw' angle
 * (0: do not flip sign, 1: flip the sign)
 */
void binauraliser_setFlipYaw(void* const hBin, int newState);

/**
 * Sets a flag as to whether to "flip" the sign of the current 'pitch' angle
 * (0: do not flip sign, 1: flip the sign)
 */
void binauraliser_setFlipPitch(void* const hBin, int newState);

/**
 * Sets a flag as to whether to "flip" the sign of the current 'roll' angle
 * (0: do not flip sign, 1: flip the sign)
 */
void binauraliser_setFlipRoll(void* const hBin, int newState);

/**
 * Sets a flag as to whether to use "yaw-pitch-roll" (0) or "roll-pitch-yaw" (1)
 * rotation order.
 */
void binauraliser_setRPYflag(void* const hBin, int newState);

/** NOT IMPLEMENTED YET */
void binauraliser_setInterpMode(void* const hBin, int newMode);


/* ========================================================================== */
/*                                Get Functions                               */
/* ========================================================================== */

/**
 * Returns current codec status codec status (see 'BINAURALISER_CODEC_STATUS'
 * enum)
 */
BINAURALISER_CODEC_STATUS binauraliser_getCodecStatus(void* const hBin);

/**
 * (Optional) Returns current intialisation/processing progress, between 0..1
 * - 0: intialisation/processing has started
 * - 1: intialisation/processing has ended
 */
float binauraliser_getProgressBar0_1(void* const hBin);

/**
 * (Optional) Returns current intialisation/processing progress text
 *
 * @note "text" string should be (at least) of length:
 *       BINAURALISER_PROGRESSBARTEXT_CHAR_LENGTH
 */
void binauraliser_getProgressBarText(void* const hBin, char* text);

/**
 * Returns the source azimuth for a given index, in DEGREES
 */
float binauraliser_getSourceAzi_deg(void* const hBin, int index);

/**
 * Returns the source elevation for a given index, in DEGREES
 */
float binauraliser_getSourceElev_deg(void* const hBin, int index);

/**
 * Returns the number of inputs/sources in the current layout
 */
int binauraliser_getNumSources(void* const hBin);

/**
 * Returns the maximum number of input sources supported by binauraliser
 */
int binauraliser_getMaxNumSources(void);

/**
 * Returns the number of ears possessed by the average homo sapien
 */
int binauraliser_getNumEars(void);

/**
 * Returns the number of directions in the currently used HRIR set
 */
int binauraliser_getNDirs(void* const hBin);

/**
 * Returns the number of triangular groupings (faces) returned by the Convex
 * Hull
 */
int binauraliser_getNTriangles(void* const hBin);

/**
 * Returns the HRIR/HRTF azimuth for a given index, in DEGREES
 */
float binauraliser_getHRIRAzi_deg(void* const hBin, int index);

/**
 * Returns the HRIR/HRTF elevation for a given index, in DEGREES
 */
float binauraliser_getHRIRElev_deg(void* const hBin, int index);

/**
 * Returns the length of HRIRs in time-domain samples
 */
int binauraliser_getHRIRlength(void* const hBin);

/**
 * Returns the HRIR sample rate
 */
int binauraliser_getHRIRsamplerate(void* const hBin);

/**
 * Returns the value of a flag used to dictate whether the default HRIRs in the
 * Spatial_Audio_Framework should be used (1), or a custom HRIR set loaded via a
 * SOFA file (0).
 *
 * @note If the custom set fails to load correctly, binauraliser will revert to
 *       the defualt set, so this will be '1'
 */
int binauraliser_getUseDefaultHRIRsflag(void* const hBin);

/**
 * Returns the file path for a .sofa file.
 *
 * @note If the custom set fails to load correctly, binauraliser will revert to
 *       the defualt set. Use 'binauraliser_getUseDefaultHRIRsflag()' to check
 *       if loading was successful.
 *
 * @param[in] hBin binauraliser handle
 * @returns        File path to .sofa file (WITH file extension)
 */
char* binauraliser_getSofaFilePath(void* const hBin);

/**
 * Returns the DAW/Host sample rate
 */
int binauraliser_getDAWsamplerate(void* const hBin);

/**
 * Returns the flag value which dictates whether to enable/disable sound-field
 * rotation (0: disabled, 1: enabled)
 */
int binauraliser_getEnableRotation(void* const hBin);

/**
 * Returns the 'yaw' rotation angle, in DEGREES
 */
float binauraliser_getYaw(void* const hBin);

/**
 * Returns the 'pitch' rotation angle, in DEGREES
 */
float binauraliser_getPitch(void* const hBin);

/**
 * Returns the 'roll' rotation angle, in DEGREES
 */
float binauraliser_getRoll(void* const hBin);

/**
 * Returns a flag as to whether to "flip" the sign of the current 'yaw' angle
 * (0: do not flip sign, 1: flip the sign)
 */
int binauraliser_getFlipYaw(void* const hBin);

/**
 * Returns a flag as to whether to "flip" the sign of the current 'pitch' angle
 * (0: do not flip sign, 1: flip the sign)
 */
int binauraliser_getFlipPitch(void* const hBin);

/**
 * Returns a flag as to whether to "flip" the sign of the current 'roll' angle
 * (0: do not flip sign, 1: flip the sign)
 */
int binauraliser_getFlipRoll(void* const hBin);

/**
 * Returns a flag as to whether to use "yaw-pitch-roll" (0) or "roll-pitch-yaw"
 * (1) rotation order.
 */
int binauraliser_getRPYflag(void* const hBin);

/** NOT IMPLEMENTED YET */
int binauraliser_getInterpMode(void* const hBin);

/**
 * Returns the processing delay in samples (may be used for delay compensation
 * purposes)
 */
int binauraliser_getProcessingDelay(void);


#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* __BINAURALISER_H_INCLUDED__ */
