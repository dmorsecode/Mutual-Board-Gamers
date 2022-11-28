#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <thread>
#include "csv.hpp"
#include <indicators.hpp>

#include "mbg/ReviewList.h"

using namespace std;
using namespace csv;
using namespace indicators;

int main() {
	ReviewList testList = ReviewList();
	cout << "Size: " << testList.getSize() << endl;
	cout << "Shuffling..." << endl;
	testList.shuffle();
	cout << "Sorting reviews..." << endl;
	testList.countingSort();
	testList.printSortTime();
	testList.shuffle();
	testList.quickSort();
	testList.printSortTime();
	testList.shuffle();
	testList.radixSort();
	testList.printSortTime();

	indicators::show_console_cursor(false);

	// start chrono timer
	auto start = chrono::high_resolution_clock::now();

    // unordered map
    unordered_map<string, ReviewList> rawUserRatings;

    // ordered map
    map<string, ReviewList> sortedUserRatings;

	CSVReader reader("user_ratings.csv");
    CSVRow row;
	std::cout << "Loaded file." << std::endl;

	// progress bar
	ProgressBar bar;
	bar.set_option(option::BarWidth{ 70 });
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


	int i = 0;
	for (auto& row : reader) {
		string user = row["Username"];
		int game = row["BGGId"].get<int>();
		float rating = row["Rating"].get<float>();

        // Creating a new review object, to add to already existing Player's ReviewList, or to add to the
        // corresponding new Player's
        ReviewList::ReviewItem newItem(game, user, rating, "");

        // Adding to unsorted map, plain and simple
        if (rawUserRatings.find(user) == rawUserRatings.end())
        {
            ReviewList newList = ReviewList();
            newList.addReview(newItem);
            rawUserRatings.insert(std::make_pair(user, newList));
        }
        else
        {
            rawUserRatings[user].addReview(newItem);
        }

        // Checking sorted reviews for the user's name
        if (sortedUserRatings.find(user) == sortedUserRatings.end())
        {
            // If not found, create a new list, add the new review to the list, and add the user and their ReviewList
            // to our map of player's and their reviews.
            ReviewList newList = ReviewList();
            newList.addReview(newItem);
            sortedUserRatings.insert(std::make_pair(user, newList));
        }
        else
        {
            // If found, just add the review to the already existing spot in our map with the player's name
            sortedUserRatings[user].addReview(newItem);
        }

		if (i % 189423 == 0) {
			bar.tick();
		}
		i++;
	}

	indicators::show_console_cursor(true);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(end - start);
	std::cout << "Time: " << duration.count() << " seconds" << endl;
	return 0;
}