#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>

namespace Ember {
	class RandomGenerator {
	public:
		static int GenRandom(int min, int max);
		static double GenRandom(double min, double max);
	};
}

#endif // !RANDOM_GENERATOR_H
