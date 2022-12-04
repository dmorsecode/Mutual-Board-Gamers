#include "ReviewList.h"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

ReviewList::ReviewList()
{
	return;
}

int ReviewList::getSize()
{
	return reviews.size();
}

std::vector<ReviewList::ReviewItem>& ReviewList::getReviews()
{
	return reviews;
}

int ReviewList::getSortType()
{
	return sortType;
}

void ReviewList::setSortType(int _sortType)
{
    sortType = _sortType;
}

void ReviewList::addReview(int gameID, std::string gameName, float rating, std::string comment)
{
	reviews.push_back(ReviewItem(gameID, gameName, rating, comment));
}

void ReviewList::addReview(int gameID, std::string gameName, float rating)
{
	reviews.push_back(ReviewItem(gameID, gameName, rating));
}

void ReviewList::addReview(ReviewItem review)
{
	reviews.push_back(review);
}

int ReviewList::findIndex(ReviewItem& game)
{
	// Use a binary search to find the index of the matching review depending on what the sort type is.
	// If the sort type is 1, then sort by gameID.
	// If the sort type is 2, then sort by gameName.
	// If the sort type is 3, then... unfortunately, we need a linear search.
	if (sortType == 3) {
		for (int i = 0; i < reviews.size(); i++) {
			if (reviews[i].gameID == game.gameID && reviews[i].gameName == game.gameName) {
				return i;
			}
		}
		return -1;
	}
	int left = 0;
	int right = reviews.size() - 1;
	int mid = 0;
	while (left <= right) {
		mid = (left + right) / 2;
		if (sortType == 1) {
			if (game.gameID == reviews[mid].gameID) {
				return mid;
			}
			else if (game.gameID < reviews[mid].gameID) {
				right = mid - 1;
			}
			else {
				left = mid + 1;
			}
		}
		else if (sortType == 2) {
			if (game.gameName == reviews[mid].gameName) {
				return mid;
			}
			else if (game.gameName < reviews[mid].gameName) {
				right = mid - 1;
			}
			else {
				left = mid + 1;
			}
		}
	}
	// If not found, return -1.
	return -1;
}

ReviewList::ReviewItem ReviewList::getIndex(int index)
{
	return reviews[index];
}

void ReviewList::printReviews()
{
	for (int i = 0; i < reviews.size(); i++) {
        // Moved this std::endl to the top of the for loop for it to print cleaner
        std::cout << std::endl;
		std::cout << "Game: " << reviews[i].gameName << " (" << reviews[i].gameID << ")" << std::endl;
		std::cout << "Rating: " << reviews[i].rating << std::endl;
		std::cout << "Comment: " << reviews[i].comment << std::endl;
	}
}

void ReviewList::countingSort()
{	
	// start timer for sort
	auto start = std::chrono::high_resolution_clock::now();

	// Counting Sort by game ids
	int max = 0;
	for (int i = 0; i < reviews.size(); i++) {
		if (reviews[i].gameID > max) {
			max = reviews[i].gameID;
		}
	}
	int* count = new int[max + 1];
	for (int i = 0; i < max + 1; i++) {
		count[i] = 0;
	}
	for (int i = 0; i < reviews.size(); i++) {
		count[reviews[i].gameID]++;
	}
	for (int i = 1; i < max + 1; i++) {
		count[i] += count[i - 1];
	}
	ReviewItem* temp = new ReviewItem[reviews.size()];
	for (int i = reviews.size() - 1; i >= 0; i--) {
		temp[count[reviews[i].gameID] - 1] = reviews[i];
		count[reviews[i].gameID]--;
	}
	for (int i = 0; i < reviews.size(); i++) {
		reviews[i] = temp[i];
	}
	delete[] count;
	delete[] temp;

	// stop timer for sort
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	sortTime = duration.count();
	if (sortTime == 0) {
		auto newDuration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		sortTime = newDuration.count();
		sortSuffix = "microseconds";
	}
	else {
		sortSuffix = "milliseconds";
	}
	sortMethod = "Counting Sort";
}

void ReviewList::quickSort() {
	// start timer for sort
	auto start = std::chrono::high_resolution_clock::now();
	
	quickSort(0, reviews.size() - 1);

	// stop timer for sort
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	sortTime = duration.count();
	// if count is too small, change to microseconds
	if (sortTime == 0) {
		auto newDuration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		sortTime = newDuration.count();
		sortSuffix = "microseconds";
	}
	else {
		sortSuffix = "milliseconds";
	}
	sortMethod = "Quick Sort";
}

