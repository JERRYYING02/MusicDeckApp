/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{   

 
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(resetButton);


       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);



    //create label design 
    addAndMakeVisible(volLabel);
    volLabel.setText("Volume", juce::dontSendNotification);

    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);


    addAndMakeVisible(roomLabel);
    roomLabel.setText("Room", juce::dontSendNotification);


    addAndMakeVisible(dampLabel);
    dampLabel.setText("Damp", juce::dontSendNotification);

    addAndMakeVisible(wetLabel);
    wetLabel.setText("Wet", juce::dontSendNotification);

    addAndMakeVisible(dryLabel);
    dryLabel.setText("Dry", juce::dontSendNotification);



    //reverbs sliders
    addAndMakeVisible(roomSlider);
    addAndMakeVisible(dampSlider);
    addAndMakeVisible(wetSlider);
    addAndMakeVisible(drySlider);


    //add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    resetButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    roomSlider.addListener(this);
    dampSlider.addListener(this);
    wetSlider.addListener(this);
    drySlider.addListener(this);

    //set ranges of sliders
    volSlider.setRange(0.0,1.0);
    volSlider.setValue(0.3);
    speedSlider.setRange(0.0,4.0);
    speedSlider.setValue(1);

    posSlider.setRange(0.0, 1.0);

    //reverb ranges
    roomSlider.setRange(0.0, 1.0);
    dampSlider.setRange(0.0, 1.0);
    wetSlider.setRange(0.0, 1.0);
    drySlider.setRange(0.0, 1.0);
    drySlider.setValue(1.0);

    startTimer(500);






}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */


    //design to play ,stop, load button
   
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    stopButton.setColour(TextButton::buttonColourId, Colours::orangered);
    loadButton.setColour(TextButton::buttonColourId, Colours::orange);
    resetButton.setColour(TextButton::buttonColourId, Colours::mediumslateblue);


    //design to volslider,speedSlider,posSlider
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    volSlider.setColour(juce::Slider::textBoxBackgroundColourId, Colours::darkviolet);
    volSlider.setColour(juce::Slider::rotarySliderOutlineColourId, Colours::darkviolet);
    volSlider.setColour(juce::Slider::rotarySliderFillColourId, Colours::violet);
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colours::palegoldenrod);



    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    speedSlider.setColour(juce::Slider::textBoxBackgroundColourId, Colours::teal);
    speedSlider.setColour(juce::Slider::rotarySliderOutlineColourId, Colours::teal);
    speedSlider.setColour(juce::Slider::rotarySliderFillColourId, Colours::turquoise);
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colours::palegoldenrod);


    posSlider.setTextBoxStyle(Slider::NoTextBox, true, 150, 500);
    posSlider.setColour(juce::Slider::backgroundColourId, Colours::darkslategrey);
    posSlider.setColour(juce::Slider::trackColourId, Colours::mediumaquamarine);
    posSlider.setColour(juce::Slider::thumbColourId, juce::Colours::palegoldenrod);


    //design to reverb sliders(roomSlider,dampSlider,wetSlider,drySlider)

    roomSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    roomSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 100);
    roomSlider.setColour(Slider::textBoxOutlineColourId, Colours::darkmagenta);
    roomSlider.setColour(Slider::trackColourId, Colours::darkmagenta);

    dampSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    dampSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 100);
    dampSlider.setColour(Slider::textBoxOutlineColourId, Colours::darkmagenta);
    dampSlider.setColour(Slider::trackColourId, Colours::darkmagenta);

    wetSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    wetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 100);
    wetSlider.setColour(Slider::textBoxOutlineColourId, Colours::darkmagenta);
    wetSlider.setColour(Slider::trackColourId, Colours::darkmagenta);


    drySlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    drySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 100);
    drySlider.setColour(Slider::textBoxOutlineColourId, Colours::darkmagenta);
    drySlider.setColour(Slider::trackColourId, Colours::darkmagenta);


    //background design 
    g.fillAll (juce::Colours::black);   // clear the background
    g.setColour (getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
}

