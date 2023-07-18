/*
Code constructs a class hierachy for analogue circuits with ideal components.
The code then creates a user environent through the console for a user to create a list of components that
can then be added to with more components or circuits they make. Code can calculate the impedance of these
items as a complex number.

By Tomas Welsh. Last edited 18/07/2023
*/


#include "unit_class.h"
#include "user_interaction.h"
using namespace unit_namespace;

void user_add_unit(std::vector<std::unique_ptr<unit>>& unit_list)
{
	std::string component_choice = user::this_that_or_the_other("Do you want to add a capacitor, resistor or inductor?",
		"c", "r", "i");
	std::cout << "Now you must choose the value of the characteristic of your chosen component." << std::endl;
	double characteristic = user::get_positive_number<double>();

	if (component_choice == "c") { unit_list.push_back(std::make_unique<capacitor>(characteristic)); }
	if (component_choice == "r") { unit_list.push_back(std::make_unique<resistor>(characteristic)); }
	if (component_choice == "i") { unit_list.push_back(std::make_unique<inductor>(characteristic)); }
}

std::vector<std::unique_ptr<unit>> user_make_sublist(std::vector<std::unique_ptr<unit>>& biglist)
{
	std::vector<std::unique_ptr<unit>> sublist;

	std::cout << "Which elements would you like to construct your circuit from?"
		" Type numbers from your current list." << std::endl;
	std::vector<int> selected_units = user::get_integer_vector(1, biglist.size());

	for (int i = 0; i < selected_units.size(); i++)
	{
		sublist.push_back(biglist[selected_units[i] - 1]->clone());
	}
	return sublist;
}

int main()
{
	std::vector<std::unique_ptr<unit>> master_list;
	std::unique_ptr<unit> constructed_circuit = nullptr;

	std::cout << "Hello, this program will allow you to construct arbitrary circuits from \n"
		"capacitors, resistors and inductors." << std::endl;

	if (user::this_or_that("Would you like to start with a list of random components or start from scratch?", "random", "scratch")) {
		std::cout << "Type a number to choose the size of your list" << std::endl;
		int size = user::get_positive_number<int>();
		master_list = random_list_generator(size, 10, 50);
	}
	else
	{
		std::cout << "You must add a component before you begin" << std::endl;
		user_add_unit(master_list);
	}

	while (true)
	{
		std::cout << "This is your current list of units and the operating frequency:" << std::endl;
		print_list(master_list);

		if (user::this_or_that("Would you like to add a component to the list or make a circuit?", "add", "make"))
		{
			user_add_unit(master_list);
		}

		else
		{
			if (user::this_or_that("Would you like to change the frequency or not?", "yes", "no"))
			{
				master_list[0]->set_freq(user::get_positive_number<double>());
			} //master_list cannot be empty so OK


			if (user::this_or_that("Would you like to construct a parallel or series circuit?", "p", "s"))
			{
				constructed_circuit = std::move(std::make_unique<parallel_circuit>(user_make_sublist(master_list)));
			}

			else
			{
				constructed_circuit = std::move(std::make_unique<series_circuit>(user_make_sublist(master_list)));
			}

			std::cout << "You have contructed a ";
			constructed_circuit->print_func(1, true);
			std::cout << "This circuit has impedance: " << constructed_circuit->get_impedance() << std::endl;

			if (user::this_or_that("Would you like to add this circuit to your list or not?", "yes", "no"))
			{
				master_list.push_back(constructed_circuit->clone());
			}

			if (user::this_or_that("Would you like to exit the code or keep working with your current list?", "exit", "keep working")) { break; }
			else { std::cout << std::endl; }
		}
	}
	return 0;
}
