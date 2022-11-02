#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <thread>
#include "csv.hpp"
#include <indicators.hpp>
using namespace std;
using namespace csv;
using namespace indicators;

int main() {
	indicators::show_console_cursor(false);

	// start chrono timer
	auto start = chrono::high_resolution_clock::now();
	unordered_map<string, unordered_map<int, float>> userRatings;

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
		userRatings[user].insert(std::make_pair(game, rating));
		userRatings[user][game] = rating;

		if (i % 189423 == 0) {
			bar.tick();
		}
		i++;
	}

	bar.mark_as_completed();

	indicators::show_console_cursor(true);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(end - start);
	std::cout << "Time: " << duration.count() << " seconds" << endl;
	return 0;
}