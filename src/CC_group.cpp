#include "CC_group.h"
#include "self_implement_functions.h"

#include <fstream>
#include <filesystem>

Group::Group(const Group& g)
{
	x_index_ = g.x_index_;
	group_name_ = g.group_name_;
}

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


GroupsResult::GroupsResult(const GroupsResult& gp)
{
	nonsep_groups_ = gp.nonsep_groups_;
	sep_groups_ = gp.sep_groups_;
	problem_id_ = gp.problem_id_;
	grouping_name_ = gp.grouping_name_;
	num_FE_cost_ = gp.num_FE_cost_;
}

void GroupsResult::outputToFile(const std::string& grouping_algName)const
{
	// file path setting
	std::string filePath = "experiments/grouping/GroupingResults_" + grouping_algName + "/";
	if (!(std::filesystem::exists(filePath)))
	{
		std::filesystem::create_directory(filePath);
	}

	// Output the grouping result data of a problem
	std::string fileName = "GroupingResult_" + grouping_name_ + "_" + IntToStr(problem_id_, 2) + ".txt";
	std::fstream rootFile(filePath + fileName, std::ios::out | std::ios::trunc);

	rootFile << "Grouping_Algorithm = " << grouping_name_ << std::endl;
	rootFile << "Probem_ID = " << problem_id_ << std::endl;
	rootFile << "Number_FE_cost = " << num_FE_cost_ << std::endl;
	rootFile << "Sep-Group_Size = " << sep_groups_.size() << std::endl;
	rootFile << "NonSep-Group_Size = " << nonsep_groups_.size() << std::endl;

	// Output sep-groups
	std::string sepGroupsFileName = "GroupingResult_" + grouping_name_ + "_" + IntToStr(problem_id_, 2) + "_sepGroups" + ".txt";
	std::fstream sepGroupsFile(filePath + sepGroupsFileName, std::ios::out | std::ios::trunc);

	for (size_t i = 0; i < sep_groups_.size(); ++i)
	{
		sepGroupsFile << sep_groups_[i].size() << std::endl;
		for (size_t j = 0; j < sep_groups_[i].size(); ++j)
		{
			sepGroupsFile << sep_groups_[i][j] << " ";
		}
		sepGroupsFile << std::endl;
	}

	// Output nonsep-groups
	std::string nonsepGroupsFileName = "GroupingResult_" + grouping_name_ + "_" + IntToStr(problem_id_, 2) + "_nonsepGroups" + ".txt";
	std::fstream nonsepGroupsFile(filePath + nonsepGroupsFileName, std::ios::out | std::ios::trunc);

	for (size_t i = 0; i < nonsep_groups_.size(); ++i)
	{
		nonsepGroupsFile << nonsep_groups_[i].size() << std::endl;
		for (size_t j = 0; j < nonsep_groups_[i].size(); ++j)
		{
			nonsepGroupsFile << nonsep_groups_[i][j] << " ";
		}
		nonsepGroupsFile << std::endl;
	}

	// close all output files
	rootFile.close();
	sepGroupsFile.close();
	nonsepGroupsFile.close();
}

void GroupsResult::inputResult(const std::string& grouping_algName, const int prob_ID)
{
	std::string dummy, groupname;
	size_t nonsep_size = 0, sep_size = 0;

	// file path setting
	std::string filePath = "experiments/grouping/GroupingResults_" + grouping_algName + "/";

	// Input rootfile data
	std::string fileName = "GroupingResult_" + grouping_algName + "_" + IntToStr(prob_ID, 2) + ".txt";
	std::fstream rootFile(filePath + fileName, std::ios::in);

	if (!rootFile)
	{
		std::cout << "Error msg : Can't load the file '" << grouping_algName << "'." << std::endl;
	}
	else
	{
		rootFile >> dummy >> dummy >> grouping_name_;
		rootFile >> dummy >> dummy >> problem_id_;
		rootFile >> dummy >> dummy >> num_FE_cost_;
		rootFile >> dummy >> dummy >> sep_size;
		rootFile >> dummy >> dummy >> nonsep_size;
	}

	// Input sep-groups file data
	std::string sepGroupsFileName = "GroupingResult_" + grouping_name_ + "_" + IntToStr(problem_id_, 2) + "_sepGroups" + ".txt";
	std::fstream sepGroupsFile(filePath + sepGroupsFileName, std::ios::in);

	if (!sepGroupsFile)
	{
		std::cout << "Error msg : Can't load the file '" << sepGroupsFileName << "'." << std::endl;
	}
	else if (sep_size > 0)
	{
		size_t num = 0;
		groupname = "separable";
		for (size_t i = 0; i < sep_size; ++i)
		{
			sepGroupsFile >> num;
			std::vector<int> x_index(num, 0);
			for (size_t j = 0; j < num; ++j)
			{
				sepGroupsFile >> x_index[j];
			}

			Group g(x_index);
			g.setname(groupname);

			sep_groups_.push_back(g);
		}
	}

	// Input nonsep-groups file data
	std::string nonsepGroupsFileName = "GroupingResult_" + grouping_name_ + "_" + IntToStr(problem_id_, 2) + "_nonsepGroups" + ".txt";
	std::fstream nonsepGroupsFile(filePath + nonsepGroupsFileName, std::ios::in);

	if (!nonsepGroupsFile)
	{
		std::cout << "Error msg : Can't load the file '" << nonsepGroupsFileName << "'." << std::endl;
	}
	else if (nonsep_size > 0)
	{
		size_t num = 0;
		groupname = "nonseparable";
		for (size_t i = 0; i < nonsep_size; ++i)
		{
			nonsepGroupsFile >> num;
			std::vector<int> x_index(num, 0);
			for (size_t j = 0; j < num; ++j)
			{
				nonsepGroupsFile >> x_index[j];
			}

			Group g(x_index);
			g.setname(groupname);

			nonsep_groups_.push_back(g);
		}
	}

	rootFile.close();
	sepGroupsFile.close();
	nonsepGroupsFile.close();
}