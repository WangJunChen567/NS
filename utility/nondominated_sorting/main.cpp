#include "fast_sort/fast_sort.h"
#include "corner_sort/corner_sort.h"
#include "T_ENS/T_ENS.h"
#include "filter_sort/filter_sort.h"
#include "link_sort/link_sort.h"
#include "ENS_NDT/ENS_NDT.h"
#include "deductive_sort/deductive_sort.h"
#include "best_order_sort/best_order_sort.h"
#include "DCNS_BSS/DCNS_BSS.h"
#include "benchmark/benchmark.h"

#include "merge_sort.h"
#include <chrono>

//*******************************************************************
// The test for comparing different Nondominated Sorting Algorithms
//*******************************************************************

//int main() {
//	std::vector<int> nums_rank({ 1,2,3,4,5,6,7,8,9,10 });
//	std::vector<int> nums_sol({ 10000 });
//	std::vector<int> nums_obj({ 3,6,9 });
//	const int num_run(5);
//
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		for (int num_rank : nums_rank) {
//	//			std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//	//			NS::benchmark2 generator(num_sol, num_obj, num_rank, 0.5);
//	//			generator.output_data_set(num_run);
//	//		}
//	//	}
//	//}
//
//	std::ofstream outfile;
//
//	//outfile.open("result/ComparisonBetweenAlgorithms/impact_num_sol.csv");
//	//outfile << "num sol,num obj,num fro,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,BOS,Filter Sort" << std::endl;
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
//	//		outfile << num_sol << "," << num_obj;
//	//		NS::benchmark1 generator(num_sol, num_obj, 0.5);
//	//		std::pair<int, int> meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS(0, 0), meas_ENS_NDT(0, 0), meas_BOS({ 0,0 }), meas_Filter_Sort({ 0,0 });
//	//		std::vector<int> rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_BOS(num_sol), rank_Filter_Sort(num_sol);
//	//		std::vector<std::vector<double>> data;
//	//		int num_rank(0);
//	//		for (int runID = 0; runID < num_run; ++runID) {
//	//			generator.read_data(data, runID);
//	//			NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
//	//			NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
//	//			NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
//	//			NS::ENS_NDT_sort(data, rank_ENS_NDT, meas_ENS_NDT);
//	//			num_rank += NS::best_order_sort(data, rank_BOS, meas_BOS);
//	//			NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
//	//		}
//	//		//outfile << "," << num_rank / num_run;
//	//		outfile << "," << meas_Corner_Sort.first / num_run;
//	//		outfile << "," << meas_Deductive_Sort.first / num_run;
//	//		outfile << "," << meas_T_ENS.first / num_run;
//	//		outfile << "," << meas_ENS_NDT.first / num_run;
//	//		outfile << "," << meas_BOS.first / num_run;
//	//		outfile << "," << meas_Filter_Sort.first / num_run;
//	//		outfile << std::endl;
//	//	}
//	//}
//	//outfile.close();
//
//	outfile.open("result/ComparisonBetweenAlgorithms/poster_simulation_obj_linux.csv");
//	outfile << "num sol,num obj,num rank,Deductive Sort,Corner Sort,T-ENS,Filter Sort" << std::endl;
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			for (int num_rank : nums_rank) {
//				std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//				outfile << num_sol << "," << num_obj << "," << num_rank;
//				NS::benchmark2 generator(num_sol, num_obj, num_rank, 0.5);
//				std::pair<int, int> meas_CS(0, 0), meas_DS(0, 0), meas_TE(0, 0), meas_FS(0, 0);
//				std::vector<int> rank_CS(num_sol), rank_DS(num_sol), rank_TE(num_sol), rank_FS(num_sol);
//				std::vector<std::vector<double>> data;
//				for (int runID = 0; runID < num_run; ++runID) {
//					generator.read_data(data, runID);
//					NS::corner_sort(data, rank_CS, meas_CS);
//					NS::deductive_sort(data, rank_DS, meas_DS);
//					NS::T_ENS(data, rank_TE, meas_TE);
//					NS::filter_sort(data, rank_FS, meas_FS);
//					//std::cout << (rank_T_ENS == rank_ENS_NDT ? "Yes" : "No") << std::endl;
//				}
//				outfile << "," << meas_DS.first / num_run;
//				outfile << "," << meas_CS.first / num_run;
//				outfile << "," << meas_TE.first / num_run;
//				outfile << "," << meas_FS.first / num_run;
//				outfile << std::endl;
//			}
//		}
//	}
//
//	outfile.close();
//
//	return 0;
//}

//*********************************************************************************
// The test for comparing two modified Filter Sort algorithms with the orginal one
//*********************************************************************************

//int main() {
//	std::vector<int> nums_sol({ 10000});
//	std::vector<int> nums_obj({ 2,4,6,8});
//	std::vector<int> nums_rank({ 1,3,5,7 });
//	const int num_run(30);
//
//	std::ofstream outfile;
//	
//	//outfile.open("result/ComparisonBetweenInnerParts/benchmark1_parallel.csv");
//	//outfile << "num sol,num obj,num fro,original,parallel,BOS" << std::endl;
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
//	//		outfile << num_sol << "," << num_obj;
//	//		std::vector<int> rank_original(num_sol), rank_2p(num_sol), rank_4p(num_sol), rank_8p(num_sol), rank_BOS(num_sol);
//	//		std::pair<int, int> meas_original(0, 0), meas_2p(0, 0), meas_4p(0, 0), meas_8p(0, 0), meas_BOS(0, 0);
//	//		std::vector<std::vector<double>> data;
//	//		int num_rank(0);
//	//		for (int runID = 0; runID < num_run; ++runID) {
//	//			NS::benchmark1 generator(num_sol, num_obj, 0.5);
//	//			generator.read_data(data, runID);
//	//			NS::filter_sort(data, rank_original, meas_original);
//	//			num_rank += NS::filter_sort_p(data, rank_2p, meas_2p);
//	//			NS::best_order_sort(data, rank_BOS, meas_BOS);
//	//			//std::cout << (rank_original == rank_modified1 ? "yes" : "no") << std::endl;
//	//		}
//	//		outfile << "," << num_rank / num_run;
//	//		outfile << "," << meas_original.first / num_run;
//	//		outfile << "," << meas_2p.first / num_run;
//	//		outfile << "," << meas_BOS.first / num_run;
//	//		outfile << std::endl;
//	//	}
//	//}
//
//	outfile.open("result/ComparisonBetweenInnerParts/benchmark2_parallel_speedup.csv");
//	outfile << "num sol,num obj,num fro,Filter Sort,Filter Sort(2 thread),Filter Sort(4 thread),Filter Sort(8 thread),Filter Sort(12 thread),Filter Sort(16 thread)" << std::endl;
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			for (int num_rank : nums_rank) {
//				std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//				outfile << num_sol << "," << num_obj << "," << num_rank;
//				NS::benchmark2 generator(num_sol, num_obj, num_rank, 0.5);
//				std::vector<int> rank_ENS_NDT(num_sol),rank_BOS(num_sol),rank_original(num_sol), rank_2p(num_sol), rank_4p(num_sol), rank_8p(num_sol), rank_12p(num_sol), rank_16p(num_sol);
//				std::pair<int, int> meas_ENS_NDT(0,0),meas_BOS(0, 0),meas_original(0, 0), meas_2p(0, 0), meas_4p(0, 0), meas_8p(0, 0), meas_12p(0,0),meas_16p(0,0);
//				std::vector<std::vector<double>> data;
//				for (int runID = 0; runID < num_run; ++runID) {
//					generator.read_data(data, runID);
//					//NS::ENS_NDT_sort(data, rank_ENS_NDT, meas_ENS_NDT);
//					//NS::best_order_sort(data, rank_BOS, meas_BOS);
//					NS::filter_sort(data, rank_original, meas_original);
//					NS::filter_sort_p(2,data, rank_2p, meas_2p);
//					NS::filter_sort_p(4,data, rank_4p, meas_4p);
//					NS::filter_sort_p(8,data, rank_8p, meas_8p);
//					NS::filter_sort_p(12, data, rank_12p, meas_12p);
//					NS::filter_sort_p(16, data, rank_16p, meas_16p);
//				}
//				//outfile << "," << meas_ENS_NDT.first / num_run;
//				//outfile << "," << meas_BOS.first / num_run;
//				outfile << "," << meas_original.first / num_run;
//				outfile << "," << meas_2p.first / num_run;
//				outfile << "," << meas_4p.first / num_run;
//				outfile << "," << meas_8p.first / num_run;
//				outfile << "," << meas_12p.first / num_run;
//				outfile << "," << meas_16p.first / num_run;
//				outfile << std::endl;
//			}
//		}
//	}
//	outfile.close();
//
//	//outfile.open("result/ComparisonBetweenInnerParts/benchmark3_modified1.csv");
//	//outfile << "num sol,num obj,num fro,original,modified1" << std::endl;
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		for (int num_rank : nums_rank) {
//	//			std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//	//			outfile << num_sol << "," << num_obj << "," << num_rank;
//	//			NS::benchmark3 generator(num_sol, num_obj, num_rank, 0.5);
//	//			std::vector<int> rank_original(num_sol), rank_modified1(num_sol);
//	//			std::pair<int, int> meas_original(0, 0), meas_modified1(0, 0);
//	//			std::vector<std::vector<double>> data;
//	//			for (int runID = 0; runID < num_run; ++runID) {
//	//				generator.read_data(data, runID);
//	//				NS::filter_sort(data, rank_original, meas_original);
//	//				NS::filter_sort_m1(data, rank_modified1, meas_modified1);
//	//			}
//	//			outfile << "," << meas_original.first / num_run;
//	//			outfile << "," << meas_modified1.first / num_run;
//	//			outfile << std::endl;
//	//		}
//	//	}
//	//}
//	//outfile.close();
//
//	return 0;
//}

//***********************************
// generate benchmark data set
//***********************************

//int main() {
//	std::vector<int> nums_sol({ 10000 });
//	std::vector<int> nums_obj({ 2,3,4,6,10,15,20,30,50,100 });
//	std::vector<int> nums_rank({ 10 });
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
//			NS::benchmark1 generator(num_sol, num_obj, 0.5);
//			generator.output_data_set(30);
//		}
//	}
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		for (int num_rank : nums_rank) {
//	//			std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//	//			NS::benchmark2 generator1(num_sol, num_obj, num_rank, 0.5);
//	//			generator1.output_data_set(30);
//	//			std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_rank << std::endl;
//	//			NS::benchmark3 generator2(num_sol, num_obj, num_rank, 0.5);
//	//			generator2.output_data_set(1);
//	//		}
//	//	}
//	//}
//	return 0;
//}

//***********************************
// generate seq_sum and num_cand
//***********************************

//int main() {
//	std::ofstream outfile;
//	outfile.open("result/num_cand_2obj_concave.csv");
//	int num_obj = 2;
//	int num_rank = 5;
//	NS::benchmark2 generator(5000, num_obj, num_rank, 0.5);
//	std::vector<std::vector<double>> data;
//	std::vector<int> rank(5000);
//	std::pair<int, int> meas(0, 0);
//	int eta_c(0), eta_f(0);
//	for (int i = 0; i < 30; ++i) {
//		generator.read_data(data, i);
//		eta_c += NS::filter_sort_m1(data, rank, meas);
//		eta_f += NS::filter_sort(data, rank, meas);
//	}
//	outfile << num_obj << "," << num_rank << "," << (double)(eta_c / 30) / 50 << "," << (double)(eta_f / 30) / 50 << std::endl;
//}

int main() {
	NS::benchmark2 generator(10000,3,8,0.5);
	generator.update_data();
	std::vector<std::vector<double>> data(generator.get_data());
	std::vector<int> rank, rank_1;
	std::pair<int, int> meas({ 0,0 }), meas_1({ 0,0 });
	int numTask(1);
	NS::filter_sort(data, rank, meas);
	NS::filter_sort_p(numTask, data, rank_1, meas_1);
	std::cout << (rank == rank_1 ? "yes" : "no") << std::endl;
	std::cout << "Filter Sort:\t" << meas.first << std::endl;
	std::cout << "Comparison:\t" << meas_1.first << std::endl;
	return 0;
}