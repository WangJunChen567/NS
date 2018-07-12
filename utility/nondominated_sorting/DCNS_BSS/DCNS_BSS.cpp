#include "DCNS_BSS.h"
#include <thread>
#include <math.h>

namespace NS {
	void DCNS(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		DCNS_Implementation dcns(data);
		
		Total_start_time = std::chrono::system_clock::now();
		dcns.sort(rank);
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();

		measurement.first += time;
		measurement.second += NumComp;
	}

	void DCNS_p(int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		DCNS_Implementation dcns(data);

		Total_start_time = std::chrono::system_clock::now();
		dcns.sort_p(numTask, rank);
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();

		measurement.first += time;
		measurement.second += NumComp;
	}

	DCNS_Implementation::DCNS_Implementation(const std::vector<std::vector<double>>& data) {
		int n = data.size();
		for (size_t i = 0; i < n; i++) {
			population.push_back(Solution(i, data[i]));
		}
	}

	void DCNS_Implementation::sort(std::vector<int>& rank) {
		int n = population.size();
		int noObjectives = population[0].noObjectives();
		std::vector<int> Q0(n);
		hs.preSortDCNS(population, Q0);
		arrSetNonDominatedFront.resize(n);
		for (int i = 0; i < n; i++) {
			arrSetNonDominatedFront[i].push_back(std::vector<int>({ Q0[i] }));
		}
		int treeLevel = (int)ceil(log10(n) / log10(2));
		for (int i = 0; i < treeLevel; i++) { //treeLevel
			int x = (int)ceil(n / (pow(2, i + 1)));
			for (int j = 0; j < x; j++) {
				int a = (int)pow(2, i + 1)*j;
				int b = a + (int)pow(2, i);
				if (b < n) {
					Merge_BSS(a, b);
				}
			}
		}
		if (rank.size() != n) rank.resize(n);
		for (size_t i = 0; i < arrSetNonDominatedFront[0].size(); i++) {
			for (int index : arrSetNonDominatedFront[0][i]) {
				rank[index] = i;
			}
		}
	}

	void DCNS_Implementation::sort_p(int numTask, std::vector<int>& rank) {
		int n = population.size();
		int noObjectives = population[0].noObjectives();
		std::vector<int> Q0(n);
		hs.preSortDCNS(population, Q0);
		arrSetNonDominatedFront.resize(n);
		for (int i = 0; i < n; i++) {
			arrSetNonDominatedFront[i].push_back(std::vector<int>({ Q0[i] }));
		}
		int treeLevel = (int)ceil(log10(n) / log10(2));
		for (int i = 0; i < treeLevel; i++) { //treeLevel
			int x = (int)ceil(n / (pow(2, i + 1)));
			int TaskSize = x;
			if (numTask > TaskSize) numTask = TaskSize;
			std::vector<std::thread> thrd;
			for (int i_ = 0; i_ < numTask; ++i_) {
				std::vector<int> js;
				for (int j = i_; j < TaskSize; j += numTask)
					js.push_back(j);
				thrd.push_back(std::thread(&DCNS_Implementation::parallel_Merge_BSS, this, i, n, std::move(js)));
			}
			for (auto&t : thrd) t.join();
		}
		if (rank.size() != n) rank.resize(n);
		for (size_t i = 0; i < arrSetNonDominatedFront[0].size(); i++) {
			for (int index : arrSetNonDominatedFront[0][i]) {
				rank[index] = i;
			}
		}
	}

	void DCNS_Implementation::parallel_Merge_BSS(const int i, const int n, const std::vector<int> && js) {
		for (int j : js) {
			int a = (int)pow(2, i + 1)*j;
			int b = a + (int)pow(2, i);
			if (b < n) {
				Merge_BSS(a, b);
			}
		}
	}

	void DCNS_Implementation::Merge_BSS(int i, int j) {
		int alpha = -1;
		int q = 0;
		while (q < arrSetNonDominatedFront[j].size()) {
			alpha = Insert_Front_BSS(i, j, q, alpha + 1);
			if (alpha == arrSetNonDominatedFront[i].size() - 1) {
				q++;
				break;
			}
			q++;
		}
		/* Add remaining fronts without comparisons */
		while (q < arrSetNonDominatedFront[j].size()) {
			arrSetNonDominatedFront[i].
				push_back(arrSetNonDominatedFront[j][q]);
			q++;
		}
	}

	int DCNS_Implementation::Insert_Front_BSS(int i, int j, int q, int alpha) {
		int P(arrSetNonDominatedFront[i].size());
		int hfi(P);  //Because indexing starts from 0 in Java
		Gamma gamma(-1, -1);
		for (int sol : arrSetNonDominatedFront[j][q]) {
			hfi = Insert_BSS(i, sol, P, alpha, hfi, gamma);
		}
		return hfi;
	}

	int DCNS_Implementation::Insert_BSS(int i, int sol, int P, int alpha, int hfi, Gamma& gamma) {
		bool isInserted = false;
		int min = alpha;
		int max = P - 1;
		int mid = (min + max) / 2;
		int count;
		while (true) {
			count = 0;
			int sizeOfFront = arrSetNonDominatedFront[i][mid].size();
			if (mid == gamma.getFrontIndex()) {
				sizeOfFront = gamma.getNoSolution();
			}
			for (int u = sizeOfFront - 1; u >= 0; u--) {
				int isdom = population[arrSetNonDominatedFront[i][mid][u]].dominates(population[sol]);
				if (isdom == 0) { //solution sol is non-dominated with the solution in the existing front
					count++;
				}
				else if (isdom == 1) { //solution sol is dominated by the solution in the existing front
					break;
				}
			}
			if (count == sizeOfFront) {
				if (mid == min) {
					arrSetNonDominatedFront[i][mid].push_back(sol);
					if (mid != gamma.getFrontIndex()) {
						gamma.setFrontIndex(mid);
						gamma.setNoSolution(count);
					}
					isInserted = true;
					if (mid < hfi) {
						hfi = mid;
					}
					break;
				}
				else {
					max = mid;
					mid = (min + max) / 2;
				}
			}
			else {
				if (min == P - 1) {
					if (arrSetNonDominatedFront[i].size() == P) {
						arrSetNonDominatedFront[i].push_back(std::vector<int>({ sol }));
					}
					else {
						arrSetNonDominatedFront[i][P].push_back(sol);
					}
					break;
				}
				else {
					min = mid + 1;
					mid = (min + max) / 2;
				}
			}
		}	
		return hfi;
	}
}