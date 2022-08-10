/*
  ==============================================================================

    OtherLookAndFeel.cpp
    Created: 22 Feb 2022 7:46:01pm
    Author:  wngbi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OtherLookAndFeel.h"

//==============================================================================
OtherLookAndFeel::OtherLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

OtherLookAndFeel::~OtherLookAndFeel()
{

}

void OtherLookAndFeel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void OtherLookAndFeel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill rotary slider 
    g.setColour(juce::Colours::darkcyan);
    g.fillEllipse(rx, ry, rw, rw);

    // outline of the rotary slider 
    g.setColour(juce::Colours::transparentBlack);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // slider dial
    g.setColour(juce::Colours::black);
    g.fillPath(p);
}
