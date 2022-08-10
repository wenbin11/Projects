#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"




class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        // Menu Message
        void printMenu();
        // C1: function to list all available commands
        void printHelp();
        // C3: function to list all available products in the order book 
        void printProd();
        // C8 function to print the current time stamp in the dataset 
        void printTime();
        // TASK 2 (MY OWN COMMANDS): function to print the trend indicator of the product
        void printTrend(int trend);
        // C2: functions to print out all the help <cmd>
        void printHelpProd();
        void printHelpMin();
        void printHelpMax();
        void printHelpAvg();
        void printHelpPredict();
        void printHelpTime();
        void printHelpStep();
        void printHelpBack();
        void printHelpTrend();
        // C9: function to move the next time stamp in the order book 
        void gotoNextTimeframe();
        void gotoPreviousTimeframe();
        // TASK 2 (MY OWN COMMANDS): By using the Aroon Indicator, the function returns the number of timesteps since the most recent highest order price.
        int AroonUp(std::string product);
        // TASK 2 (MY OWN COMMANDS): By using the Aroon Indicator, the function returns the number of timesteps since the most recent lowest order price.
        int AroonDown(std::string product);
        // function to get the user input
        std::string getUserOption();
        // function to process and run the neccessary functions with the user input
        void processUserOption(std::string userOption);

        std::string currentTime;

//        OrderBook orderBook{"20200317.csv"};
	OrderBook orderBook{"20200317.csv"};

};
