#pragma once

#include <random>

class RandomNumberGenerator
{
private:
	std::default_random_engine defEngine{};

public:
	RandomNumberGenerator() {}
	~RandomNumberGenerator() = default;

	//template <typename T>
	float getNumber(float from, float to) {
		std::uniform_real_distribution<float> floatDistro1{ from, to };
		return floatDistro1(defEngine);
	}
	
};

