#ifndef _H_MOOGFILTER_
#define _H_MOOGFILTER_

// framework
#include "DistrhoPlugin.hpp"

// classes

// libs
#include <cstdint>
#include <iostream>

// constants
constexpr uint32_t kNumChannels = 2;

START_NAMESPACE_DISTRHO

/**
  Your plugin class that subclases the base DPF Plugin one.
*/
class MoogFilter : public Plugin
{
public:
    enum Parameters {
        kCutoffFreq,
        kResonance,
        kDrive,
        kOutputGain,
        kNumParams
    };

    /**
      Plugin class constructor.
      You must set all parameter values to their defaults, matching the value in initParameter().
    */
    MoogFilter();

protected:
    // -------------------------------------------------------------------
    // Information

    /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const noexcept override { return "moogFilter"; }

    /**
      Get an extensive comment/description about the plugin.
      Optional, returns nothing by default.
    */
    const char* getDescription() const override { return "sve-moogFilter"; }

    /**
      Get the plugin author/maker.
    */
    const char* getMaker() const noexcept override { return "SvE plugins"; }

    /**
      Get the plugin license (a single line of text or a URL).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const noexcept override { return "ISC"; }

    /**
      Get the plugin version, in hexadecimal.
      @see d_version()
    */
    uint32_t getVersion() const noexcept override { return d_version(0, 0, 0); }

    /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
      @see d_cconst()
    */
    int64_t getUniqueId() const noexcept override { return d_cconst('S', 'v', 'E', 'T'); }

    // -------------------------------------------------------------------
    // Init

    /**
      Initialize the parameter @ index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override;

    /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime
      operations are performed.
    */
    void setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // methods
    // -------------------------------------------------------------------

    // -------------------------------------------------------------------
    // Process

    void activate() override;

    void deactivate() override;

    /**
      Run/process function for plugins with MIDI input.
    */
    void run(const float**, float**, uint32_t nframes) override;

    // -------------------------------------------------------------------
    // callbacks

    /**
        Optional callback to inform the plugin about a sample rate change.
        This function will only be called when the plugin is deactivated.
    */
    void sampleRateChanged(double newSampleRate) override;

private:
    float fCutoffFreq;
    float fResonance;
    float fDrive;
    float fOutputGain;
    float fDelay[kNumChannels][4];
    float fState[kNumChannels][2];
    /**
        Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MoogFilter)
};

END_NAMESPACE_DISTRHO

#endif // _H_MOOGFILTER_