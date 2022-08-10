#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"



MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    std::string input;
    currentTime = orderBook.getEarliestTime();


    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    //Menu Message 
    std::cout << "Please enter a command, or type help for a list of commands" << std::endl;

}

void MerkelMain::printHelp()
{
    // C1: printing all the available commands
    std::cout << "Here are the available commands: \n help \n help <cmd> \n prod \n min \n max \n avg \n predict\n time \n step \n back \n trend" << std::endl;
}

void MerkelMain::printProd()
{   
    // C3: to print all the products in the order book.
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Products Available: " << p << std::endl;
    }
    
}

void MerkelMain::printTime()
{
    // C8: print the current timestamp in the order book.
    std::cout << currentTime << std::endl;
}

void MerkelMain::printTrend(int trend)
{
    // TASK 2 (MY OWN COMMAND): function to tell the user about the trend indicator. 
    if (trend >= -100 && trend <= -50)
    {
        std::cout << "The product is undergoing a strong down trend." << std::endl;
    }
    else if (trend >= -49 && trend < 0)
    {
        std::cout << "The product is undergoing a weak down trend." << std::endl;
    }
    else if (trend >= 1 && trend < 50)
    {
        std::cout << "The product is undergoing a weak up trend." << std::endl;
    }
    else if (trend >= 50 && trend <= 100)
    {
        std::cout << "The product is undergoing a strong up trend." << std::endl;
    }
    else std::cout << "The product is not undergoing any trend." << std::endl;
}


void MerkelMain::printHelpProd()
{
    // C2: to print help prod command function.
    std::cout << "command: prod" << std::endl;
    std::cout << "List available products in the data" << std::endl;
}

void MerkelMain::printHelpMin()
{
    // C2: to print help min command function.
    std::cout << "command: min <product> bid/ask" << std::endl;
    std::cout << "E.g. min ETH/BTC ask -> the minimum ask for the product ETH/BTC" << std::endl;
}

void MerkelMain::printHelpMax()
{
    // C2: to print help max command function. 
    std::cout << "command: max <product> bid/ask" << std::endl;
    std::cout << "E.g. max DOGE/BTC bid -> the maximum bid for the product DOGE/BTC" << std::endl;
}

void MerkelMain::printHelpAvg()
{
    // C2: to print help avg command function.
    std::cout << "command: avg <product> bid/ask <timesteps>" << std::endl;
    std::cout << "E.g. avg ETH/BTC ask 5 -> average ETH/BTC ask over last 5 time steps" << std::endl;
}

void MerkelMain::printHelpPredict()
{
    // C2: to print the help predict command function.
    std::cout << "command: predict max/min <product> ask/bid " << std::endl;
    std::cout << "Predict max or min ask or bid for the sent product for the next time step" << std::endl;
}

void MerkelMain::printHelpTime()
{
    // C2: to print the help time command function. 
    std::cout << "command: time" << std::endl;
    std::cout << "State the current timeframe you are looking at in the dataset" << std::endl;
}

void MerkelMain::printHelpStep()
{
    // C2: to print the help step command function
    std::cout << "command: step" << std::endl;
    std::cout << "Move on to the next time step" << std::endl;
}

void MerkelMain::printHelpBack()
{
    // C2: to print the help step command function
    std::cout << "command: back" << std::endl;
    std::cout << "Move back to the previous time step" << std::endl;
}

void MerkelMain::printHelpTrend()
{
    // C2: to print the help trend command function.
    std::cout << "command: trend <product>" << std::endl;
    std::cout << "It will tell you how strong is the trend indicator." << std::endl;
}

int MerkelMain::AroonUp(std::string product)
{
    // TASK 2 (MY OWN COMMANDS): This function will return the number of timestamps since the most recent highest price.
    double Highest;
    int k;

    for (int i = 1; i < 25; i++)
    {
        std::vector<OrderBookEntry> Askentries = orderBook.getOrders(OrderBookType::ask, product, currentTime);
        std::vector<OrderBookEntry> Bidentries = orderBook.getOrders(OrderBookType::bid, product, currentTime);
        double HighestAsk = OrderBook::getHighPrice(Askentries); // getting the highest price for the ask orders
        double HighestBid = OrderBook::getHighPrice(Bidentries); // getting the highest price for the bid orders
        if (HighestAsk > Highest) // if there is any ask orders that is higher than the current highest, Highest will be updated and we will save the current timestamp number.
        {
            Highest = HighestAsk;
            k = i;
        }
        if (HighestBid > Highest) // if there is any bid orders that is higher than the current highest, Highest will be updated and we will save the current timestamp number.
        {
            Highest = HighestBid;
            k = i;
        }
        currentTime = orderBook.getPreviousTime(currentTime);
       }
    return k;
}

