#ifndef T_ENS_H
#define T_ENS_H

#include <chrono>
#include <vector>

namespace NS {

	// return FrontNo
	void T_ENS(const std::vector<std::vector<double>>& Population, std::vector<int>& te_rank, std::pair<int, int>& measurement, int nSort = -1); 
}

#endif // !T_ENS_H

