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
		}
		ReviewItem(int id, std::string name, float rating, std::string comment) {
			gameID = id;
			gameName = name;
			this->rating = rating;
			this->comment = comment;
		}
		ReviewItem(int id, std::string name, float rating) {
			gameID = id;
			gameName = name;
			this->rating = rating;
		}
	};
	std::vector<ReviewItem> reviews;
	int sortTime = 0;
	std::string sortMethod;
	std::string sortSuffix = "milliseconds";
	void quickSort(int left, int right);
    int sortType = -1;
public:
	ReviewList();
	int getSize();
	std::vector<ReviewItem>& getReviews();
	int getSortType();
	void setSortType(int sortType);
	void addReview(int gameID, std::string gameName, float rating, std::string comment);
	void addReview(int gameID, std::string gameName, float rating);
	void addReview(ReviewItem review);
	int findIndex(ReviewItem& game);
	ReviewItem getIndex(int index);
	void printReviews();
	void countingSort();
	void quickSort();
	void radixSort();
	void printSortTime();
	void shuffle();
	std::pair<ReviewList, ReviewList> getIntersection(ReviewList& rhs);
};