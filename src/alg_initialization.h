#ifndef INITIALIZATION__
#define INITIALIZATION__

class Individual;
class Population;
class CProblem;

class TRandomInitialization
{
public:
	void operator()(Population *pop, const CProblem &prob) const;
	void operator()(Individual *indv, const CProblem &prob) const;
};

extern TRandomInitialization RandomInitialization;

#endif