/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Feb 2022 10:31:25am
    Author:  wngbi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);


    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();
    // get the file length in seconds 
    double getLengthInSeconds();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

};
