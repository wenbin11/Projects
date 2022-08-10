/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Feb 2022 10:32:40am
    Author:  wngbi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "OtherLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer,
    public LookAndFeel_V4
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;


    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

    // configure looks of a rotary slider 
    void drawRotarySlider(juce::Graphics& g, 
                          int x, 
                          int y, 
                          int width, 
                          int height, 
                          float sliderPos,
                          const float rotaryStartAngle, 
                          const float rotaryEndAngle, 
                          juce::Slider&) override;

    // function to load URL to deck 
    void loadToDeck(juce::URL audioURL);

private:

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    TextButton randomButton{ "Shuffle" };

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    Label positionName;
    Label volumeName;
    Label speedName;

    Font buttonFont;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;
    OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
