// main.cpp
// CLI version of two gambits game

/*
	Each event has an option chain,
		- Each option is a security and it should have its own ExchangeEngine
		- Two type of options, different in terms of end game termination process

	Each user has a position, order pending list => same type 
*/

#include <map>
#include <queue>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <memory>    
#include <cstdio>

#define LOOP for(;;)

// MACRO ===================================================================================================================================
template <typename T>
using ORDER_BOOK = std::map<double, std::queue<T>>; // price => queue {T}

// basic object classes ====================================================================================================================

// trade engine -------------------------------------------------------------------------------
template <class T>
class ExchangeEngine{  // exchange engine for class T
	/*
		The order can be matched by the same amount of buy and sell of the same security type 
		or same amount of yes and no whose limit price adds up to 1
		=> 
		ExchangeEngine should be 1 per security, contain 4 orderBooks
	*/
	ORDER_BOOK<T> orderBookYesSell, orderBookYesBuy;
	ORDER_BOOK<T> orderBookNoSell, orderBookNoBuy;
public:
	ExchangeEngine(){}
	~ExchangeEngine() = default;
};


// Option object -- abstract
class Option{
	// represents a time vertical, contains 2 
	std::string oid;
	double price;
	std::string expiry;
public:
	Option(std::string oid, double price, std::string expiry) : oid(oid), price(price), expiry(expiry) {}
	virtual ~Option() = default; 

	virtual Option& finalizePrice() = 0;
};

// NoOption class - subclass of Option ----------------------------------------
class NoOption : public Option{

public:
	NoOption(std::string id, double p, std::string exp) : Option(id, p, exp) {} 
	~NoOption() override = default;

	Option& finalizePrice() override {
		return *this;
	}

};

// YesOption class - subclass of Option --------------------------------------
class YesOption : public Option{

public:
	YesOption(std::string id, double p, std::string exp) : Option(id, p, exp) {}
	~YesOption() override = default;

	Option& finalizePrice() override {
		return *this;
	}
};

#define OPTION_CHAIN std::map<std::string, std::shared_ptr<Option>> // time => Option

// timeVertical class --------------------------------------------------------
class TimeVertical{
	ExchangeEngine<std::shared_ptr<Option>> exchangeEngine; // each time frame has its own order book
	std::string time; // dublicated but whatever
	OPTION_CHAIN option_chain;
public:
	TimeVertical(){}
	~TimeVertical() = default;
};

// Position class ---------------------------------------------------------------
class Position{
	std::shared_ptr<Option> security;
	double entry_price;
	int amount;
public:
	Position(){}
	~Position() = default;
};

#define POSITIONS std::unordered_map<std::string, std::unordered_map<std::string, Position>> // eid => oid => Position

// Order class ----------------------------------------------------------------------
class Order{
	std::shared_ptr<Option> security;
	double limit;
	int amount;
public:
	Order(){}
	~Order() = default;
};

// Event object ---------------------------------------------------------------------
class Event{ 
	/*
		Each event has an option chain, consist of multiple TimeVertical
	*/
	int eid;
	std::string description;
	std::map<std::string, TimeVertical> timeVerticals;
	bool ifEnd = false;
public:
	Event(int eid, std::string description) : eid(eid), description(description){}
	~Event() = default;

	int getEid() const { return eid;}
	std::string getDescription() const { return description;}
	bool getIfEnd() const {return ifEnd;}

	Event& endEventProcedure(){
		return *this;
	}

	// Event& addOption(const std::string &date, Option option){
	// 	option_chain[date] = std::move(option);
	// 	return *this;
	// }

};

// user class ==============================================================================================================================

// store users credit left and position
class User{
	double balance = 100000.0;
	POSITIONS spositions; 
public:
	User(){}
	~User() = default;
};

// game engine class =======================================================================================================================






// game engine ----------------------------------------------------------------------------
class Game{
	std::vector<Event> events;
public:
	Game(const std::vector<Event>& evts) : events(evts) {}
	~Game() = default;

	void viewAllEvent() const {
		std::cout << 
		"========================================================================\n" <<
		"|| Event ID       | Event Description                   | If Ended    ||\n";

		for (const auto& event : events) {
	        printf("|| %-14s | %-35s | %-11s ||\n",
	               std::to_string(event.getEid()).c_str(),
	               event.getDescription().c_str(),
	               std::to_string(event.getIfEnd()).c_str());
    	}
		std::cout << 
		"========================================================================" << std::endl;
	}	
};

// main ===========================================================================================================================

// command enum --------------------------------------------------------------------------------
enum Command {VIEW_ALL, VIEW, BUY, SELL, UNKNOWN};
Command toCommand(const std::string &com){
	if (com == "viewall") return VIEW_ALL;
    if (com == "view") return VIEW;
    if (com == "buy") return BUY;
    if (com == "sell") return SELL;
    return UNKNOWN;
}

// main function --------------------------------------------------------------------------------
int main(){
	// create a few events
	std::vector<Event> events = {Event(1, "When will K get a gf"), Event(2, "When will K like IgumDrop Post")};

	// init games
	Game game(events);

	std::cout 
		    << "=========================================\n"
		    << "||      Two Gambits - CLI Started      ||\n"
		    << "||        Prototype Build - 2025       ||\n"
		    << "=========================================\n\n";

	LOOP{
		// get user info
		std::string command;
		std::cout << "> ";
		std::cin >> command;

		switch(toCommand(command)){
			case VIEW_ALL:
				game.viewAllEvent();
				break;
			case VIEW:
				std::cout << "View >";
				break;
			case BUY:
				std::cout << "Buy >";
				break;
			case SELL:
				std::cout << "Sell >";
				break;
			case UNKNOWN:
				std::cout << "Error: UNKNOWN token\n";
				break;
		}

	}
	return 0;
}