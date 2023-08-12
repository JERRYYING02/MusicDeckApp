/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager): formatManager(_formatManager)
{
   
}

DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{   

    //resampleSource.getNextAudioBlock(bufferToFill);
    reverbSource.getNextAudioBlock(bufferToFill);
  
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // if good file
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, 
        true)); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());          
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
   
}
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
      DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100"); 
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
         DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
    




void DJAudioPlayer::setLoop(bool isLooping)
{
    /*  if readerSource not null then the function sets the looping status of the
        "readerSource" object to the value of the argument "isLooping". 
        This means that if "isLooping" is "true", the audio will loop, and if "isLooping" is "false", the audio will not loop.
    */
    if (readerSource)
        readerSource->setLooping(isLooping);
}



//reverb functions
void DJAudioPlayer::setReverbRoom(double roomValue)
{
    reverbParam.roomSize = roomValue;
    reverbSource.setParameters(reverbParam);
}
    
void DJAudioPlayer::setReverbDamp(double dampValue)
{   
    reverbParam.damping = dampValue;
    reverbSource.setParameters(reverbParam);
}

void DJAudioPlayer::setReverbWet(double wetValue)
{
    reverbParam.wetLevel = wetValue;
    reverbSource.setParameters(reverbParam);
}

void DJAudioPlayer::setReverbDry(double dryValue)
{
    reverbParam.dryLevel = dryValue;
    reverbSource.setParameters(reverbParam);
}


