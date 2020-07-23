#ifndef PROBLEM_SET__
#define PROBLEM_SET__

#include <fstream>
#include <vector>
#include "problem.h"

class CProblem;

class CPromblemSet
{
public:
	explicit CPromblemSet(std::ifstream &ifile);

	CProblem & operator[](std::size_t i) { return problems_[i]; }
	const CProblem & operator[](std::size_t i) const { return problems_[i]; }

	std::size_t size() const { return problems_.size(); }
	bool empty() const { return size() == 0; }
	void resize(std::size_t t) { problems_.resize(t); }
	void push_back(const CProblem &prob) { problems_.push_back(prob); }
	void clear() { problems_.clear(); }

private:
	std::vector<CProblem> problems_;
};




#endif // !PROBLEM_SET__

