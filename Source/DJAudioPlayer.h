/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource{
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    
    //playback functions
    void start();
    void stop();
    void setLoop(bool isLooping);
    /** get the relative position of the playhead */
    double getPositionRelative();



    /* setting values of reverbs */
    void setReverbRoom(double roomValue);
    void setReverbDamp(double dampValue);
    void setReverbWet(double wetValue);
    void setReverbDry(double dryValue);



private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};

    ReverbAudioSource reverbSource{ &resampleSource, false };
    Reverb::Parameters reverbParam;
};




