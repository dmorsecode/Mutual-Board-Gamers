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
	std::cout << "(You may enter 0 to load every entry.)" << std::endl;
	int entries;
	std::cin >> entries;
	// check if entries is a valid number
	while (std::cin.fail() || entries < 0 || entries > 18964806) {
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Invalid input. Please enter a number between 0 and 18964806." << std::endl;
		std::cin >> entries;
	}
	if (entries == 0) {
		entries = 18964806;
	}
	std::cout << std::endl;

	std::cout << "Do you want to include the review text? [Y/N]" << std::endl;
	std::string includeText;
	std::cin >> includeText;
	// check if includeText is a valid input
	while (includeText != "Y" && includeText != "y" && includeText != "N" && includeText != "n") {
		std::cout << "Invalid input. Please enter Y or N." << std::endl;
		std::cin >> includeText;
	}
	std::cout << std::endl;

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
	std::cout << std::endl;

	// progress bar
	ProgressBar bar;
	bar.set_option(option::BarWidth{ 50 });
	bar.set_option(option::Start{ "[" });
	bar.set_option(option::Fill{ "=" });
	bar.set_option(option::Lead{ "=" });
	bar.set_option(option::Remainder{ " " });
	bar.set_option(option::End{ "]" });
	bar.set_option(option::ShowPercentage{ true });
	bar.set_option(option::ShowElapsedTime{ true });
	bar.set_option(option::ShowRemainingTime{ true });
	bar.set_option(option::ForegroundColor{ Color::white });
	bar.set_option(option::FontStyles{ std::vector<FontStyle>{FontStyle::bold} });
	bar.set_option(option::MaxProgress{ 100 });
	indicators::show_console_cursor(false);

	std::cout << "Creating list..." << std::endl << std::endl;

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
			if (includeText == "Y" || includeText == "y") {
				orderedUsernames[user].addReview(gameID, gameName, rating, comment);
			}
			else {
				orderedUsernames[user].addReview(gameID, gameName, rating);
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
	std::cout << std::endl;
	indicators::show_console_cursor(true);

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time Elapsed: " << duration.count() << " seconds" << std::endl;
	std::cout << std::endl;
	std::cout << "Would you like to sort reviews by game ID (1), game name (2), or rating (3)?" << std::endl;
	int sortType;
	std::cin >> sortType;
	// check if sortType is a valid input
	while (std::cin.fail() || sortType < 1 || sortType > 3) {
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Invalid input. Please enter 1, 2, or 3." << std::endl;
		std::cin >> sortType;
	}
	std::cout << std::endl;

	std::pair<std::string, int> mostReviews = { "", 0 };

	// New progress bar to track sorts
	ProgressBar bar2;
	bar2.set_option(option::BarWidth{ 50 });
	bar2.set_option(option::Start{ "[" });
	bar2.set_option(option::Fill{ "=" });
	bar2.set_option(option::Lead{ "=" });
	bar2.set_option(option::Remainder{ " " });
	bar2.set_option(option::End{ "]" });
	bar2.set_option(option::ShowPercentage{ true });
	bar2.set_option(option::ShowElapsedTime{ true });
	bar2.set_option(option::ShowRemainingTime{ true });
	bar2.set_option(option::ForegroundColor{ Color::white });
	bar2.set_option(option::FontStyles{ std::vector<FontStyle>{FontStyle::bold} });
	bar2.set_option(option::MaxProgress{ 100 });
	indicators::show_console_cursor(false);

	std::cout << "Sorting... ";
	start = std::chrono::high_resolution_clock::now();
	int userCount = 0;
	if (mapType == 1) {
			for (auto& user : unorderedUsernames) {
				user.second.quickSort(sortType);
				userCount++;
				if (user.second.getSize() > mostReviews.second) {
					mostReviews.first = user.first;
					mostReviews.second = user.second.getSize();
				}
				if (userCount % (unorderedUsernames.size() / 100) == 0) {
					// make sure bar isnt completed first
					if (bar2.current() != 100) {
						bar2.tick();
					}
				}
			}
	}
	else if (mapType == 2) {
		for (auto& user : orderedUsernames) {
			user.second.quickSort(sortType);
			userCount++;
			if (user.second.getSize() > mostReviews.second) {
				mostReviews.first = user.first;
				mostReviews.second = user.second.getSize();
			}
			if (userCount % (orderedUsernames.size() / 100) == 0) {
				// make sure bar isnt completed first
				if (bar2.current() != 100) {
					bar2.tick();
				}
			}
		}
	}
	else {
		std::cout << "Invalid map type." << std::endl;
		return 0;
	}
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "done. Sorted " << userCount << " usernames in " << duration.count() << " seconds" << std::endl;
	std::cout << std::endl;

	// Print the reviews of the user with the most reviews.
	std::cout << "User with the most reviews: " << mostReviews.first << std::endl;
	std::cout << "Number of reviews: " << mostReviews.second << std::endl;
	if (mapType == 1) {
		unorderedUsernames[mostReviews.first].printReviews();
	}
	else if (mapType == 2) {
		orderedUsernames[mostReviews.first].printReviews();
	}
	else {
		std::cout << "Invalid map type." << std::endl;
		return 0;
	}
	
	//// Print the reviews of a random user.
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<> dis(0, userCount - 1);
	//if (mapType == 1) {
	//	auto it = unorderedUsernames.begin();
	//	std::advance(it, dis(gen));
	//	std::cout << it->first << "'s reviews:" << std::endl;
	//	it->second.printReviews();
	//}
	//else if (mapType == 2) {
	//	auto it = orderedUsernames.begin();
	//	std::advance(it, dis(gen));
	//	std::cout << it->first << "'s reviews:" << std::endl;
	//	it->second.printReviews();
	//}
	//else {
	//	std::cout << "Invalid map type." << std::endl;
	//	return 0;
	//}

	return 0;
}