void DeckGUI::resized()
{
    double rowH = getHeight() /8; 
    playButton.setBounds(20, 15, getWidth()/4, rowH);
    stopButton.setBounds(20, rowH+20, getWidth()/4, rowH); 
    loadButton.setBounds(20, rowH * 2+25, getWidth()/4, rowH);
    loopButton.setBounds(20, rowH * 3+35, getWidth() / 4, rowH);
    resetButton.setBounds(getWidth() / 1.2, rowH * 2 + 30, getWidth() / 10, rowH);

    volSlider.setBounds(getWidth()/3, 0, getWidth() / 3, rowH*2);
    volSlider.setNumDecimalPlacesToDisplay(1);
    speedSlider.setBounds(getWidth()/1.5, 0, getWidth() /3, rowH*2);
    speedSlider.setNumDecimalPlacesToDisplay(1);

    posSlider.setBounds(0, rowH * 7, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    

    //vertical bars
    roomSlider.setBounds(getWidth() / 3, rowH * 2 + 25, getWidth() / 10, rowH * 2);
    roomSlider.setNumDecimalPlacesToDisplay(2);

    dampSlider.setBounds(getWidth() / 2.2, rowH * 2 + 25, getWidth() / 10, rowH * 2);
    dampSlider.setNumDecimalPlacesToDisplay(2);
    
    wetSlider.setBounds(getWidth() /1.75, rowH * 2 + 25, getWidth() / 10, rowH * 2);
    wetSlider.setNumDecimalPlacesToDisplay(2);
   
    drySlider.setBounds(getWidth()/1.45, rowH * 2 + 25, getWidth() / 10, rowH * 2);
    drySlider.setNumDecimalPlacesToDisplay(2);


    //label positions
    volLabel.setBounds(getWidth()/3, 0, 50, rowH );
    speedLabel.setBounds(getWidth()/1.5, 0, 50, rowH );

    roomLabel.setBounds(getWidth() / 3, rowH * 2 + 10, 50, 10);
    dampLabel.setBounds(getWidth() / 2.2, rowH * 2 + 10, 50, 10);
    wetLabel.setBounds(getWidth() / 1.75, rowH * 2 + 10,50, 10);
    dryLabel.setBounds(getWidth() / 1.45, rowH * 2 + 10, 50, 10);



}

//call functions when play,stop,load,loop,reset button is clicked
void DeckGUI::buttonClicked(Button* button)
{   
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    else if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    else if (button == &loadButton)
    {
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {  
                
            File chosenFile = chooser.getResult();
            if (!chosenFile.existsAsFile())
            {
                // Show an alert window if no file is chosen
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
                    "Error",
                    "No file chosen",
                    "Ok");
                return;
            };

            player->loadURL(URL{chooser.getResult()});
            // load into waveformdisplay also
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
    }
    else if (button == &loopButton)
    {   
        //get the toggle state of button and assign to the djaudiplayer setloop function
        DBG("loop button was clicked ");
        player->setLoop(loopButton.getToggleState());
        
    }
    //reset the preset slider values to defaults
    else if (button == &resetButton)
    {
        volSlider.setValue(0.3);
        speedSlider.setValue(1.0);
        roomSlider.setValue(0.0);  
        dampSlider.setValue(0.0);
        wetSlider.setValue(0.0);
        drySlider.setValue(1.0);
    }


}





//detect changes in slider value
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    //reverbs

    if (slider == &roomSlider)
    {
        player->setReverbRoom(slider->getValue());
    }
    if (slider == &dampSlider)
    {
        player->setReverbDamp(slider->getValue());
    }
    if (slider == &wetSlider)
    {
        player->setReverbWet(slider->getValue());
    }
    if (slider == &drySlider)
    {
        player->setReverbDry(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI:: filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{   
    //only get position when song is loaded(not nan)
    if (!isnan(player->getPositionRelative()))
    {
        //posSlider thumb move along with waveformdisplay 
        posSlider.setValue(player->getPositionRelative());
        waveformDisplay.setPositionRelative(player->getPositionRelative());
        
    }
}


//get individual player and waveformdisplay for two decks
DJAudioPlayer* DeckGUI::buildPlayer()
{
    return player;
};

WaveformDisplay* DeckGUI::buildWaveformDisplay()
{
    return &waveformDisplay;
};


    

