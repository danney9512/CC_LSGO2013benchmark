#ifndef CC_GROUP__
#define CC_GROUP__

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Group
{
public:
	Group(){}
	explicit Group(const int index);
	explicit Group(std::vector<int>& index_arr);

	const std::string& name()const { return group_name_; }
	const std::vector<int>& vec()const { return x_index_; }
	std::vector<int>& vec() { return x_index_; }
	const size_t size()const { return x_index_.size(); }
	int& operator [] (size_t i) { return x_index_[i]; }
	const int operator [] (size_t i)const { return x_index_[i]; }
	bool operator == (const Group& g) const
	{
		if (g.size() != x_index_.size())
			return false;

		sort(g.vec().begin(), g.vec().end());
		sort(x_index_.begin(), x_index_.end());

		for (size_t i = 0; i < g.size(); i += 1)
		{
			if (g[i] != x_index_[i])
				return false;
		}
		return true;
	}
	Group& operator += (const Group& rhs)
	{
		for (size_t i = 0; i < rhs.size(); i += 1)
		{
			x_index_.push_back(rhs[0]);
		}
		sort(x_index_.begin(), x_index_.end());
		return *this;
	}
	const Group operator + (const Group& rhs) const
	{
		Group new_group = *this;
		new_group += rhs;
		return new_group;
	}

	void setname(std::string& name) { group_name_ = name; }
	void push_back(const int& index) { x_index_.push_back(index); }
	bool empty() const { return x_index_.size() == 0; }
	void clear() { x_index_.clear(); group_name_ = ""; }

private:
	std::string group_name_;
	std::vector<int> x_index_;
};

class GroupsResult
{
public:
	const int problem_ID() const { return problem_id_; }
	const std::string& grouping_alg() const { return grouping_name; }
	std::vector<Group>& nonsep_groups() { return nonsep_groups_; }
	std::vector<Group>& sep_groups() { return sep_groups_; }

	void setProblemID(const int ID) { problem_id_ = ID; }
	void setGroupingAlgName(const std::string& g_name) { grouping_name = g_name; }

private:
	std::vector<Group> nonsep_groups_;
	std::vector<Group> sep_groups_;

	int problem_id_ = 0;
	std::string grouping_name;
};


#endif // !CC_GROUP__

