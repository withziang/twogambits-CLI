// CLI version of two gambits game

#include <map>
#include <iostream>
#include <vector>
#include <format>

#define LOOP for(;;)
#define OPTION_CHAIN std::map<std::string, Option>


// trade engine
template <class T>
class ExchangeEngine{  // exchange engine for class T
private:
	std::map<double, std::pair<std::string, int>> order_book_YES, order_book_NO; // id + amount
public:

};


// Option object
class Option{
private:
	std::string oid;
	double price;
	std::string expiry;
	ExchangeEngine<Option> exchangeEngine; // each security has its own order book
public:
	Option(std::string oid, double price, std::string expiry) : oid(oid), price(price), expiry(expiry) {}
	~Option() = default;

};

// Event object
class Event{ 
private:
	int eid;
	std::string description;
	OPTION_CHAIN option_chain; // key:date, val: Option
	bool ifEnd = false;

public:
	Event(int eid, std::string description) : eid(eid), description(description){}
	~Event() = default;

	int getEid() const {
		return eid;
	}
	std::string getDescription() const {
		return description;
	}
	bool getIfEnd() const {
		return ifEnd;
	}

	Event& endEventProcedure(){
		return *this;
	}

	Event& addOption(const std::String &date, Option option){
		option_chain[date] = std::move(option);
		return *this;
	}

};




// store users credit left and position
class User{
private:
	double balance = 100000.0;
	std::map<std::string, std::map<std::string,int>> position; // eid => oid => amount
public:
	User(){}
	~User() = default;
};

// game engine
class Game{
private:
	std::vector<Event> events;
public:
	Game(std::vector<Event> events ) : events(events) {}
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


// command enum
enum Command {VIEW_ALL, VIEW, BUY, SELL, UNKNOWN};
Command toCommand(const std::string &com){
	if (com == "viewall") return VIEW_ALL;
    if (com == "view") return VIEW;
    if (com == "buy") return BUY;
    if (com == "sell") return SELL;
    return UNKNOWN;
}


int main(){
	// create a few events
	std::vector<Event> events = {Event(1, "When will Kevin get a gf"), Event(2, "When will Kevin like IgumDrop Post")};


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
		std::cout << "Enter command > ";
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