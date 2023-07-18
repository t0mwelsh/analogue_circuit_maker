#include "unit_class.h"

namespace unit_namespace {

	std::map<std::string, std::string> make_identifier(std::string name, std::string measure)
	{
		std::map<std::string, std::string> identifier;
		identifier.insert(std::make_pair("name", name));
		identifier.insert(std::make_pair("measure", measure));
		return identifier;
	}

	//unit class implementation
	double unit::get_freq() const { return omega; }
	void unit::set_freq(const double input) { omega = input; }
	double unit::get_impedance_magnitude() const { return abs(this->get_impedance()); } // true for all units
	double unit::get_phase() const { return arg(this->get_impedance()); } // true for all units - using standard complex library
	std::map<std::string, std::string> unit::get_identifier() const { return identifier; };

	// circuit class implementation

	circuit::circuit(const circuit& other) { // copy constructor
		for (const auto& unit_ptr : other.units) {
			units.push_back(unit_ptr->clone());
		}
		identifier = other.identifier;
	}
	circuit::circuit(circuit&& other) noexcept {
		units = std::move(other.units); other.units.clear(); identifier = std::move(other.identifier);
	} //move constructor

	circuit& circuit::operator=(const circuit& other) { //copy assignment operator
		if (this != &other) {
			units.clear();
			for (const auto& unit_ptr : other.units) {
				units.push_back(unit_ptr->clone());
			}
			identifier = other.identifier;
		}
		return *this;
	}

	circuit& circuit::operator=(circuit&& other) noexcept { //move assignment operator
		if (this != &other) {
			units = std::move(other.units);
			other.units.clear();
			identifier = std::move(other.identifier);
		}
		return *this;
	}

	void circuit::add_unit(std::unique_ptr<unit> input) { units.push_back(std::move(input)); }

	void circuit::print_func(int level, bool full_output) // recursive function that uses the variable 'level' to give indentation of nesting
	{
		std::cout << identifier["name"];
		if (full_output) {
			std::cout << " with impendance magnitude " << this->get_impedance_magnitude() << " and phase " << this->get_phase();
			std::cout << ". It has elements:" << std::endl;
		}
		else { std::cout << " with elements:" << std::endl; }
		
		std::vector<std::unique_ptr<unit>>::const_iterator iter;
		for (iter = units.begin(); iter != units.end(); iter++)
		{
			std::cout << std::string(level, '-'); (*iter)->print_func(level + 1, full_output);
		}
		std::cout << "//////////end of circuit/////////////" << std::endl;
	}

	//parallel circuit implementation
	parallel_circuit::parallel_circuit() { identifier = make_identifier("Parallel Circuit", "None"); } //default
	parallel_circuit::parallel_circuit(std::vector<std::unique_ptr<unit>>&& us) //parameterised
	{
		units = (std::move(us)); identifier = make_identifier("Parallel Circuit", "None");
	} //parameterised; using move as we have unique pointers

	std::unique_ptr<unit> parallel_circuit::clone() const { return std::make_unique<parallel_circuit>(*this); }

	std::complex<double> parallel_circuit::get_impedance() const {
		std::vector<std::unique_ptr<unit>>::const_iterator iter; std::complex<double> sum;
		for (iter = units.begin(); iter != units.end(); iter++)
			sum += 1. / ((*iter)->get_impedance());
		return 1. / sum;
	}

	//series circuit implementation
	series_circuit::series_circuit() { identifier = make_identifier("Series Circuit", "None"); } //default
	series_circuit::series_circuit(std::vector<std::unique_ptr<unit>>&& us)
	{
		units = (std::move(us)); identifier = make_identifier("Series Circuit", "None");
	} //parameterised

	std::unique_ptr<unit> series_circuit::clone() const { return std::make_unique<series_circuit>(*this); }

	std::complex<double> series_circuit::get_impedance() const {
		std::vector<std::unique_ptr<unit>>::const_iterator iter; std::complex<double> sum;
		for (iter = units.begin(); iter != units.end(); iter++)
			sum += (*iter)->get_impedance();
		return sum;
	}

	// Inductor implementation
	inductor::inductor() { characteristic = 0; identifier = make_identifier("Inductor", "H"); }
	inductor::inductor(double c) { //param constructor
		characteristic = 0;
		if (c >= 0) { characteristic = c; }
		else {
			std::cout << "This characteristic must be greater than zero; zero value assigned" << std::endl;
		}
		identifier = make_identifier("Inductor", "H");
	}

	std::complex<double> inductor::get_impedance() const {
		return std::complex<double>{ 0, characteristic* omega };
	}

	// Resistor implementation
	resistor::resistor() { characteristic = 0; identifier = make_identifier("Resistor", "Ohms"); }
	resistor::resistor(double c) {
		characteristic = 0;
		if (c >= 0) { characteristic = c; }
		else {
			std::cout << "This characteristic must be greater than zero; zero value assigned" << std::endl;
		}
		identifier = make_identifier("Resistor", "Ohms");
	}

	std::complex<double> resistor::get_impedance() const {
		return std::complex<double>{ characteristic, 0 };
	}

	// Capacitor implementation

	capacitor::capacitor() { characteristic = 0; identifier = make_identifier("Capacitor", "F"); }
	capacitor::capacitor(double c) {
		characteristic = 0;
		if (c >= 0) { characteristic = c; }
		else {
			std::cout << "This characteristic must be greater than zero; zero value assigned" << std::endl;
		}
		identifier = make_identifier("Capacitor", "F");;
	}

	std::complex<double> capacitor::get_impedance() const {
		return std::complex<double>{ 0, -1. / (characteristic * omega) };
	}

	double unit::omega = 0; // define static data member outside class

	void print_list(std::vector<std::unique_ptr<unit>>& input)
	{
		int index{ 1 };
		std::cout << "Current frequency is " << unit::omega << "Hz" << std::endl;
		std::vector<std::unique_ptr<unit>>::const_iterator iter;
		for (iter = input.begin(); iter != input.end(); iter++, index++)
		{
			std::cout << index << ". "; (*iter)->print_func(1, false);
		}
	}

	std::vector<std::unique_ptr<unit>> random_list_generator(int size, int b, int c)
	{
		std::vector<std::unique_ptr<unit>> random_list;
		srand(time(NULL)); // Seed the random number generator
		int x; double y;
		for (int i = 1; i <= size; i++) {
			x = rand() % 3; // generate a random integer between 0 and 3 exclusive
			y = b * static_cast<double>(rand()) / RAND_MAX; // generate a random double between 0 and b inclusive
			if (x == 0) { random_list.push_back(std::move(std::make_unique<inductor>(y/1000000))); } // division included for physical reasons
			if (x == 1) { random_list.push_back(std::move(std::make_unique<resistor>(y))); }
			if (x == 2) { random_list.push_back(std::move(std::make_unique<capacitor>(y/1000000.))); } // division included for physical reasons
		}
		unit::omega = c * static_cast<double>(rand()) / RAND_MAX; // assigns omega to a random double between 0 and c inclusive
		return random_list;
	}
}

