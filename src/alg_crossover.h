#ifndef CROSSOVER__
#define CROSSOVER__

class Individual;
class Population;

//------------------------------------
// Global Discrete Recombination (GDR)
//------------------------------------
class GlobalDiscreteRecombination  
{
public:
	explicit GlobalDiscreteRecombination(double pc = 1.0) : pc_(pc) {}

	void SetCrossoverRate(double pc) { pc_ = pc;  }
	double CrossoverRate() { return pc_; }

	bool operator() (Individual *c, const Population &pop, const size_t Pop_Size, double pc) const;
	bool operator() (Individual *c, const Population &pop, const size_t Pop_Size) const { return operator() (c, pop, Pop_Size, pc_); }

private:
	double pc_; //crossover rate
};

//---------------------------------------
// Local Intermediate Recombination (LIR)
//---------------------------------------
class LocalIntermediateRecombination
{
public:
	explicit LocalIntermediateRecombination(double pc = 1.0) : pc_(pc) {}

	void SetCrossoverRate(double pc) { pc_ = pc; }
	double CrossoverRate() { return pc_; }

	bool operator() (Individual *c, const Individual &p1, const Individual &p2, double pc) const;
	bool operator() (Individual *c, const Individual &p1, const Individual &p2) const { return operator()(c, p1, p2, pc_); }


private:
	double pc_;
};

#endif // !CROSSOVER__

