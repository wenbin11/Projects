/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Feb 2022 10:31:47am
    Author:  wngbi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
 
//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _metaData,
                                     DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2)
                                     : metaData(_metaData),
                                       deckGUI1(_deckGUI1),
                                       deckGUI2(_deckGUI2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // set column name and width of tableListBox
    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Song Length", 2, 200);
    tableComponent.setColour(TableListBox::backgroundColourId, Colours::black);
    tableComponent.setModel(this);

    // display components 
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addFile);
    addFile.setColour(TextButton::buttonColourId, Colours::orange);
    addFile.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(search);
    search.setColour(TextButton::buttonColourId, Colours::orange);
    search.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(randomise);
    randomise.setColour(TextButton::buttonColourId, Colours::orange);
    randomise.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(deck1);
    deck1.setColour(TextButton::buttonColourId, Colours::orange);
    deck1.setColour(TextButton::textColourOffId, Colours::black);
    addAndMakeVisible(deck2);
    deck2.setColour(TextButton::buttonColourId, Colours::orange);
    deck2.setColour(TextButton::textColourOffId, Colours::black);

    // add listeners to the text buttons 
    addFile.addListener(this);
    search.addListener(this);
    randomise.addListener(this);
    deck1.addListener(this);
    deck2.addListener(this);

    // configure the search box and displaying it 
    searchBox.setColour(TextEditor::backgroundColourId, Colours::floralwhite);
    searchBox.setColour(TextEditor::textColourId, Colours::black);
    searchBox.setText("Search songs here.");
    addAndMakeVisible(searchBox);

    // load saved audio in playlist 
    loadSavedFile();
    
}

PlaylistComponent::~PlaylistComponent()
{
    // save the audio in playlist 
    saveAudioToFile();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::black);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    // setting the position of the components
    deck1.setBounds(0, 0, 200, 40);
    deck2.setBounds(200, 0, 200, 40);

    randomise.setBounds(0, 40, 200, 40);
    addFile.setBounds(200, 40, 200, 40);

    searchBox.setBounds(0, 80, 150, 30);
    search.setBounds(150, 80, 100, 30);

    tableComponent.setBounds(0, 110, getWidth(), getHeight() - 110);
}

int PlaylistComponent::getNumRows()
{
    // return number of rows 
    return audioName.size();
}
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::grey);
    }
}
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1)
    {
        // draw the audio fileName 
        g.drawText(audioName[rowNumber], 2, 1, width - 4, height, Justification::centredLeft, true);
    }
    if (columnId == 2)
    {
        // draw the audio file length 
        g.drawText(audioLength[rowNumber], 2, 2, width - 2, height, Justification::centredLeft, true);

    }
}   

Component* PlaylistComponent::refreshComponentForCell(int 	rowNumber,
    int 	columnId,
    bool 	isRowSelected,
    Component* existingComponentToUpdate)
{
    return existingComponentToUpdate;
}
void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &addFile)
    {
        // allow user to select files to display in playlist 
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForMultipleFilesToOpen())
        {
            for (const juce::File& file : chooser.getResults())
            {
                // get the filename
                juce::String fileName{ file.getFileNameWithoutExtension() };
                if (checkFileExist(fileName) == false)
                {   
                    // if file is not in playlist 
                    juce::URL audioURL{ file };
                    audioName.push_back(fileName);
                    audioLength.push_back(getAudioLength(audioURL));
                    musicURL.push_back(audioURL);
                    audioFullPathName.push_back(file.getFullPathName());
                }
                else
                {
                    // display error: file is already in playlist
                    juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "Add Audio", fileName + " already exists", "OK", nullptr);
                }
            }
        }
        // refresh the playlist 
        tableComponent.updateContent();
    }
    else if (button == &search)
    {
        // get user input in the searchBox 
        String searchValue = searchBox.getText();
        for (int i = 0; i < audioName.size(); i++)
        {
            // check if any filename matches with the user input 
            if (audioName[i].contains(searchValue))
            {
                // select the file name if it matches 
                tableComponent.selectRow(i, false, true);
            }
        }
    }
    else if (button == &deck1)
    {
        // load file to deck 1
        loadToDeck(deckGUI1);
    }
    else if (button == &deck2)
    {
        // load file to deck 2
        loadToDeck(deckGUI2);
    }
    else if (button == &randomise)
    {
        // load random files to deck 1 and 2
        DBG("DECK 1:");
        loadRandomFiles(deckGUI1);
        DBG("DECK 2:");
        loadRandomFiles(deckGUI2);
    }
}
bool PlaylistComponent::checkFileExist(juce::String fileName)
{
    // check against audio file name, return true if it matches 
    if (std::find(audioName.begin(), audioName.end(), fileName) != audioName.end())
    {
        return true;
    }
    else return false;
}
juce::String PlaylistComponent::getAudioLength(juce::URL audioURL)
{
    // use the audioURL to get the song length 
    metaData->loadURL(audioURL);
    double seconds{ metaData->getLengthInSeconds() };
    juce::String songLength{ convertAudioTime(seconds) };
    return songLength;
}
juce::String PlaylistComponent::convertAudioTime(double seconds)
{
    // get the minutes and seconds 
    int doubleToInt{ int(std::round(seconds)) };
    juce::String min{ std::to_string(doubleToInt / 60) };
    juce::String sec{ std::to_string(doubleToInt % 60) };

    if (sec.length() < 2) // if seconds is less than 10
    {
        sec = sec.paddedLeft('0', 2); // add a 0 in front e.g. "05" for 5 seconds 
    }
    return juce::String{ min + ":" + sec };
}
void PlaylistComponent::loadToDeck(DeckGUI* deckGUI)
{
    // select the row and load the file to deck with the audioURL index 
    int rowNumber{ tableComponent.getSelectedRow() };
    deckGUI->loadToDeck(musicURL[rowNumber]);
}
void PlaylistComponent::loadRandomFiles(DeckGUI* deckGUI)
{
    // random get an index between the playlist 
    int randomRow = (std::rand() % tableComponent.getNumRows()) + 0;
    DBG(audioName[randomRow] << " has been loaded");
    // load the random row to the deck 
    deckGUI->loadToDeck(musicURL[randomRow]);
}
void PlaylistComponent::saveAudioToFile()
{
    // create a file 
    std::ofstream myTracks("myTracks.csv");

    for (int i = 0; i < audioName.size(); i++)
    {
        // save the full path name and the audio length for all files in playlist 
        myTracks << audioFullPathName[i] << "," << audioLength[i] << "\n";
    }
}
void PlaylistComponent::loadSavedFile()
{
    // read the file 
    std::ifstream myTracks("myTracks.csv");
    std::string fileName;
    std::string fileLength;

    // open the file 
    if (myTracks.is_open())
    {
        while (std::getline(myTracks, fileName, ',')) // delimit with a ','
        {
            // save the audio name, audio length and the audio URL back to the corresponding vectors
            juce::File file{ fileName };
            juce::URL audioURL{ file };
            musicURL.push_back(audioURL);
            audioName.push_back(file.getFileNameWithoutExtension());

            std::getline(myTracks, fileLength);
            audioLength.push_back(fileLength);
        }
    }
    // close the file 
    myTracks.close();
}