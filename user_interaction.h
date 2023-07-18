//header file for useful functions when interacting with a user through the console
#pragma once
#ifndef user_interaction_h
#define user_interaction_h

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace user {

	bool this_or_that(std::string question, std::string option1, std::string option2);
	// returns a boolean to say if a user chose option1

	std::string this_that_or_the_other(std::string question, std::string option1, std::string option2, std::string option3);
	// returns a string of the chosen option

	template<typename T> // template to get a positive number of a certain type off of a user
	T get_positive_number() {
		std::string input;
		T num;
		std::cout << "Please enter a number greater than 0: ";

		while (true) {
			std::getline(std::cin, input);
			std::stringstream stream(input);
			if (stream >> num && num > 0) {
				break;
			}
			std::cout << "Invalid input. Please enter a number greater than 0: ";
		}
		return num;
	}

	std::vector<int> get_integer_vector(int min, int max);
	// returns an vector of integers from a user in a given range
}
#endif
