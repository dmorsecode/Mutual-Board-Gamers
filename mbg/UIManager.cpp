#include "UIManager.h"
#include <tabulate\tabulate.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using tabulate::Table;


int UIManager::displayPrompts(std::vector<std::string> prompts)
{
	for (int i = 0; i < prompts.size(); i++) {
		cout << i + 1 << ". " << prompts[i] << endl;
	}

	int result = getIntInput(1, prompts.size());

	return result;
}

int UIManager::getIntInput(int min, int max) {
	cout << "> ";
	int result;

	cin >> result;

	// check if result is a valid number
	while (cin.fail() || result < min || result > max) {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "Invalid input. Please enter a number between " << min << " and " << max << "." << endl;
		cin >> result;
	}

	cout << endl;

	cin.clear();
	cin.ignore(256, '\n');

	return result;
}

std::string UIManager::getStringInput(std::string prompt)
{
	cout << prompt << endl;
	cout << "> ";

	string result;

	std::getline(cin, result);

	// TODO: input validation (regex?)
	while (cin.fail() || result == "") {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "Invalid input. Please enter a proper string." << endl;
		std::getline(cin, result);
	}
	cout << endl;

	return result;
}

int UIManager::getIntInput(std::string prompt, int min, int max, int defaultEntry, int defaultValue)
{
	cout << prompt << endl;
	cout << "[" << min << " - " << max << "] ( enter " << defaultEntry << " for " << defaultValue << " )" << endl;

	cout << "> ";
	int result;

	cin >> result;

	// check if result is a valid number
	while (cin.fail() || (result < min && result != defaultEntry) || (result > max && result != defaultEntry)) {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "Invalid input. Please enter a number between " << min << " and " << max << "." << endl;
		cin >> result;
	}
	if (result == defaultEntry) {
		result = defaultValue;
	}
	cout << endl;

	return result;
}

int UIManager::getIntInput(std::string prompt, int min, int max)
{
	cout << prompt << endl;
	cout << "[" << min << " - " << max << "]" << endl;

	string entry;

	cout << "> ";
	int result;

	cin >> result;

	// check if result is a valid number
	while (cin.fail() || result < min || result > max) {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "Invalid input. Please enter a number between " << min << " and " << max << "." << endl;
		cin >> result;
	}

	cout << endl;

	return result;
}

bool UIManager::getBoolInput(string prompt)
{
	cout << prompt << " [Y/N]" << endl;
	string result;

	cout << "> ";
	cin >> result;
	
	// check if result is a valid input
	while (result != "Y" && result != "y" && result != "N" && result != "n") {
		cout << "Invalid input. Please enter Y or N." << endl;
		cin >> result;
	}

	cout << endl;

	if (result == "Y" || result == "y")
		return true;
	else
		return false;
}

int UIManager::displayMainMenu()
{
	std::vector<string> prompts;

	cout << "Main Menu" << endl;

	prompts.push_back("View a specific user.");
	prompts.push_back("View a random user.");
	prompts.push_back("Change sorting method.");
	prompts.push_back("Exit");

	return displayPrompts(prompts);
}

int UIManager::displayUserMenu(std::string selectedUser)
{
	cout << "User Menu: " << selectedUser << endl;

	std::vector<string> prompts;
	prompts.push_back("Show " + selectedUser + "'s reviews.");
	prompts.push_back("Compare with specific user.");
	prompts.push_back("Compare with random user.");
	prompts.push_back("Go back to main menu");

	return displayPrompts(prompts);
}

void UIManager::printUserRatings(ReviewList user, bool comments)
{	
	Table userRatings;
	
	if (comments) {
		userRatings.add_row({ "Game", "Rating", "Comment" });
	}
	else {
		userRatings.add_row({ "Game", "Rating" });
	}
	userRatings[0].format()
		.padding_top(1)
		.padding_bottom(1)
		.font_align(tabulate::FontAlign::center)
		.font_style({ tabulate::FontStyle::underline });

	if (user.getSize() == 0) {
		cout << "No reviews found." << endl;
		return;
	}

	for (int i = 0; i < user.getSize(); i++) {
		string rating = std::to_string(user.getReviews()[i].rating + 0.05).substr(0, 3);
		if (rating == "10.") {
			rating = "10.0";
		}
		if (comments) {
			userRatings.add_row({ user.getReviews()[i].gameName, rating, user.getReviews()[i].comment });
		}
		else {
			userRatings.add_row({ user.getReviews()[i].gameName, rating });
		}
	}

	userRatings.print(cout);

	cout << endl;

}

void UIManager::printRatingComparison(ReviewList user1, ReviewList user2, std::pair<string, string> names)
{
	
	Table userRatingsCompare;

	userRatingsCompare.add_row({ "Game", "Rating (" + names.first + ")", "Rating (" + names.second + ")", "Compatibility"});

	userRatingsCompare[0].format()
		.padding_top(1)
		.padding_bottom(1)
		.font_align(tabulate::FontAlign::center)
		.font_style({ tabulate::FontStyle::underline });

	if (user1.getSize() == 0 || user2.getSize() == 0) {
		cout << "No reviews in common found." << endl << endl;
		return;
	}

	std::vector<double> compatList;

	for (int i = 0; i < user1.getSize(); i++) {
		string user1rating = std::to_string(user1.getReviews()[i].rating + 0.05).substr(0, 3);
		string user2rating = std::to_string(user2.getReviews()[i].rating + 0.05).substr(0, 3);
		string compat = std::to_string(5.0 - abs(user1.getReviews()[i].rating - user2.getReviews()[i].rating) + 0.05).substr(0, 3);
		if (user1rating == "10.") {
			user1rating = "10.0";
		}
		if (user2rating == "10.") {
			user2rating = "10.0";
		}
		if (compat[0] == '-') {
			compat += '0';
		}
		compatList.push_back(stod(compat));
		userRatingsCompare.add_row({ 
			user1.getReviews()[i].gameName, 
			user1rating, 
			user2rating,
			compat});
	}

	userRatingsCompare.add_row({ // Add a spacer.
			"",
			"",
			"",
			"" });
	
	// calculate average compatibility
	double sum = 0;
	for (int i = 0; i < compatList.size(); i++) {
		sum += compatList[i];
	}
	double avg = sum / compatList.size();
	string avgStr = std::to_string(avg + 0.05).substr(0, 3); // Fix it to 1 decimal place.
	if (avgStr[0] == '-') {
		avgStr += '0';
	}
	userRatingsCompare.add_row({
		"OVERALL COMPATIBILITY",
		"",
		"",
		avgStr });

	userRatingsCompare.print(cout);

	cout << endl;
}
