#ifndef POPULATION__
#define POPULATION__

#include "alg_individual.h"
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

class Population
{
public:
	explicit Population(std::size_t s = 0) :individuals_(s) {}

	Individual & operator[](std::size_t i) { return individuals_[i]; }
	const Individual & operator[](std::size_t i) const { return individuals_[i]; }

	std::size_t size() const { return individuals_.size(); }
	bool empty() const { return size() == 0; }
	
	std::vector<Individual>::iterator begin() { return individuals_.begin(); }
	std::vector<Individual>::iterator end() { return individuals_.end(); }

	void sort() { std::sort(individuals_.begin(), individuals_.end()); }
	void push_back(const Individual &indv) { individuals_.push_back(indv); }

	void shuffle() 
	{ 
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(individuals_.begin(), individuals_.end(), std::default_random_engine(seed)); 
	}
	void resize(std::size_t t) { individuals_.resize(t); }
	void clear() { individuals_.clear(); }
	
private:
	std::vector<Individual> individuals_;
};


#endif // !POPULATION__

