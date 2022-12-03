#include "UIManager.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

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
		cout << "Invalid input. Please enter Y or N." << endl;
		std::getline(cin, result);
	}
	cout << endl;

	return result;
}

int UIManager::getIntInput(std::string prompt, int min, int max, int defaultEntry, int defaultValue)
{
	cout << prompt << endl;
	cout << "[" << min << "-" << max << "] (enter " << defaultEntry << " for " << defaultValue << ")" << endl;

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
	cout << "[" << min << "-" << max << "]" << endl;

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

	prompts.push_back("View a random user's reviews");
	prompts.push_back("View a specific user's reviews");
	prompts.push_back("Exit");

	return displayPrompts(prompts);
}

int UIManager::displayUserMenu(std::string selectedUser)
{
	cout << "User Menu: " << selectedUser << endl;

	std::vector<string> prompts;
	prompts.push_back("Show " + selectedUser + "'s reviews by name");
	prompts.push_back("Show " + selectedUser + "'s reviews by rating");
	prompts.push_back("Compare with second user");
	prompts.push_back("Go back to main menu");

	return displayPrompts(prompts);
}

void UIManager::printUserRatings(ReviewList user, int sort)
{
	for (int i = 0; i < user.getSize(); i++) {
		cout << user.getReviews()[i].gameName << ": " << user.getReviews()[i].rating << endl;
	}

	cout << endl;

}

void UIManager::printRatingComparison(ReviewList user1, ReviewList user2, int sort)
{
	for (int i = 0; i < user1.getSize(); i++) {
		cout << user1.getReviews()[i].gameName << ": " << user1.getReviews()[i].rating << " | " << user2.getReviews()[i].rating << endl;
	}

	cout << endl;
}