void ReviewList::quickSort(int left, int right) {
	int i = left, j = right;
	if (sortType == 1) {
		// Quicksort by game ID ascending
		int pivot = reviews[(left + right) / 2].gameID;
		while (i <= j) {
			while (reviews[i].gameID < pivot) {
				i++;
			}
			while (reviews[j].gameID > pivot) {
				j--;
			}
			if (i <= j) {
				ReviewItem temp = reviews[i];
				reviews[i] = reviews[j];
				reviews[j] = temp;
				i++;
				j--;
			}
		}
		if (left < j) {
			quickSort(left, j);
		}
		if (i < right) {
			quickSort(i, right);
		}
	}
	else if (sortType == 2) {
		// Quicksort by game name
		std::string pivot = reviews[(left + right) / 2].gameName;
		while (i <= j) {
			while (reviews[i].gameName < pivot) {
				i++;
			}
			while (reviews[j].gameName > pivot) {
				j--;
			}
			if (i <= j) {
				ReviewItem temp = reviews[i];
				reviews[i] = reviews[j];
				reviews[j] = temp;
				i++;
				j--;
			}
		}
		if (left < j) {
			quickSort(left, j);
		}
		if (i < right) {
			quickSort(i, right);
		}
	}
	else if (sortType == 3) {
		// Quicksort by rating descending
		float pivot = reviews[(left + right) / 2].rating;
		while (i <= j) {
			while (reviews[i].rating > pivot) {
				i++;
			}
			while (reviews[j].rating < pivot) {
				j--;
			}
			if (i <= j) {
				ReviewItem temp = reviews[i];
				reviews[i] = reviews[j];
				reviews[j] = temp;
				i++;
				j--;
			}
		}
		if (left < j) {
			quickSort(left, j);
		}
		if (i < right) {
			quickSort(i, right);
		}
	}
}

void ReviewList::radixSort()
{
	// start timer for sort
	auto start = std::chrono::high_resolution_clock::now();

	// Radix Sort by game ids
	int max = 0;
	for (int i = 0; i < reviews.size(); i++) {
		if (reviews[i].gameID > max) {
			max = reviews[i].gameID;
		}
	}
	int maxDigits = 0;
	while (max > 0) {
		max /= 10;
		maxDigits++;
	}
	int* count = new int[10];
	ReviewItem* temp = new ReviewItem[reviews.size()];
	for (int i = 0; i < maxDigits; i++) {
		for (int j = 0; j < 10; j++) {
			count[j] = 0;
		}
		for (int j = 0; j < reviews.size(); j++) {
			count[(reviews[j].gameID / (int)pow(10, i)) % 10]++;
		}
		for (int j = 1; j < 10; j++) {
			count[j] += count[j - 1];
		}
		for (int j = reviews.size() - 1; j >= 0; j--) {
			temp[count[(reviews[j].gameID / (int)pow(10, i)) % 10] - 1] = reviews[j];
			count[(reviews[j].gameID / (int)pow(10, i)) % 10]--;
		}
		for (int j = 0; j < reviews.size(); j++) {
			reviews[j] = temp[j];
		}
	}
	delete[] count;
	delete[] temp;

	// stop timer for sort
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	sortTime = duration.count();
	if (sortTime == 0) {
		auto newDuration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		sortTime = newDuration.count();
		sortSuffix = "microseconds";
	}
	else {
		sortSuffix = "milliseconds";
	}
	sortMethod = "Radix Sort";
}

void ReviewList::printSortTime()
{
	// print method and time
	std::cout << sortMethod << " took " << sortTime << " " << sortSuffix << " to sort " << reviews.size() << " reviews." << std::endl;
}

void ReviewList::shuffle()
{
	// Shuffles the ReviewItem vector.
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(reviews.begin(), reviews.end(), g);
}

std::pair<ReviewList, ReviewList> ReviewList::getIntersection(ReviewList& rhs)
{
	ReviewList& smaller = getSize() < rhs.getSize() ? *this : rhs;
	ReviewList& larger = getSize() > rhs.getSize() ? *this : rhs;

	ReviewList returnListS;
	ReviewList returnListL;

	// iterate through all elements of smaller list, adding if match is in larger list
	for (int i = 0; i < smaller.getSize(); i++) {

		int largeIndex = larger.findIndex(smaller.getReviews()[i]);

		if (largeIndex != -1) {
			returnListS.addReview(smaller.getReviews()[i]);
			returnListL.addReview(larger.getReviews()[largeIndex]);
		}
	}

	// return in correct order 
	if (getSize() < rhs.getSize())
		return std::pair<ReviewList, ReviewList>(returnListS, returnListL);
	else
		return std::pair<ReviewList, ReviewList>(returnListL, returnListS);
}
