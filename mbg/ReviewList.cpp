#include "ReviewList.h"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

ReviewList::ReviewList()
{
	// generate 1000 game IDs in sequential order
	for (int i = 0; i < 43; i++) {
		reviews.push_back(ReviewItem(i, "Game " + std::to_string(i), 0, "This is a comment for game " + std::to_string(i)));
	}
}

int ReviewList::getSize()
{
	return reviews.size();
}

std::vector<ReviewList::ReviewItem> ReviewList::getReviews()
{
	return reviews;
}

void ReviewList::addReview()
{
	return;
}

void ReviewList::printReviews()
{
	for (int i = 0; i < reviews.size(); i++) {
		std::cout << "Game ID: " << reviews[i].gameID << std::endl;
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
	
	// Quick Sort by game ids
	int i = left, j = right;
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
	// shuffle the vector and copy it into reviews and reviews
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(reviews.begin(), reviews.end(), g);
}
