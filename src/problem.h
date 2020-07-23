#ifndef PROBLEM__
#define PROBLEM__

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

class CProblem
{
public:
	explicit CProblem(int id = -1, std::string name = "", int dim = 0, double l_bd = 0.0, double u_bd = 0.0, double optimum = 0.0);

	const int & id() const { return id_; }
	const std::string & name() const { return func_name_; }
	const int & dim() const { return dimension_; }

	const double & lower_bound() const { return lower_bound_; }
	const double & upper_bound() const { return upper_bound_; }
	const double & global_optimum() const { return global_optimum_; }
	
	void set_id(const size_t new_id) { id_ = new_id; }
	void set_name(const std::string new_name) { func_name_ = new_name; }
	void set_dim(const size_t new_dim) { dimension_ = new_dim; }

	void set_lower_bound(const double new_lb) { lower_bound_ = new_lb; }
	void set_upper_bound(const double new_ub) { upper_bound_ = new_ub; }
	void set_global_optimum(const double new_optimum) { global_optimum_ = new_optimum; }

private:
	int id_, dimension_;
	std::string func_name_;
	double lower_bound_, upper_bound_, global_optimum_;
};

std::ostream & operator << (std::ostream &os, const CProblem &problem);

#endif // !PROBLEM__
