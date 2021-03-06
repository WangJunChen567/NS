//==================================================================
// This program was used in the following paper:
// H. Wang and X. Yao, ``Corner Sort for Pareto-based Many-Objective
// Optimization,'' IEEE Transactions on Cybernetics, 44(1):92-102, January
// 2014.
//
//==================================================================
#ifndef CONERSORT_H
#define CONERSORT_H

#include<malloc.h>
#include<stdlib.h>
#include <vector>
#include <chrono>
#include "../list.h"

namespace NS{
	class CornerSort {
		struct cs_node {
			unsigned int index;//index in POP
			cs_node *marknext;
			cs_node *markpre;
			cs_node *ranknext;
			cs_node *rankpre;
		};
	public:
		CornerSort(const std::vector<std::vector<double>>& data);
		~CornerSort() { delete[] POP; }
		//==================================================================
		//void  cornerSort(double **POP,unsigned int m,unsigned int n,unsigned int *rank)
		//POP-population  m-No. of Objectives  n-size of POP  rank-the ranking result as output
		void sort(std::vector<int>& rank, int& NumComp);
	private:
		//==================================================================
		//unsigned int min_obj(double **POP,unsigned int obj_index,node **head,node **mh)
		//POP-population??obj_index-objective index??head-linked list head?? mh-the head of the mark linked list
		//Output:cur-the index of individual with the minimal obj_index-th objective
		//==================================================================
		unsigned int min_obj(unsigned int obj_index, cs_node **head, cs_node **mh, int *comp, const unsigned int m, int& NumComp);
	private:
		const double ** POP = nullptr;
		int n, m;
		std::vector<int> mv_comp;
		int * comp;
	};

	void corner_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	//void corner_sort_p(const int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);

	namespace Corner_Sort {
		//void parallel_check(const size_t corner, const size_t M, const std::vector<std::vector<double>>& data, const std::vector<int> && idxs, std::vector<size_t>& remove);
	}
}
#endif