/*
  ==============================================================================

    PlaylistComponent.h
    Created: 13 Feb 2022 10:31:47am
    Author:  wngbi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <vector>
#include <string>
#include <fstream>

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* _metaData,
                      DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2);
    ~PlaylistComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int 	rowNumber,
                                       int 	columnId,
                                       bool 	isRowSelected,
                                       Component* existingComponentToUpdate
                                       ) override;

    void buttonClicked(Button* button) override;

private:

    DJAudioPlayer* metaData;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    TableListBox tableComponent;
    TextButton addFile{ "Add File" };
    TextButton search{ "Search" };
    TextButton randomise{ "Shuffle Load" };
    TextButton deck1{ "Add To Deck 1" };
    TextButton deck2{ "Add To Deck 2" };
    TextEditor searchBox;
    
    // vector of audio file
    std::vector<juce::String> audioName;
    // vector of audio full file path
    std::vector<juce::String> audioFullPathName;
    // vector of audio length in minutes and seconds 
    std::vector<juce::String> audioLength;
    // vector of audio URL 
    std::vector<juce::URL> musicURL;

    // function to get audio length of the audio URL 
    juce::String getAudioLength(juce::URL audioURL);
    // function to convert seconds to minutes and seconds 
    juce::String convertAudioTime(double seconds);
    // check if file is in the playlist 
    bool checkFileExist(juce::String fileName);
    // load audio URL to the deck
    void loadToDeck(DeckGUI* deckGUI);
    // load random audio URL in the playlist to the deck
    void loadRandomFiles(DeckGUI* deckGUI);
    // save the playlist
    void saveAudioToFile();
    // load the saved playlist 
    void loadSavedFile();

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
