#ifndef FAST_NONDOMINATED_SORT_H
#define FAST_NONDOMINATED_SORT_H

#include<map>
#include <list>
#include <vector>
#include "../../functional.h"
#include <chrono>

namespace NS {
	int fast_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);

	int fast_sort_p(int numTask,  const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);

	void ParallelCompare(int popsize, const std::vector<int>&& ks, const std::vector<std::vector<double>>& data, std::vector<int>& rank_, std::vector<int>& count, std::vector<std::vector<int>>& cset);
}




#endif // !FAST_NONDOMINATED_SORT_H