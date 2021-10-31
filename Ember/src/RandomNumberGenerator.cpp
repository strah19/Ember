/**
 * @file RandomNumberGenerator.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file contains simple random generator functions using the mt19937.
 */

#include "RandomNumberGenerator.h"

namespace Ember {
	static std::random_device random_device;
	static std::mt19937 random_engine(random_device());

	/**
	* A random number generator for integers.
	* 
	* @param int min value of the number.
	* @param int max value of the number.
	* 
	* @return int the random number.
	*/
	int RandomGenerator::GenRandom(int min, int max) {
		static std::uniform_int_distribution<int> int_distro(min, max);
		return int_distro(random_engine);
	}

	/**
	* A random number generator for doubles.
	* 
	* @param double min value of the number.
	* @param double max value of the number.
	* 
	* @return double the random number.
	*/
	double RandomGenerator::GenRandom(double min, double max) {
		static std::uniform_real_distribution<double> dbl_distro(min, max);
		return dbl_distro(random_engine);
	}
}