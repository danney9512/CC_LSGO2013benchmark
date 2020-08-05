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
	Group(const Group& g);
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
			bool add = true;
			for (int j = 0; j < x_index_.size(); j += 1)
			{
				if (rhs[i] == x_index_[j])
				{
					add = false;
					break;
				}
			}

			if(add)
				x_index_.push_back(rhs[i]);
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

	void setname(const std::string& name) { group_name_ = name; }
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
	GroupsResult() {}
	explicit GroupsResult(const GroupsResult& gp);

	const int problem_ID() const { return problem_id_; }
	const std::string& grouping_alg() const { return grouping_name_; }
	const unsigned long long int FFE_cost() const { return num_FE_cost_; }
	std::vector<Group>& nonsep_groups() { return nonsep_groups_; }
	std::vector<Group>& sep_groups() { return sep_groups_; }

	void setNumFE(const unsigned long long int FE) { num_FE_cost_ = FE; }
	void setProblemID(const int ID) { problem_id_ = ID; }
	void setGroupingAlgName(const std::string& g_name) { grouping_name_ = g_name; }

	void outputToFile(const std::string& grouping_algName)const;
	void inputResult(const std::string& grouping_algName, const int prob_ID);

private:
	std::vector<Group> nonsep_groups_;
	std::vector<Group> sep_groups_;

	int problem_id_ = 0;
	std::string grouping_name_;

	unsigned long long int num_FE_cost_ = 0;
};


#endif // !CC_GROUP__

