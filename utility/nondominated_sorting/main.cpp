#include "fast_sort/fast_sort.h"
#include "corner_sort/corner_sort.h"
#include "T_ENS/T_ENS.h"
#include "filter_sort/filter_sort.h"
#include "link_sort/link_sort.h"
#include "ENS_NDT/ENS_NDT.h"
#include "deductive_sort/deductive_sort.h"
#include "best_order_sort/best_order_sort.h"
#include "benchmark/benchmark.h"

//*******************************************************************
// The test for comparing different Nondominated Sorting Algorithms
//*******************************************************************

int main() {
	std::vector<int> nums_sol({5000});
	std::vector<int> nums_obj({ 2,4,6,8 });
	std::vector<int> nums_fro({ 1,2,3,4,5,6,7,8 });
	const int num_run(5);

	std::ofstream outfile;

	//outfile.open("result/ComparisonBetweenAlgorithms/benchmark1.csv");
	//outfile << "num sol,num obj,Fast Sort,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,Filter Sort" << std::endl;
	//for (int num_sol : nums_sol) {
	//	for (int num_obj : nums_obj) {
	//		std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
	//		outfile << num_sol << "," << num_obj;
	//		NS::benchmark1 generator(num_sol, num_obj, 0.5);
	//		std::pair<int, int> meas_Fast_Sort({ 0,0 }), meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS({ 0,0 }), meas_ENS_NDT({ 0,0 }), meas_Filter_Sort({ 0,0 });
	//		std::vector<int> rank_Fast_Sort(num_sol), rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_Filter_Sort(num_sol);
	//		std::vector<std::vector<double>> data;
	//		for (int runID = 0; runID < num_run; ++runID) {
	//			generator.read_data(data, runID);
	//			NS::fast_sort(data, rank_Fast_Sort, meas_Fast_Sort);
	//			NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
	//			NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
	//			NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
	//			rank_ENS_NDT =  NS::ENS_NDT::Sort(data, meas_ENS_NDT);
	//			NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
	//		}
	//		outfile << "," << meas_Fast_Sort.first / num_run;
	//		outfile << "," << meas_Corner_Sort.first / num_run;
	//		outfile << "," << meas_Deductive_Sort.first / num_run;
	//		outfile << "," << meas_T_ENS.first / num_run;
	//		outfile << "," << meas_ENS_NDT.first / num_run;
	//		outfile << "," << meas_Filter_Sort.first / num_run;
	//		outfile << std::endl;
	//	}
	//}
	//outfile.close();

	outfile.open("result/ComparisonBetweenAlgorithms/impact_num_fro_2.csv");
	outfile << "num sol,num obj,num fro,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,BOS,Filter Sort" << std::endl;
	for (int num_sol : nums_sol) {
		for (int num_obj : nums_obj) {
			for (int num_fro : nums_fro) {
				std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
				outfile << num_sol << "," << num_obj << "," << num_fro;
				NS::benchmark3 generator(num_sol, num_obj, num_fro, 0.5);
				std::pair<int, int> meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS(0,0), meas_ENS_NDT(0,0),meas_BOS({0,0}), meas_Filter_Sort({ 0,0 });
				std::vector<int> rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_BOS(num_sol),rank_Filter_Sort(num_sol);
				std::vector<std::vector<double>> data;
				for (int runID = 0; runID < num_run; ++runID) {
					generator.read_data(data, runID);
					NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
					NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
					NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
					NS::ENS_NDT::Sort(data, meas_ENS_NDT);
					NS::best_order_sort(data, rank_BOS, meas_BOS);
					NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
				}
				outfile << "," << meas_Corner_Sort.first / num_run;
				outfile << "," << meas_Deductive_Sort.first / num_run;
				outfile << "," << meas_T_ENS.first / num_run;
				outfile << "," << meas_ENS_NDT.first / num_run;
				outfile << "," << meas_BOS.first / num_run;
				outfile << "," << meas_Filter_Sort.first / num_run;
				//std::cout << (rank_BOS == rank_Filter_Sort ? "Yes" : "No") << std::endl;
				outfile << std::endl;
			}
		}
	}
	outfile.close();

	//outfile.open("result/ComparisonBetweenAlgorithms/benchmark3.csv");
	//outfile << "num sol,num obj,num fro,Fast Sort,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,Filter Sort" << std::endl;
	//for (int num_sol : nums_sol) {
	//	for (int num_obj : nums_obj) {
	//		for (int num_fro : nums_fro) {
	//			std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
	//			outfile << num_sol << "," << num_obj << "," << num_fro;
	//			NS::benchmark3 generator(num_sol, num_obj, num_fro, 0.5);
	//			std::pair<int, int> meas_Fast_Sort({ 0,0 }), meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS({ 0,0 }), meas_ENS_NDT({ 0,0 }), meas_Filter_Sort({ 0,0 });
	//			std::vector<int> rank_Fast_Sort(num_sol), rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_Filter_Sort(num_sol);
	//			std::vector<std::vector<double>> data;
	//			for (int runID = 0; runID < num_run; ++runID) {
	//				generator.read_data(data, runID);
	//				NS::fast_sort(data, rank_Fast_Sort, meas_Fast_Sort);
	//				NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
	//				NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
	//				NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
	//				rank_ENS_NDT = NS::ENS_NDT::Sort(data, meas_ENS_NDT);
	//				NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
	//			}
	//			outfile << "," << meas_Fast_Sort.first / num_run;
	//			outfile << "," << meas_Corner_Sort.first / num_run;
	//			outfile << "," << meas_Deductive_Sort.first / num_run;
	//			outfile << "," << meas_T_ENS.first / num_run;
	//			outfile << "," << meas_ENS_NDT.first / num_run;
	//			outfile << "," << meas_Filter_Sort.first / num_run;
	//			outfile << std::endl;
	//		}
	//	}
	//}
	//outfile.close();

	return 0;
}

//***********************************
// generate benchmark data set
//***********************************

//int main() {
//	std::vector<int> nums_sol({ 5000,10000,15000,20000 });
//	std::vector<int> nums_obj({ 2,3,4,5,6,7,8 });
//	std::vector<int> nums_fro({ 1,2,3,4,5,6,7,8 });
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
//			NS::benchmark1 generator(num_sol, num_obj, 0.5);
//			generator.output_data_set(50);
//		}
//	}
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			for (int num_fro : nums_fro) {
//				std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
//				NS::benchmark2 generator1(num_sol, num_obj, num_fro, 0.5);
//				generator1.output_data_set(50);
//				std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
//				NS::benchmark3 generator2(num_sol, num_obj, num_fro, 0.5);
//				generator2.output_data_set(50);
//			}
//		}
//	}
//	return 0;
//}