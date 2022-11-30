#include <chrono>
#include <iostream>
#include <cmath>
#include <random>
#include <iterator>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <thread>
#include "csv.hpp"
#include <indicators.hpp>

#include "mbg/ReviewList.h"
using namespace csv;
using namespace indicators;

int main() {
	
    // unordered map
	std::unordered_map<std::string, ReviewList> unorderedUsernames;

    // ordered map
	std::map<std::string, ReviewList> orderedUsernames;

	//CSVReader reader("user_ratings.csv");
	CSVReader reader("bgg-19m-reviews.csv");
    CSVRow row;
	

	std::cout << "How many entries would you like to load? [1 - 18964806]" << std::endl;
	int entries;
	std::cin >> entries;
	// check if entries is a valid number
	while (std::cin.fail() || entries < 1 || entries > 18964806) {
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Invalid input. Please enter a number between 1 and 18964806." << std::endl;
		std::cin >> entries;
	}

	std::cout << "Do you want to include the review text? [Y/N]" << std::endl;
	std::string includeText;
	std::cin >> includeText;
	// check if includeText is a valid input
	while (includeText != "Y" && includeText != "y" && includeText != "N" && includeText != "n") {
		std::cout << "Invalid input. Please enter Y or N." << std::endl;
		std::cin >> includeText;
	}

	// ask which map type the user wants to use
	std::cout << "Do you want to use an unordered map (1) or an ordered map (2)?" << std::endl;
	int mapType;
	std::cin >> mapType;
	// check if mapType is a valid input
	while (std::cin.fail() || mapType < 1 || mapType > 2) {
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Invalid input. Please enter 1 or 2." << std::endl;
		std::cin >> mapType;
	}

	// progress bar
	ProgressBar bar;
	bar.set_option(option::BarWidth{ 50 });
	bar.set_option(option::Start{ "[" });
	bar.set_option(option::Fill{ "=" });
	bar.set_option(option::Lead{ "=" });
	bar.set_option(option::Remainder{ " " });
	bar.set_option(option::End{ " ]" });
	bar.set_option(option::ShowPercentage{ true });
	bar.set_option(option::ShowElapsedTime{ true });
	bar.set_option(option::ShowRemainingTime{ true });
	bar.set_option(option::ForegroundColor{ Color::white });
	bar.set_option(option::FontStyles{ std::vector<FontStyle>{FontStyle::bold} });
	bar.set_option(option::MaxProgress{ 100 });
	indicators::show_console_cursor(false);

	std::cout << "Creating list..." << std::endl;

	int i = 0;
	// start chrono timer to track how long it takes to create the map
	auto start = std::chrono::high_resolution_clock::now();
	for (auto& row : reader) {
		std::string user = row["user"].get<>();
		int gameID = row["ID"].get<int>();
		std::string gameName = row["name"].get<>();
		float rating = row["rating"].get<float>();
		std::string comment = row["comment"].get<>();

		if (mapType == 1) {
			if (includeText == "Y" || includeText == "y") {
				unorderedUsernames[user].addReview(gameID, gameName, rating, comment);
			}
			else {
				unorderedUsernames[user].addReview(gameID, gameName, rating);
			}
		}
		else if (mapType == 2) {
			// try-catch block in case the username doesn't exist in the map yet
			try {
				if (includeText == "Y" || includeText == "y") {
					orderedUsernames.at(user).addReview(gameID, gameName, rating, comment);
				}
				else {
					orderedUsernames.at(user).addReview(gameID, gameName, rating);
				}
			}
			catch (const std::out_of_range& oor) {
				ReviewList newList = ReviewList();
				if (includeText == "Y" || includeText == "y") {
					newList.addReview(gameID, gameName, rating, comment);
				}
				else {
					newList.addReview(gameID, gameName, rating);
				}
				orderedUsernames.insert({ user, newList });
			}
		}
		else {
			std::cout << "Invalid map type." << std::endl;
			return 0;
		}
		i++;
		if (i % (entries / 100) == 0) {
			// make sure bar isnt completed first
			if (bar.current() != 100) {
				bar.tick();
			}
		}
		// break after i = entries
		if (i >= entries) break;
	}
	indicators::show_console_cursor(true);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time Elapsed: " << duration.count() << " seconds" << std::endl;

	// get random element from our unordered map
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, unorderedUsernames.size() - 1);
	auto randomElement = std::next(unorderedUsernames.begin(), dis(gen));
	// print reviews
	std::cout << "Reviews for " << randomElement->first << std::endl;
	randomElement->second.printReviews();

	return 0;
}