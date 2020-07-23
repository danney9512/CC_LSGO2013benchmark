#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>
#include <ostream>

class CProblem;

class Individual
{
public:
	typedef std::vector<double> GeneVec;
	
	explicit Individual(std::size_t gene_len = 0, double fit_val = 0.0);

	GeneVec & gene() { return gene_;  }
	const GeneVec & gene() const { return gene_; }
	double & fitness() { return fitness_; }
	const double & fitness() const { return fitness_; }

	static void SetTargetProblem(const CProblem &p) { target_problem_ = &p; }
	static const CProblem & TargetProblem() { return *target_problem_; }

	bool operator < (const Individual &cmp) const { return fitness_ < cmp.fitness_; }
	bool operator == (const Individual &cmp) const
	{
		if (gene_.size() != cmp.gene().size()) return false;
		for (size_t i = 0; i < gene_.size(); ++i)
		{
			if (gene_[i] != cmp.gene()[i]) return false;
		}
		return true;
	}

private:
	GeneVec gene_;
	double fitness_;
	static const CProblem *target_problem_;
};

std::ostream & operator << (std::ostream &os, const Individual &indv);

#endif // !INDIVIDUAL__