int MerkelMain::AroonDown(std::string product)
{
    // TASK 2 (MY OWN COMMANDS): This function will return the number of timestamps since the most recent lowest price.
    double Lowest;
    int k;

    for (int i = 1; i < 25; i++) // loop through the next 25 timestamps
    {
        std::vector<OrderBookEntry> Askentries = orderBook.getOrders(OrderBookType::ask, product, currentTime);
        std::vector<OrderBookEntry> Bidentries = orderBook.getOrders(OrderBookType::bid, product, currentTime);
        double LowestAsk = OrderBook::getLowPrice(Askentries); // getting the lowest price for the ask orders
        double LowestBid = OrderBook::getLowPrice(Bidentries); // getting the lowest price for the bid orders
        if (LowestAsk < Lowest) // if there is any ask orders that is lower than the current lowest, Lowest will be updated and we will save the current timestamp number.
        {
            Lowest = LowestAsk;
            k = i;
        }
        if (LowestBid > Lowest) // if there is any bid orders that is lower than the current lowest, Lowest will be updated and we will save the current timestamp number.
        {
            Lowest = LowestBid;
            k = i;
        }
        currentTime = orderBook.getPreviousTime(currentTime); // go to next timestamp
        
    }
    return k;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "now at " << currentTime << std::endl;
}

void MerkelMain::gotoPreviousTimeframe()
{
    std::cout << "Going to previous Time Frame" << std::endl;
    currentTime = orderBook.getPreviousTime(currentTime);
    std::cout << "Current Time is: " << currentTime << std::endl;
}
 
std::string MerkelMain::getUserOption()
{
    // function to get the user commands
    std::string userOption;

    std::getline(std::cin, userOption);
    
    return userOption;
}

