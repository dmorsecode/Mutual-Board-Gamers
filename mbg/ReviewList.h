#pragma once
#include <vector>
#include <iostream>
#include <string>

class ReviewList {
	struct ReviewItem {
		int gameID;
		std::string gameName;
		float rating;
		std::string comment;
		ReviewItem() {
			// generate random values
			gameID = rand() % 1000;
			gameName = "Game " + std::to_string(gameID);
			rating = rand() % 10;
			comment = "This is a comment for game " + std::to_string(gameID);
		}
		ReviewItem(int id, std::string name, float rating, std::string comment) {
			gameID = id;
			gameName = name;
			this->rating = rating;
			this->comment = comment;
		}
	};
	std::vector<ReviewItem> reviews;
	int sortTime = 0;
	std::string sortMethod;
	std::string sortSuffix = "milliseconds";
	void quickSort(int left, int right);
public:
	ReviewList();
	int getSize();
	std::vector<ReviewItem> getReviews();
	void addReview();
	void printReviews();
	void countingSort();
	void quickSort();
	void radixSort();
	void printSortTime();
	void shuffle();
};