/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Feb 2022 10:32:40am
    Author:  wngbi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                 ) : player(_player),
                 waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setLookAndFeel(&otherLookAndFeel);


    // display text buttons 
    addAndMakeVisible(playButton);
    playButton.setColour(TextButton::buttonColourId, Colours::darkcyan);
    playButton.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(stopButton);
    stopButton.setColour(TextButton::buttonColourId, Colours::darkcyan);
    stopButton.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(loadButton);
    loadButton.setColour(TextButton::buttonColourId, Colours::darkcyan);
    loadButton.setColour(TextButton::textColourOffId, Colours::black);

    // display sliders 
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    // display the waveform 
    addAndMakeVisible(waveformDisplay);

    // configure volume slider label 
    addAndMakeVisible(volumeName);
    volumeName.setFont(juce::Font(12.0f, juce::Font::bold));
    volumeName.setText("Volume Slider", juce::dontSendNotification);
    volumeName.setColour(Label::textColourId, Colours::darkcyan);
    volumeName.setJustificationType(juce::Justification::centred);

    // configure speed slider label
    addAndMakeVisible(speedName);
    speedName.setFont(juce::Font(12.0f, juce::Font::bold));
    speedName.setText("Speed Slider", juce::dontSendNotification);
    speedName.setColour(Label::textColourId, Colours::darkcyan);
    speedName.setJustificationType(juce::Justification::centred);

    // configure position slider label
    addAndMakeVisible(positionName);
    positionName.setFont(juce::Font(12.0f, juce::Font::bold));
    positionName.setText("Position Slider", juce::dontSendNotification);
    positionName.setColour(Label::textColourId, Colours::darkcyan);
    positionName.setJustificationType(juce::Justification::centred);

    // setting volume slider to be linear vertical slider
    volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 30);

    // setting speed slider to be linear vertical slider
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 30);

    // setting position slider to be a rotary slider 
    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 30);

    // add listeners to text buttons
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    // add listeners to sliders 
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // setting slider range 
    volSlider.setRange(0.0, 1.0, 0.0001);
    speedSlider.setRange(0.0, 3.0, 0.0001);
    posSlider.setRange(0.0, 1.0, 0.0001);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(Colours::black);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::black);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // set positions of GUI components
    double rowH = getHeight() / 7;
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    posSlider.setBounds(-20, rowH * 2 + 35, getWidth() - 200, rowH * 3);
    volSlider.setBounds(200, rowH * 2 + 35, 80, rowH * 3);
    speedSlider.setBounds(300, rowH * 2 + 35, 80, rowH * 3);
    positionName.setBounds(40, rowH * 2 + 10, 80, 20);
    volumeName.setBounds(200, rowH * 2 + 10, 80, 20);
    speedName.setBounds(300, rowH * 2 + 10, 80, 20);
    playButton.setBounds(10, rowH * 6, 120, rowH);
    stopButton.setBounds(140, rowH * 6, 120, rowH);
    loadButton.setBounds(270, rowH * 6, 120, rowH);
}
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        // play the loaded file
        DBG("Play button was clicked!");
        player->start();
    }
    if (button == &stopButton)
    {
        // stop the loaded file from playing 
        DBG("Stop button was clicked!");
        player->stop();

    }
    if (button == &loadButton)
    {
        // select file and load file to deck
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            loadToDeck(URL{ chooser.getResult() });
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        // display the slider value upon change 
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        // display the slider value upon change 
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        // display the slider value upon change 
        player->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        loadToDeck(URL{ File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{

    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
}

void DeckGUI::drawRotarySlider(Graphics& g,
                               int 	x,
                               int 	y,
                               int 	width,
                               int 	height,
                               float sliderPos,
                               float rotaryStartAngle,
                               float rotaryEndAngle,
                               Slider&)
{
    // configure the look and paint of a rotary slider 
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(juce::Colours::darkcyan);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::transparentBlack);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::black);
    g.fillPath(p);
}
void DeckGUI::loadToDeck(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}
