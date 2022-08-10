#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** construct, reading a csv data file */
        OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);
        /** returns the previous time after the 
         * sent time in the orderbook  
         * If there is no previous timestamp, wraps around to the last 
         * */
        std::string getPreviousTime(std::string timestamp);

        // function to find the highest price in the timestamp
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        // function to find the lowest price in the timestamp
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        
        //function to find the average price of the timestamp
        static double getAVGPrice(std::vector<OrderBookEntry>& orders);

    private:
        std::vector<OrderBookEntry> orders;


};
