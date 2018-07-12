#ifndef DCNS_BSS_H
#define DCNS_BSS_H

#include <chrono>
#include <vector>

/*
Mishra, S., Saha, S., & Mondal, S. (2016, July). 
Divide and conquer based non-dominated sorting for parallel environment. 
In Evolutionary Computation (CEC), 2016 IEEE Congress on (pp. 4297-4304). IEEE.
*/

namespace NS {
	void DCNS(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	void DCNS_p(int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);

	class DCNS_Implementation {
	private:
		class Gamma { // Used to reduce the number of dominance comparions by taking constant space
		private:
			int frontIndex;
			int noSolution;
		public: 
			Gamma() : frontIndex(-1), noSolution(0) {}
			Gamma(int frontIndex, int noSolution) : frontIndex(frontIndex), noSolution(noSolution) {}
			int getFrontIndex() { return frontIndex; }
			int getNoSolution() { return noSolution; }
			void setFrontIndex(int frontIndex) { this->frontIndex = frontIndex; }
			void setNoSolution(int noSolution) { this->noSolution = noSolution; }
		};
		class Solution {
		private:
			int id;
			std::vector<double> objectives;
		public:
			Solution() {}
			Solution(int noObjectives) : objectives(noObjectives) {}
			Solution(const Solution& sol) : id(sol.id), objectives(sol.objectives) {}
			Solution(Solution&& sol) : id(sol.id), objectives(std::move(sol.objectives)) {}
			Solution(int id, const std::vector<double>& objectives) : id(id), objectives(objectives) {} 
			int getId() { return id; }
			const std::vector<double>& getObjectives() { return objectives; }
			double getObjective(int index) { return objectives[index]; }
			void setId(int id) { this->id = id; }
			void setObjectives(const std::vector<double>& objectives) { this->objectives = objectives; }
			void setObjective(double objective, int index) { objectives[index] = objective; }
			int noObjectives() { return objectives.size(); }
			/* 1: objectives dominates point
			-1: point dominates objectives
			0: objectives and point is non-dominting */
			int dominates(const Solution& sol) const {
				bool flag1 = false;
				bool flag2 = false;
				int noObjectives = sol.objectives.size();
				for (int i = 0; i < noObjectives; i++) {
					if (objectives[i] < sol.objectives[i]) {
						flag1 = true;
					}
					else if (objectives[i] > sol.objectives[i]) {
						flag2 = true;
					}
				}
				if (flag1 == true && flag2 == false) {
					return 1;
				}
				else if (flag1 == false && flag2 == true) {
					return -1;
				}
				else {
					/*--  but in this case non-dominting --*/
					return 0;
				}
			}
			/* Used for pre-sorting
			* 1: First solution is having small value for a objctive function
			-1: Second solution is having small value for a objctive function
			0: Same */
			int isSmall(Solution sol) {
				int noObjectives = sol.getObjectives().size();
				for (int i = 0; i < noObjectives; i++) {
					if (objectives[i] < sol.objectives[i]) {
						return 1;
					}
					else if (objectives[i] > sol.objectives[i]) {
						return -1;
					}
				}
				return 0;
			}
		};
		class HeapSort {
		public:
			void heapifyFirstObjective(std::vector<int>& arr, int heapSize, int i, std::vector<Solution>& population)
			{
				int largest = i;  // Initialize largest as root
				int l = 2 * i + 1;  // left = 2*i + 1
				int r = 2 * i + 2;  // right = 2*i + 2

									// If left child is larger than root
									/*if (l < heapSize && population[arr[l]].isBig(population[arr[largest]]) == 1)
									largest = l;*/
				if (l < heapSize && population[arr[l]].isSmall(population[arr[largest]]) == -1)
					largest = l;

				// If right child is larger than largest so far
				/*if (r < heapSize && population[arr[r]].isBig(population[arr[largest]]) == 1)
				largest = r;*/
				if (r < heapSize && population[arr[r]].isSmall(population[arr[largest]]) == -1)
					largest = r;

				// If largest is not root
				if (largest != i) {
					int swap = arr[i];
					arr[i] = arr[largest];
					arr[largest] = swap;

					// Recursively heapify the affected sub-tree
					heapifyFirstObjective(arr, heapSize, largest, population);
				}
			}
			void preSortDCNS(std::vector<Solution>& population, std::vector<int>& Q0) {
				int n = population.size();
				int noObjectives = population[0].noObjectives();
				for (int i = 0; i < n; i++) {
					Q0[i] = population[i].getId();
				}

				/*-- Sort based on first objective --*/
				// Build heap (rearrange array)
				for (int i = n / 2 - 1; i >= 0; i--)
					heapifyFirstObjective(Q0, n, i, population);

				// One by one extract an element from heap
				for (int i = n - 1; i >= 0; i--) {
					// Move current root to end
					int temp = Q0[0];
					Q0[0] = Q0[i];
					Q0[i] = temp;

					// call max heapify on the reduced heap
					heapifyFirstObjective(Q0, i, 0, population);
				}
				/*-- Sort based on first objective --*/
			}
		};
	public:
		DCNS_Implementation(const std::vector<std::vector<double>>& data);
		void sort(std::vector<int>& rank);
		void sort_p(int numTask, std::vector<int>& rank);
	private:
		void Merge_BSS(int i, int j);
		void parallel_Merge_BSS(const int i, const int n, const std::vector<int> && js);
		int Insert_Front_BSS(int i, int j, int q, int alpha);
		int Insert_BSS(int i, int sol, int P, int alpha, int hfi, Gamma& gamma);
	private:
		std::vector<Solution> population;
		HeapSort hs;
		std::vector<std::vector<std::vector<int>>> arrSetNonDominatedFront;
	};
}

#endif // !DCNS_BSS_H

