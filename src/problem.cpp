#include "problem.h"
#include <sstream>
#include <fstream>
#include <cstdio>

CProblem::CProblem(int id, std::string name, int dim, double l_bd, double u_bd, double optimum)
{
	id_ = id;
	func_name_ = name;
	dimension_ = dim;
	lower_bound_ = l_bd;
	upper_bound_ = u_bd;
	global_optimum_ = optimum;
}

std::ostream & operator << (std::ostream &os, const CProblem &problem)
{
	os << "Problem's Info :" << std::endl;
	os << "Id : " << problem.id() << " / Name : " << problem.name() << std::endl;
	os << "Dim : " << problem.dim() << std::endl;
	os << "Global optimum : " << problem.global_optimum() << std::endl;
	os << "Search Range : [" << problem.lower_bound() << "," << problem.upper_bound() << "]" << std::endl;
	return os;
}