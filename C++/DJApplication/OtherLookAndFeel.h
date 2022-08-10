/*
  ==============================================================================

    OtherLookAndFeel.h
    Created: 22 Feb 2022 7:46:01pm
    Author:  wngbi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OtherLookAndFeel  : public juce::Component,
                          public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    ~OtherLookAndFeel();

    void paint (juce::Graphics&) override;
    void resized() override;

    // function to configure the looks of a rotary slider
    void drawRotarySlider(juce::Graphics& g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          const float rotaryStartAngle,
                          const float rotaryEndAngle,
                          juce::Slider&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OtherLookAndFeel)
};
