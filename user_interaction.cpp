#include "user_interaction.h"

namespace user {
	bool this_or_that(std::string question, std::string option1, std::string option2)
	{
		std::cout << question << std::endl;
		std::cout << "Type '" << option1 << "' for the former and type '" << option2 << "' for the latter." << std::endl;

		std::string input_string;
		while (true)
		{
			getline(std::cin, input_string);
			if (input_string == option1) { return true; }
			else if (input_string == option2) { return false; }
			else { std::cout << "Please enter " << option1 << " or " << option2 << std::endl; }
		}
	}

	std::string this_that_or_the_other(std::string question, std::string option1, std::string option2, std::string option3)
	{
		std::cout << question << std::endl;
		std::cout << "Type '" << option1 << "' for the first option, '" << option2 << "' for the second and '" <<
			option3 << "' for the last." << std::endl;

		std::string input_string;
		while (true)
		{
			getline(std::cin, input_string);
			if (input_string == option1) { return option1; }
			else if (input_string == option2) { return option2; }
			else if (input_string == option3) { return option3; }
			else { std::cout << "Please enter " << option1 << " or " << option2 << " or " << option3 << std::endl; }
		}
	}

	std::vector<int> get_integer_vector(int min, int max)
		// returns an vector of integers from a user in a given range
	{
		std::string input;
		int num;
		std::vector<int> integers;
		std::cout << "Please enter integers successively. This will keep repeating until you type 'stop'. \n"
			<< "Note that typing a number twice will result in a double selection" << std::endl;

		while (true) {
			std::getline(std::cin, input);
			std::stringstream stream(input);
			// Use stringstream to attempt to convert input to an int
			if (stream >> num && stream.eof()) {
				if (num >= min && num <= max) {
					integers.push_back(num);
				}
				else { std::cout << "Input must be between " << min << " and " << max << "." << std::endl; }
			}
			else if (input == "stop" && integers.size() == 0) { std::cout << "Please choose at least one integer" << std::endl; }
			else if (input == "stop" && integers.size() != 0) { break; }
			else {
				std::cout << "Invalid input. Please enter an integer." << std::endl;
			}
		}
		return integers;
	}
}