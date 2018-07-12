#ifndef LINKSORT_H
#define LINKSORT_H
#include <vector>
#include "../../../core/definition.h"
#include "../list.h"
#include <chrono>

namespace NS {

	// Filter Sort Orignial Version
	int filter_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	// Filter Sort Modified Version 1 (without filter solution£©
	int filter_sort_m1(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	// Filter Sort Modified Version 2 (without worst negate£©
	int filter_sort_m2(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	// Filter Sort Parallel Computing Version
	int filter_sort_p(const int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	// Output the number of non-dominated solutions of solutions in the first PF
	namespace Filter_Sort {
		void output_num_cand(const std::vector<std::vector<double>>& data, const std::vector<int>& rank);
		// Function for Filter Sort Parallel Computing Version to check the validity of candidates
		void parallel_filter(const std::vector<int>&& candidates, const std::vector<List>& SeqByObj_Lists, const std::vector<int>& MaxIdxs, const std::vector<int>& MinIdxs, const int N, const std::vector<std::vector<int>>& SolStas, bool* InCurFront);
		// Function for Filter Sort Parallel Computing Version to quick sort
		void parallel_mergesort(const std::vector<std::vector<double>>& data, std::vector<std::vector<int>>& SeqByObj, const std::vector<int>&& ObjIdxs);
	}
}

#endif // !LINKSORT_H
