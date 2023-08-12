/*
  ==============================================================================
    PlaylistComponent.h
    Created: 25 Jan 2023 9:34:27pm
    Author:  yingjie
  ==============================================================================
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"


using namespace juce;

class PlaylistComponent : public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public TextEditor::Listener, //Receives callbacks from a TextEditor component when it changes.
                          public FileDragAndDropTarget
                                
{
    public:
        PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2);
        ~PlaylistComponent() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        /* TableListBoxModel functions  */
        void paintRowBackground(Graphics& g,
            int rowNumber,
            int width,
            int height,
            bool rowIsSelected) override;

        void paintCell(Graphics& g,
            int rowNumber,
            int columnId,
            int width,
            int height,
            bool rowIsSelected) override;

        Component* refreshComponentForCell(int rowNumber,
            int columnId,
            bool isRowSelected,
            Component* existingComponentToUpdate) override;
        
       

        // get duration of song
        double getSongDuration(File file);
       
        // fix bug 
        int getNumRows() override;

        // button function 
        void buttonClicked(Button* button) override;


        // save song vector into file 
        void saveFile(std::vector<juce::File>, std::string fileName);


        // load song from saved csv file
        void loadSavedFile(std::string fileName);


        //tokenize function for getting filenames in csvfile line
        static std::vector<std::string> tokenise(std::string csvLine, char separator);



        bool isInterestedInFileDrag(const StringArray& files)override;
        void filesDropped(const StringArray& files, int x, int y)override;



    private:

        TableListBox tableComponent;
        juce::AudioFormatManager playlistCompFormatManager;

        //direct when selecting file
        juce::FileChooser fChooser{"Select song to playlist....."};
        
        // initialize text button and labels 
        TextButton addSongButton{ "Add Song" };
        Label searchBoxLabel;
        TextEditor inputBox;
        
        
        //vector for handle song duration length for song files 
        
        //vector for handle all song files and to-be-displayed song files
        std::vector <juce::File> songEntries;
        std::vector <juce::File> songEntriesToShow;

        // pointers for two individual deck , so we can load to only selected deck
        DeckGUI* deckOne;
        DeckGUI* deckTwo;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};




