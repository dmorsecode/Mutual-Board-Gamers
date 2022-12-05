﻿#include <chrono>
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
#include "mbg/UIManager.h"

using namespace csv;
using namespace indicators;

void sortReviews(int& sortType, int mapType, std::unordered_map<std::string, ReviewList>& unorderedUsernames, std::map<std::string, ReviewList>& orderedUsernames) {
	sortType = UIManager::getIntInput("Would you like to sort reviews by game ID (1), game name (2), or rating (3)?", 1, 3);
	
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

	
	auto start = std::chrono::high_resolution_clock::now();
	int userCount = 0;
	if (mapType == 1) {
		for (auto& user : unorderedUsernames) {
			user.second.setSortType(sortType);
			user.second.quickSort();
			userCount++;
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
			user.second.setSortType(sortType);
			user.second.quickSort();
			userCount++;
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
		return;
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Done. Sorted " << userCount << " usernames in " << duration.count() << " seconds" << std::endl;
	std::cout << std::endl;
}

int main() {

	// unordered map
	std::unordered_map<std::string, ReviewList> unorderedUsernames;

    // ordered map
	std::map<std::string, ReviewList> orderedUsernames;

	int sortType;

	//CSVReader reader("user_ratings.csv");
	CSVReader reader("bgg-19m-reviews.csv");
    CSVRow row;
	

	// get number of records to load
	int entries = UIManager::getIntInput("How many entries would you like to load?", 100, 18964806, 0, 18964806);
	
	// include review text or not
	bool includeText = UIManager::getBoolInput("Do you want to include the review text?");
	
	// ask which map type the user wants to use
	int mapType = UIManager::getIntInput("Do you want to use an unordered map (1) or an ordered map (2)?", 1, 2);



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
			if (includeText) {
				unorderedUsernames[user].addReview(gameID, gameName, rating, comment);
				unorderedUsernames[user].setUsername(user);
			}
			else {
				unorderedUsernames[user].addReview(gameID, gameName, rating);
				unorderedUsernames[user].setUsername(user);
			}
			
		}
		else if (mapType == 2) {
			
			if (includeText) {
				orderedUsernames[user].addReview(gameID, gameName, rating, comment);
				orderedUsernames[user].setUsername(user);
			}
			else {
				orderedUsernames[user].addReview(gameID, gameName, rating);
				orderedUsernames[user].setUsername(user);
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

	sortReviews(sortType, mapType, unorderedUsernames, orderedUsernames);
		
	bool exit = false;
	bool changeMenu = false;

	std::string user1, user2;

	while (exit == false) {

		// main menu (no selected user)
		if (user1 == "") { 
			int uiChoice = UIManager::displayMainMenu();
			
			switch (uiChoice) {
			case 1: // find user
				user1 = UIManager::getStringInput("Enter a username:");
				if (mapType == 1) {
					if (unorderedUsernames.find(user1) == unorderedUsernames.end()) {
						std::cout << "User not found" << std::endl;
						user1 = "";
					}
				}
				else if(mapType == 2) {
					if (orderedUsernames.find(user1) == orderedUsernames.end()) {
						std::cout << "User not found" << std::endl;
						user1 = "";
					}
				}
				break;
			case 2: // view random user
			{
				// choose random user from the correct map
				std::random_device rd;
				std::mt19937 gen(rd());
				if (mapType == 1) {
					std::uniform_int_distribution<> dis(0, unorderedUsernames.size() - 1);
					int randomIndex = dis(gen);
					auto it = unorderedUsernames.begin();
					std::advance(it, randomIndex);
					user1 = it->first;
				}
				else if (mapType == 2) {
					std::uniform_int_distribution<> dis(0, orderedUsernames.size() - 1);
					int randomIndex = dis(gen);
					auto it = orderedUsernames.begin();
					std::advance(it, randomIndex);
					user1 = it->first;
				}
				else {
					std::cout << "Invalid map type." << std::endl;
					return 0;
				}
				break;
			}
			case 3: // change sort method
				sortReviews(sortType, mapType, unorderedUsernames, orderedUsernames);
				break;
			case 4: // exit
				exit = true;
				break;
			}
		}
		else // user is selected
		{
			int uiChoice = UIManager::displayUserMenu(user1);

			switch (uiChoice) {
			case 1: // display reviews by name
				
				if (mapType == 1)
					UIManager::printUserRatings(unorderedUsernames[user1], 2);
				else if (mapType == 2) 
					UIManager::printUserRatings(orderedUsernames[user1], 2);
				break;

			case 2: // compare with specific user
			{
				while (user2 == "") {
					user2 = UIManager::getStringInput("Enter a username:");
					if (mapType == 1) {
						if (unorderedUsernames.find(user2) == unorderedUsernames.end()) {
							std::cout << "User not found" << std::endl;
							user2 = "";
						}
					}
					else if (mapType == 2) {
						if (orderedUsernames.find(user2) == orderedUsernames.end()) {
							std::cout << "User not found" << std::endl;
							user2 = "";
						}
					}
				}
				std::pair<std::string, std::string> users(user1, user2);

				// calculate and display comparison
				if (mapType == 1) {
					std::pair<ReviewList, ReviewList> gamesInCommon = unorderedUsernames[user1].getIntersection(unorderedUsernames[user2]);
					UIManager::printRatingComparison(gamesInCommon.first, gamesInCommon.second, users);
				}
				else if (mapType == 2) {
					std::pair<ReviewList, ReviewList> gamesInCommon = orderedUsernames[user1].getIntersection(orderedUsernames[user2]);
					UIManager::printRatingComparison(gamesInCommon.first, gamesInCommon.second, users);

				}

				//reset user2
				user2 = "";
			}
				break;

			case 3: // compare with random user
			{
				// start a timer to make sure we don't take too long
				bool unfound = false; // Track whether or not we gave up due to not finding anyone with comparable reviews.
				auto start = std::chrono::high_resolution_clock::now();
				std::pair<ReviewList, ReviewList> gamesInCommon;
				std::random_device rd;
				std::mt19937 gen(rd());
				while (user2 == "") {
					// check we haven't taken longer than 5 seconds
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = end - start;
					if (elapsed.count() > 5) {
						std::cout << "Could not find a user to compare with." << std::endl;
						unfound = true;
						break;
					}
					if (mapType == 1) {
						std::uniform_int_distribution<> dis(0, unorderedUsernames.size() - 1);
						int randomIndex = dis(gen);
						auto it = unorderedUsernames.begin();
						std::advance(it, randomIndex);
						user2 = it->first;
					}
					else if (mapType == 2) {
						std::uniform_int_distribution<> dis(0, orderedUsernames.size() - 1);
						int randomIndex = dis(gen);
						auto it = orderedUsernames.begin();
						std::advance(it, randomIndex);
						user2 = it->first;
					}
					gamesInCommon = unorderedUsernames[user1].getIntersection(unorderedUsernames[user2]);
					// check if they have none in common. if so, we need to reroll
					if (gamesInCommon.first.getSize() == 0 || gamesInCommon.second.getSize() == 0) {
						user2 = "";
					}
				}
				if (!unfound) {
					std::pair<std::string, std::string> users(user1, user2);
					// display comparison
					if (mapType == 1) {
						UIManager::printRatingComparison(gamesInCommon.first, gamesInCommon.second, users);
					}
					else if (mapType == 2) {
						UIManager::printRatingComparison(gamesInCommon.first, gamesInCommon.second, users);

					}
				}
				//reset user2
				user2 = "";
			}
				break;
			
			case 4: // go back
				user1 = "";
				break;
			}
		}
				
	}

	return 0;
}