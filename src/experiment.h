#ifndef EXPERIMENT__
#define EXPERIMENT__

#include <fstream>
#include <vector>

class Experiment
{
public:
	explicit Experiment(std::string algo_str = "", std::string grouping = "", bool grouped=false, std::string optimizer="", std::string CB = "", size_t pid = 0, unsigned long long int nFE = 0);
	const std::string& algo_name() const { return algo_name_; }
	const std::string& grouping() const { return grouping_; }
	const std::string& optimizer() const { return optimizer_; }
	const std::string& CB() const { return contributationBase; }
	const size_t& problem_id() const { return problem_id_; }
	const unsigned long long int max_nFE()const { return max_nFE_; }
	const bool grouped() const { return grouped_; }

private:
	std::string algo_name_;
	std::string grouping_;
	std::string optimizer_;
	std::string contributationBase;
	size_t problem_id_;
	unsigned long long int max_nFE_;
	bool grouped_;
};

class ExperimentSet
{
public:
	explicit ExperimentSet(std::ifstream &ifile);

	Experiment & operator[](std::size_t i) { return exps[i]; }
	const Experiment & operator[](std::size_t i) const { return exps[i]; }

	std::size_t size() const { return exps.size(); }
	bool empty() const { return size() == 0; }
	void resize(std::size_t t) { exps.resize(t); }
	void push_back(const Experiment &exp) { exps.push_back(exp); }
	void clear() { exps.clear(); }

private:
	std::vector<Experiment> exps;
};

std::ostream& operator << (std::ostream& os, const Experiment& exp);

#endif // !EXPERIMENT__