void MerkelMain::processUserOption(std::string userOption)
{
    // function to process and run the commands given by the user.

    // tokenising the user command with " " as the separator. 
    std::vector<std::string> tokens = CSVReader::tokenise(userOption, ' ');

    if (tokens.size() > 4 ) // check if there is more than 4 tokens, sends an error to the user
    {
        std::cout << "You have typed too many parameters, type help for all available commands"  << std::endl;
    }
    if (tokens.size() == 1){ // if there is only one token.
        try {
            if (tokens[0] == "help") // C1: list all available commands if help is typed
            {
                printHelp(); 
            }
            else if (tokens[0] == "prod") // C3: list all known products in the orderbook if prod is typed
            {
                printProd();
            }
            else if (tokens[0] == "time") // C8: display the current timestamp if time is typed
            {
                printTime();
            }
            else if (tokens[0] == "step") // C9: go to the next timestamp if step is typed
            {
                gotoNextTimeframe();
            }
            else if (tokens[0] == "back") // TASK 2 (MY OWN COMMANDS): go to the previous timestamp if step is typed
            {
                gotoPreviousTimeframe();
            }
            else
            {
                // sends an error to the user if none of the above commands have been typed.
                std::cout << "The input is wrong, type help for all available commands" << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            // check if the user has input the correct data type for each tokens.
            std::cout << "Bad Input:tokens[0]. Type help for all available commands" << std::endl;
            throw;
        }   
    }
    
    if (tokens.size() == 2) // if there are two tokens 
    {
        try { //C2: print out the help for the specified commands 
                if (tokens[0] == "help" && tokens[1] == "prod")
                {
                    printHelpProd();
                }
                else if (tokens[0] == "help" && tokens[1] == "min")
                {
                    printHelpMin();
                }
                else if (tokens[0] == "help" && tokens[1] == "max")
                {
                    printHelpMax();
                }
                else if (tokens[0] == "help" && tokens[1] == "avg")
                {
                    printHelpAvg();
                }
                else if (tokens[0] == "help" && tokens[1] == "predict")
                {
                    printHelpPredict();
                }
                else if (tokens[0] == "help" && tokens[1] == "time")
                {
                    printHelpTime();
                }
                else if (tokens[0] == "help" && tokens[1] == "step")
                {
                    printHelpStep();
                }
                 else if (tokens[0] == "help" && tokens[1] == "back")
                {
                    printHelpBack();
                }
                else if (tokens[0] == "help" && tokens[1] == "trend")
                {
                    printHelpTrend();
                } 
                else if (tokens[0] == "trend") // TASK 2 (MY OWN COMMANDS): check the trend of the product.
                {
                    std::string time = currentTime;
                    for (std::string const& p : orderBook.getKnownProducts())
                    {
                        if (tokens[1] == p)
                        {
                            int k = MerkelMain::AroonUp(p); // returns the number of timestamps since the most recent highest order price.
                            int m = MerkelMain::AroonDown(p); // returns the number of timestamps since the most recent lowest order price.
                            double AroonUp = 100 * (25 - k) / 25; // using the Aroon Indicator Formula
                            double AroonDown = 100 * (25 - m) / 25; // Using the Aroon Indicator Formula
                            int trend = AroonUp - AroonDown; // Calculate the Aroon Oscillator
                            std::cout << "By using the Aroon Oscillator, the trend index is: " << trend << std::endl;
                            printTrend(trend); // print out the trend line for the specified product
                        }
                    }
                    currentTime = time;
                }
                else
                {
                    // sends an error to the user if none of the above commands have been typed.
                    std::cout << "The input is wrong, type help for all available commands" << std::endl;
                }

        }
        catch(const std::exception& e)
        {
            // check if the user has input the correct data type for each tokens.
            std::cout << "Bad Input:tokens[1]. Type help for all available commands" << std::endl;
            throw;
        }   
    
    }

    if (tokens.size() == 3) // if there are three tokens 
    {
        try
        {
            std::string time = currentTime;
            for (std::string const& p : orderBook.getKnownProducts())
            {
                // run if min <product> ask command has been typed.
                if (tokens[0] == "min" && tokens[1] == p && tokens[2] == "ask")
                {
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                    std::cout << "The min ask for " << p << " is " << OrderBook::getLowPrice(entries) << std::endl;
                    // C4: output the minimum ask price in the current timestamp
                }
                // run if min <product> bid command has been typed.
                else if (tokens[0] == "min" && tokens[1] == p && tokens[2] == "bid")
                {
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
                    std::cout << "The min bid for " << p << " is " << OrderBook::getLowPrice(entries) << std::endl;
                    // C4: output the minimum bid price in the current timestamp
                }
                else if (tokens[0] == "max" && tokens[1] == p && tokens[2] == "ask")
                {
                    // run if max <product> ask command has been typed.
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                    std::cout << "The max ask for " << p << " is " << OrderBook::getHighPrice(entries) << std::endl;
                    // C5: output the maximum ask price in the current timestamp.
                }
                else if (tokens[0] == "max" && tokens[1] == p && tokens[2] == "bid")
                {
                    // run if max <product> bid command has been typed.
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
                    std::cout << "The max bid for " << p << " is " << OrderBook::getHighPrice(entries) << std::endl;
                    // C5: output the maximum bid price in the current timestamp.
                }
            }
            time = currentTime;
        }
        catch(const std::exception& e)
            {
                // check if the user has input the correct data type for each tokens.
                std::cout << "Bad Input:tokens[2]. Type help for all available commands" << std::endl;
                throw;
            }   
    }

    if (tokens[0] == "avg") // check if the first token is avg
    {
        try
        {
            int timesteps; 
            std::string time = currentTime;
            double sum;
            timesteps = std::stoi(tokens[3]); // convert data type from std::string to int for the 3rd token

            for (std::string const& p : orderBook.getKnownProducts())
            {
                // run if avg <product> ask has been typed
                if (tokens[0] == "avg" && tokens[1] == p && tokens[2] == "ask")
                {
                    for (int i = 1; i < timesteps; i++) // loop through the number of timesteps given from the user 
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                        double avgPrice = OrderBook::getAVGPrice(entries); // get average price for the current timestamp
                        sum += avgPrice; // add up the average price 
                        currentTime = orderBook.getPreviousTime(currentTime);
                    }
                    double avg = sum / timesteps; // find the average price in the number of timesteps
                    std::cout << "The average price for " << p << " ask price over the last " << timesteps << " timesteps was " << avg << std::endl;
                    // C6: output the average ask price for the product in the number of time steps.
                }
                // run if avg <product> bid has been typed 
                else if (tokens[0] == "avg" && tokens[1] == p && tokens[2] == "bid")
                {
                    for (int i = 1; i < timesteps; i++) // loop through the number of timesteps given from the user
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
                        double avgPrice = OrderBook::getAVGPrice(entries); // get average price for the current timestamp
                        sum += avgPrice; // add up the average price
                        currentTime = orderBook.getPreviousTime(currentTime);
                    }
                    double avg = sum / timesteps; // find the average price in the number of timesteps
                    std::cout << "The average price for " << p << " bid price over the last " << timesteps << " timesteps was " << avg << std::endl;
                    // C6: output the average bid price for the product in the number of time steps.
                }
            
            }
            currentTime = time;
        }
        catch(const std::exception& e)
            {
                // check if the user has input the correct data type for each tokens.
                std::cout << "Bad Input:tokens[3]. Type help for all available commands" << std::endl;
                throw;
            }   
        
    }
    if (tokens[0] == "predict") // check if the first token is predict 
    {
        try
        {
            std::string time = currentTime;
            double maxPrice, minPrice, movingAverage;
            for (std::string const& p : orderBook.getKnownProducts())
            {
                // The algorithm used here is simple moving average

                // check if predict max <product> bid has been typed 
                if (tokens[0] == "predict" && tokens[1] == "max" && tokens[2] == p && tokens[3] == "bid")
                {
                    for (int i = 1; i < 20; i++) // loop through the next 20 timestamps
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
                        double highest = OrderBook::getHighPrice(entries); // get the highest order bid price for the current timestamp
                        maxPrice += highest; // add up the highest prices 
                        currentTime = orderBook.getPreviousTime(currentTime);
                    }
                    double movingAverage = maxPrice / 20; // finding the 20 Simple Moving Average
                    std::cout << "By using the Simple Moving Average formula, the average max bid price for the past 20 timeframes is:" << movingAverage << std::endl;
                    // C7: output the predict max bid price for the product for the next time step
                }
                // check if predict max <product> ask has been typed 
                else if (tokens[0] == "predict" && tokens[1] == "max" && tokens[2] == p && tokens[3] == "ask")
                {
                    for (int i = 1; i < 20; i++) // loop through the next 20 timesteps
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                        double highest = OrderBook::getHighPrice(entries); // finding the highest ask price in the current timestamp
                        maxPrice += highest; // adding up the highest prices
                        currentTime = orderBook.getPreviousTime(currentTime);

                    }
                    double movingAverage = maxPrice / 20; // finding the 20 Simple Moving Average 
                    std::cout << "By using the Simple Moving Average formula, the average max ask price for the past 20 timeframes is:" << movingAverage << std::endl;
                    // C7: output the predict max ask price for the product for the next time step
                }
                // check if predict min <product> bid has been typed 
                else if (tokens[0] == "predict" && tokens[1] == "min" && tokens[2] == p && tokens[3] == "bid")
                {
                    for (int i = 1; i < 20; i++) // loop through the next 20 time steps
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
                        double lowest = OrderBook::getLowPrice(entries); // finding the lowest bid price in the current timestamp
                        minPrice += lowest; // adding up the lowest prices 
                        currentTime = orderBook.getPreviousTime(currentTime);
                        
                    }
                    double movingAverage = minPrice / 20; // finding the 20 Simple Moving Average 
                    std::cout << "By using the Simple Moving Average formula, the average min bid price for the past 20 timeframes is:" << movingAverage << std::endl;
                    // C7: output the predict min bid price for the product for the next time step
                }
                // check if predict min <product> ask has been typed 
                else if (tokens[0] == "predict" && tokens[1] == "min" && tokens[2] == p && tokens[3] == "ask")
                {
                    for (int i = 1; i < 20; i++) // loop through the next 20 time steps
                    {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
                        double lowest = OrderBook::getLowPrice(entries);  // finding the lowest ask price in the current timestamp
                        minPrice += lowest; // adding up the lowest prices
                        currentTime = orderBook.getPreviousTime(currentTime);
                        
                    }
                    double movingAverage = minPrice / 20; // finding the 20 Simple Moving Average
                    std::cout << "By using the Simple Moving Average formula, the average min bid price for the past 20 timeframes is:" << movingAverage << std::endl;
                    // C7: output the predict min ask price for the product for the next time step
                }
            }
            currentTime = time;
        }
        catch(const std::exception& e)
        {
            // check if the user has input the correct data type for each tokens.
            std::cout << "Bad Input:tokens[3]. Type help for all available commands" << std::endl;
            throw;
        }
        
    }
}
 