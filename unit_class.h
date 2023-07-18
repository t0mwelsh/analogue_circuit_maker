// header file for the unit class hierachy. A unit refers to an overall combination of circuit elements; the elements
// can be circuits themselves or just simple components, like an inductor.
#pragma once
#ifndef unit_class_h
#define unit_class_h

#include <iostream>
#include <iomanip>
#include <complex>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>

namespace unit_namespace {

	std::map<std::string, std::string> make_identifier(std::string name, std::string measure);
	//function makes the std::map identifier data-member without having to redefine key everytime

	class unit //abstract base class
	{
	protected:
		std::map<std::string, std::string> identifier; //identifier is used to differentiate between different components/circuits
		static double omega; //frequency of the AC circuit

		friend void print_list(std::vector<std::unique_ptr<unit>>& input); //needs to be friend function to access omega
		// iterates through a list of base class pointers and uses print_func class members to print each element
		friend std::vector<std::unique_ptr<unit>> random_list_generator(int size, int b, int c);
		//Creates random list of base class pointers with characteristics between zero and a chosen value. 
		//Also randomly sets frequency of unit class hierachy between zero and a chosen value.

	public:
		virtual ~unit() {}; // Need this!
		double get_freq() const; // all units should be able to access and set the frequency
		void set_freq(const double input);
		double get_impedance_magnitude() const;
		double get_phase() const;
		virtual std::complex<double> get_impedance() const = 0; // pure virtual function
		std::map<std::string, std::string> get_identifier() const;

		virtual std::unique_ptr<unit> clone() const = 0; // pure virtual function - creates a copy of a derived class unique pointer

		virtual void print_func(int level, bool full_output) = 0; // pure virtual function
		// described at the particular derived class in question
	};

	class circuit : public unit // this inheritance scheme is used so we can put subcircuits in a circuit
	{
	protected:
		std::vector<std::unique_ptr<unit>> units;
	public:
		circuit() = default; // include this to define copy and move even though we shall never instantiate a circuit class itself
		~circuit() {} // virtual-ness is inherited
		virtual std::unique_ptr<unit> const clone() { return nullptr; } // still needs to be defined in derived classes

		circuit(const circuit& other); //copy constructor
		circuit(circuit&& other) noexcept; //move constructor
		circuit& operator=(const circuit& other); //copy assignment operator
		circuit& operator=(circuit&& other) noexcept; //move assignment operator

		void add_unit(std::unique_ptr<unit> input); // adds a unit to the circuit

		void print_func(int level, bool full_output) override; // prints out all components inside its units data-member
	};

	class parallel_circuit : public circuit
	{
	public:
		parallel_circuit(); //default
		parallel_circuit(std::vector<std::unique_ptr<unit>>&& us); //parameterised
		~parallel_circuit() {} // virtual-ness is inherited
		std::unique_ptr<unit> clone() const override;

		std::complex<double> get_impedance() const override;
	};

	class series_circuit : public circuit
	{
	public:
		series_circuit(); //default
		series_circuit(std::vector<std::unique_ptr<unit>>&& us); //parameterised
		~series_circuit() {} // virtual-ness is inherited
		std::unique_ptr<unit> clone() const override;

		std::complex<double> get_impedance() const override;
	};

	template<typename T> //template implementation must be kept in header file
	class component_impl : public unit {
	protected:
		double characteristic;
	public:
		component_impl() = default; // Compiler doesn't like this not included as gives warning characteristic is uninitialised. Doesn't really matter
		~component_impl() {}

		std::unique_ptr<unit> clone() const override {
			return std::make_unique<T>(static_cast<const T&>(*this));
		}

		void set_characteristic(double c) {
			if (c >= 0) { characteristic = c; }
			else { std::cout << "This characteristic must be greater than zero; value unchanged" << std::endl; }
			characteristic = c;
		}
		double get_characteristic() const { return characteristic; }

		void print_func(int level, bool full_output) override { // prints out what the component is and its characteristic
			std::cout << identifier["name"] << ": " << characteristic << " " << identifier["measure"];
			if (full_output){
				std::cout << "; impedance magnitude: " << this->get_impedance_magnitude() << ", phase: " << this->get_phase();
			}
			std::cout << std::endl;
		}
	};

	class inductor : public component_impl<inductor> {
	public:
		inductor();
		inductor(double c);
		~inductor() {}

		using component_impl<inductor>::component_impl;
		std::complex<double> get_impedance() const override;
	};

	class resistor : public component_impl<resistor> {
	public:
		resistor();
		resistor(double c);
		~resistor() {}

		using component_impl<resistor>::component_impl;
		std::complex<double> get_impedance() const override;
	};

	class capacitor : public component_impl<capacitor> {
	public:
		capacitor();
		capacitor(double c);
		~capacitor() {}

		using component_impl<capacitor>::component_impl;
		std::complex<double> get_impedance() const override;
	};

	std::vector<std::unique_ptr<unit>> random_list_generator(int size, int b, int c); //had to be included again at bottom as otherwise get a function identifier not found error
	void print_list(std::vector<std::unique_ptr<unit>>& input);
}

#endif

