/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 25 Jan 2023 9:34:27pm
    Author:  yingjie

  ==============================================================================
*/


#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"



//playlist component constructor
PlaylistComponent::PlaylistComponent(DeckGUI* deck1,
                                     DeckGUI* deck2) : deckOne(deck1), deckTwo(deck2)
{


    // load saved csv file ,so playlist restored when restart
    loadSavedFile("playlist.csv");

    //playlistcomponent column title bar

    tableComponent.getHeader().addColumn("Track title", 1, 320);
    tableComponent.getHeader().addColumn("Filetype", 2, 150);
    tableComponent.getHeader().addColumn("Duration", 3, 300);
    tableComponent.getHeader().addColumn("Deckplayer 1", 4, 100);
    tableComponent.getHeader().addColumn("Deckplayer 2", 5, 100);
    tableComponent.getHeader().addColumn("Deck Unload", 6, 100);
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::transparentBlack);
    //resize the columns
    tableComponent.getHeader().setStretchToFitActive(true);
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent); 

   

    // addSongButton makevisble and listener declaration
    addAndMakeVisible(addSongButton);
    addSongButton.addListener(this);
    addSongButton.setColour(TextButton::buttonColourId, juce::Colours::salmon);
    
    playlistCompFormatManager.registerBasicFormats();

    // searchBoxLabel 
    addAndMakeVisible(searchBoxLabel);
    searchBoxLabel.setText("Search song:", juce::dontSendNotification);
    searchBoxLabel.setColour(juce::Label::backgroundColourId, juce::Colours::salmon);
    searchBoxLabel.setColour(juce::Label::textColourId, juce::Colours::floralwhite);
    //searchBoxLabel.setJustificationType(juce::Justification::centredRight);
    
    //text input makevisble and listener declaration
    addAndMakeVisible(inputBox);
   

    inputBox.onTextChange = [this]
    {
        String inputText = inputBox.getText();

     
        //if inputbox is empty then refresh songEntriesToShow
        if (inputText.isEmpty())
        {
            songEntriesToShow = songEntries;      
            tableComponent.resized(); 
        }


        //if inputbox is searched
        else if (!inputText.isEmpty())
        {
            songEntriesToShow.clear();

            for (int i = 0; i < songEntries.size(); ++i)
            {
             
                // if found substring return file to songEntriesToShow
                std::string songFilename = songEntries[i].getFileName().toStdString();
                std::string searchedSong = inputText.toStdString();

                // convert both strings to lowercase(so we can search uppercase also)
                std::transform(songFilename.begin(), songFilename.end(), songFilename.begin(), ::tolower);
                std::transform(searchedSong.begin(), searchedSong.end(), searchedSong.begin(), ::tolower);

                //method : https://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
                //The find() method will attempt to locate the given string, searchedSong, within the string songFilename. 
                //If the search is successful, the method will return the index of the first character of the located string.
                if (songFilename.find(searchedSong) != std::string::npos)
                {
                    DBG("Song is found!");
                    songEntriesToShow.push_back(songEntries[i]);
                }
            }
            tableComponent.resized();
        }
        tableComponent.resized();
    };

   
}


//PlaylistComponent destructor class which is used to save a playlist to a file 
//clean up the data in the tableComponent object by setting its model 
//(which contains the data) to nullptr (a null pointer). The saveFile function is used to write the data to the file.
PlaylistComponent::~PlaylistComponent()
{
    // save songEntries to csv file
    saveFile(songEntries, "playlist.csv");
    tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.
       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll(Colours::black);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::antiquewhite);
    g.setFont(14.0f);
  
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowHeight = getHeight() / 10;
    double rowWidth = getWidth() / 10;

    searchBoxLabel.setBounds(rowWidth, 0, rowWidth*2, rowHeight);
    inputBox.setBounds(rowWidth * 2.2, 0, getWidth()/1.54, rowHeight);
    tableComponent.setBounds(rowWidth, rowHeight, getWidth()/1.299, getHeight());

    // add song button position 
    addSongButton.setBounds(0, rowHeight *9, getWidth(), rowHeight);
}

int PlaylistComponent::getNumRows()
{
    return songEntriesToShow.size();
};

/*
   method:https://stackoverflow.com/questions/71186547/how-to-read-the-length-of-audio-files-using-juce-c-without-playing-the-file
*/


//get song duration length
double PlaylistComponent::getSongDuration(File file)
{
    double duration = 0.0;
    /*
    
    If the reader is created, the length of the file in samples is 
    divided by the sample rate to get the duration in seconds.
    returned value is a double representing the duration of the song in seconds.
    
    */
    auto readFile = playlistCompFormatManager.createReaderFor(file);
    if (readFile)
    {
        auto durationSeconds = readFile->lengthInSamples / readFile->sampleRate;
        duration = durationSeconds;
    }

    return duration;

};




