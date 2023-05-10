#include "moogFilter.hpp"

START_NAMESPACE_DISTRHO

   /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching the value in initParameter().
    */
MoogFilter::MoogFilter()
    : Plugin(kNumParams, 0, 0)
{

    // Initialize filter parameters
    fCutoffFreq = 1000.0f;
    fResonance = 0.1f;
    fDrive = 0.0f;
    fOutputGain = 1.0f;

    // Initialize filter state
    for (uint32_t i = 0; i < kNumChannels; ++i) {
        for (uint32_t j = 0; j < 4; ++j) {
            fDelay[i][j] = 0.0f;

            fState[i][0] = 0.0f;
            fState[i][1] = 0.0f;
        }
    }
}


// -------------------------------------------------------------------
// Init
// -------------------------------------------------------------------

/**
  Initialize the parameter @a index.
  This function will be called once, shortly after the plugin is created.
*/
void MoogFilter::initParameter(uint32_t index, Parameter& parameter)
{
    if (index >= kNumParams) { return; }

    switch (index) {
        case kCutoffFreq:
            parameter.hints = kParameterIsAutomable | kParameterIsLogarithmic;
            parameter.name = "Cutoff";
            parameter.symbol = "cutoff";
            parameter.unit = "Hz";
            parameter.ranges.min = 20.0f;
            parameter.ranges.max = 20000.0f;
            parameter.ranges.def = 1000.0f;
            break;
        case kResonance:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Resonance";
            parameter.symbol = "resonance";
            parameter.unit = "";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.1f;
            break;
        case kDrive:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Drive";
            parameter.symbol = "drive";
            parameter.unit = "";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.0f;
            break;
        case kOutputGain:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Output Gain";
            parameter.symbol = "output_gain";
            parameter.unit = "dB";
            parameter.ranges.min = -60.0f;
            parameter.ranges.max = 12.0f;
            parameter.ranges.def = 0.0f;
            break;
    }
}

// -------------------------------------------------------------------
// Internal data
// -------------------------------------------------------------------

/**
  Get the current value of a parameter.
  The host may call this function from any context, including realtime processing.
*/
float MoogFilter::getParameterValue(uint32_t index) const
{
    switch (index) {
        case kCutoffFreq:
            return fCutoffFreq;
        case kResonance:
            return fResonance;
        case kDrive:
            return fDrive;
        case kOutputGain:
            return fOutputGain;
    }
}

/**
  Change a parameter value.
  The host may call this function from any context, including realtime processing.
  When a parameter is marked as automable, you must ensure no non-realtime
  operations are performed.
*/
void MoogFilter::setParameterValue(uint32_t index, float value)
{
    switch (index) {
        case kCutoffFreq:
            fCutoffFreq = value;
            break;
        case kResonance:
            fResonance = value;
            break;
        case kDrive:
            fDrive = value;
            break;
        case kOutputGain:
            fOutputGain = value;
            break;
        default:
            break;
    }
}

// -------------------------------------------------------------------
// methods
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// Process

void MoogFilter::activate()
{
    // plugin is activated
}

void MoogFilter::deactivate()
{
    // plugin is deactivated
}

/**
  Run/process function for plugins with MIDI input.
*/
void MoogFilter::run(const float** inputs, float** outputs, uint32_t nframes) {
        const float cutoff = fCutoffFreq / getSampleRate();
        const float resonance = fResonance;
        const float drive = fDrive;
        const float outputGain = fOutputGain;

        for (uint32_t i = 0; i < nframes; ++i) {
            // Apply drive
            const float in = inputs[0][i] * drive;

            // Calculate feedback and output gain
            const float fb = resonance + resonance / (1.0f - cutoff);
            const float g = outputGain / (1.0f + fb);

            // Calculate filter coefficients
            const float a0 = 1.0f / (1.0f + cutoff * (1.61803398875f - cutoff));
            const float a1 = 2.0f * a0;
            const float a2 = a0;
            const float b1 = 2.0f * (1.0f - cutoff) * a0;
            const float b2 = (1.0f - cutoff * (1.61803398875f - cutoff)) * a0;

            // Process filter for each channel
            for (uint32_t ch = 0; ch < kNumChannels; ++ch) {
                // Calculate filter output
                const float out = g * (in - fb * fDelay[ch][3]);

                // Shift delay line
                fDelay[ch][0] = fDelay[ch][1];
                fDelay[ch][1] = fDelay[ch][2];
                fDelay[ch][2] = fDelay[ch][3];
                fDelay[ch][3] = out;

                // Calculate output
                const float y = a0 * (fDelay[ch][0] + a1 * fDelay[ch][1] + a2 * fDelay[ch][2]) - b1 * fState[ch][0] -
                                b2 * fState[ch][1];
                fState[ch][1] = fState[ch][0];
                fState[ch][0] = y;

                // Write output
                outputs[ch][i] = y;
            }
        }
}

// -------------------------------------------------------------------
// callbacks

/**
    Optional callback to inform the plugin about a sample rate change.
    This function will only be called when the plugin is deactivated.
*/
void MoogFilter::sampleRateChanged(double newSampleRate)
{
    (void)newSampleRate;
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new MoogFilter();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO