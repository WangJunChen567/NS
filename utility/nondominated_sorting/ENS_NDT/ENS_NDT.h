#ifndef ENS_NDT_H
#define ENS_NDT_H

/*
Gustavsson, P., & Syberfeldt, A. (2018).
A new algorithm using the non-dominated tree to improve non-dominated sorting.
Evolutionary computation, 26(1), 89-116.
*/

#include "NDTree.h"

using namespace ENS_NDT;

namespace NS {
	int ENS_NDT_sort(const std::vector<std::vector<double>>& individuals, std::vector<int>& rank, std::pair<int, int>& measurement);
	class ENS_NDT {
	public:
		static int NondominatedSort(const std::vector<std::vector<double>>& P, int k, int& NumComp, std::vector<int>& rank);
	private:
		static int FrontIndexBinarySearch(std::vector<double>* s, std::vector<std::unique_ptr<NDTree>>& NDT, int& NumComp);
		static bool FitnessEquals(std::vector<double>& a, std::vector<double>& b, int k, int& NumComp);
	};
}

#endif // !ENS_NDT_H

