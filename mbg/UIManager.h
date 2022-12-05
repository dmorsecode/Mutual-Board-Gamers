#pragma once
#include "mbg\ReviewList.h"
class UIManager
{
	static int displayPrompts(std::vector<std::string> prompts);

public:
	static std::string getStringInput(std::string prompt);
	static int getIntInput(std::string prompt, int min, int max, int defaultEntry, int defaultValue);
	static int getIntInput(std::string prompt, int min, int max);
	static int getIntInput(int min, int max);
	static bool getBoolInput(std::string prompt);

	static int displayMainMenu();
	static int displayUserMenu(std::string selectedUser);

	static void printUserRatings(ReviewList user, bool comments);
	static void printRatingComparison(ReviewList user1, ReviewList user2, std::pair<std::string, std::string> names);
};

