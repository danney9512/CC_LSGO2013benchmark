#include "CC_group.h"

Group::Group(const int index)
{
	x_index_.push_back(index);
	group_name_ = "";
}

Group::Group(std::vector<int>& index_arr)
{
	x_index_ = index_arr;
	group_name_ = "";
}