/* TableListBoxModel functions  */
void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    // when row is selected fill lightsalmon else default is black
    if (rowIsSelected)
    {
        g.fillAll(Colours::lightsalmon);
    }
    else
    {
        g.fillAll(Colours::black);
        g.setColour(Colours::white);
    }


};

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    /* Bug : 
        Note that the rowNumber value may be greater than 
        the number of rows in your list, 
        so be careful that you don't assume it's less than getNumRows().
    */
    if (rowNumber < getNumRows())
    {
        if (songEntriesToShow.size() > 0)
        {
            if (columnId == 1)
            {   
                //display song title
                g.drawText(songEntriesToShow[rowNumber].getFileNameWithoutExtension(),
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
            }

            if (columnId == 2)
            {
                //display song title
                g.drawText(songEntriesToShow[rowNumber].getFileExtension(),
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
            }
            else if (columnId == 3)
            {
                //display song duration in min and seconds
                double songDuration = getSongDuration(songEntriesToShow[rowNumber]);
                int minutes = (int)songDuration / 60;
                int seconds = (int)songDuration % 60;
                String songDurationShow = std::to_string(minutes) + " min " + std::to_string(seconds) + " s";

                g.drawText(songDurationShow,
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);

            }
        }


    }
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 4)
    {   
        //display load button to deck 1
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Deck 1" };

            // assign button name with setName()
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);   
            btn->setName("loadDeck1");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    else if (columnId == 5)
    {   
        //display load button to deck 2
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"Deck 2"};

            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->setName("loadDeck2");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    else if (columnId == 6)
    {   

        if (existingComponentToUpdate == nullptr)
        {
            //display unload button 
            TextButton* btn = new TextButton{ "Unload" };

            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->setName("unloadButton");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        
    }

    return existingComponentToUpdate;
};

//playlist component buttons functionalities from listener
void PlaylistComponent::buttonClicked(Button* button)
{   
    //to check addSongButton clicked
    if (button == &addSongButton)
    {   

       
        //check if button is clicked
        DBG("add song button");

        // choose a file
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
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
                }

                // add file to songEntries
                songEntries.push_back(chosenFile);
                songEntriesToShow.push_back(chosenFile);

                // call getSongDuration to get song duration 
                double songDuration = getSongDuration(chosenFile);

                for (int i = 0; i < songEntries.size(); ++i)
                {
                    DBG(i + 1 << " Song title: " << songEntries[i].getFileName());
                    DBG("  Song Duration: " << getSongDuration(songEntries[i]));

                }

                tableComponent.resized();
            });
    }

    else // handle button with name (loadDeck1,loadDeck2,unloadButton)
    {
        //initialize button id 0,1,2,3,4 in columns and button names
        int btnID = std::stoi(button->getComponentID().toStdString());
        String btnName = button->getName().toStdString();

        if (btnName == "loadDeck1") 
        {
            // load the song and begin to build waveform to deckgui
            deckOne->buildPlayer()->loadURL(URL{ songEntries[btnID] });
            deckOne->buildWaveformDisplay()->loadURL(URL{ songEntries[btnID] });

            //check load of song entry located at index "btnID" of array "songEntries"
            DBG("LoadDeck1: " << songEntries[btnID].getFileName());
        }
        else if (btnName == "loadDeck2") 
        {
            deckTwo->buildPlayer()->loadURL(URL{ songEntries[btnID] });
            deckTwo->buildWaveformDisplay()->loadURL(URL{ songEntries[btnID] });
            DBG("LoadDeck2: " << songEntries[btnID].getFileName());
        }
        else if (btnName == "unloadButton")
        {
            /* 
               1. create a vector "songEntriesRemain" to store remaining song after unload
               2. assign the new vector to songEntries
               3. clear existing entries then only push back the remained songs
            */
            std::vector<juce::File> songEntriesRemain;
            songEntriesRemain = songEntries;
            songEntries.clear();

            // song index which not unloaded are pushed back to songEntries
            for (int i = 0; i < songEntriesRemain.size(); ++i)
            {
                if (i != btnID)
                {
                    songEntries.push_back(songEntriesRemain[i]);
                }
            }
            DBG("Remained Songs: " << songEntriesRemain.size());

            //assign the final results to show into Vector "songEntriesToShow"
            songEntriesToShow.clear();
            songEntriesToShow = songEntries;

            tableComponent.resized();

            return;
        }
        else
        {
            DBG("Unknown button name");
        }
    }
};



void PlaylistComponent::saveFile(std::vector<juce::File> songVectorInput, std::string fileName)
{
    // create and open file stream to save file
    std::ofstream fileLocate;
    fileLocate.open(fileName);

    // saving filenames
    for (int i = 0; i < songVectorInput.size(); ++i)
    {
        fileLocate << songVectorInput[i].getFullPathName()<< std::endl;
       
    }

    // close file after save
    fileLocate.close();
};

void PlaylistComponent::loadSavedFile(std::string fileName)
{
    // create input file stream to open previosuly saved file
    std::ifstream fileLocate;
    fileLocate.open(fileName);

    //token to store tokenized filename
    std::vector<std::string> token;
    std::string line;
  
    // tokenise filename by line and new space
    while (getline(fileLocate, line))
    {
        token = tokenise(line, '\n');
        DBG(line);
        
        File tokenizedFile(token[0]);

        // push files into playlist
        songEntries.push_back(tokenizedFile);
        songEntriesToShow.push_back(tokenizedFile);
    };

    fileLocate.close();
};


//CSV tokenize function from mid-term coursework
std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{   
    DBG("interested drag");
    return true;
};

//to implement drag and drop files
void PlaylistComponent::filesDropped(const StringArray& filenames, int x, int y)
{
    for (const auto& file : filenames)
    {
        auto filePath = File(file);
        if (filePath.hasFileExtension(".mp3") || filePath.hasFileExtension(".wav"))
        {
            DBG("file is dropped");
            // Add the dropped file to the songEntries list
            songEntries.push_back(filePath);
            songEntriesToShow.push_back(filePath);

            // get song duration for songs
            double songDuration = getSongDuration(filePath);
         
        }

    }
    // Refresh the table component
    tableComponent.resized();
}


