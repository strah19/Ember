#include "RandomNumberGenerator.h"

namespace Ember {
	static std::random_device random_device;
	static std::mt19937 random_engine(random_device());

	int RandomGenerator::GenRandom(int min, int max) {
		static std::uniform_int_distribution<int> int_distro(min, max);
		return int_distro(random_engine);
	}

	double RandomGenerator::GenRandom(double min, double max) {
		static std::uniform_real_distribution<double> dbl_distro(min, max);
		return dbl_distro(random_engine);
	}
}