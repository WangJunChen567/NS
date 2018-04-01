#include "fast_sort/fast_sort.h"
#include "corner_sort/corner_sort.h"
#include "T_ENS/T_ENS.h"
#include "filter_sort/filter_sort.h"
#include "link_sort/link_sort.h"
#include "ENS_NDT/ENS_NDT.h"
#include "deductive_sort/deductive_sort.h"
#include "benchmark/benchmark.h"

//#define USING_MULTI_THREAD

#ifdef USING_MULTI_THREAD
#include <thread>
#endif // USING_MULTI_THREAD


//*******************************************************************
// The test for comparing different Nondominated Sorting Algorithms
//*******************************************************************

int main() {
	std::vector<int> nums_sol({ 5000,10000,15000,20000 });
	std::vector<int> nums_obj({ 2,3,4,5,6,7,8 });
	std::vector<int> nums_fro({ 1,2,3,4,5,6,7,8 });
	const int num_run(50);

	std::ofstream outfile;
#ifdef USING_MULTI_THREAD
	outfile.open("result/ComparisonBetweenAlgorithms/benchmark1_multi_thread.csv");
#else
	outfile.open("result/ComparisonBetweenAlgorithms/benchmark1.csv");
#endif USING_MULTI_THREAD

	outfile << "num sol,num obj,Fast Sort,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,Filter Sort" << std::endl;
	for (int num_sol : nums_sol) {
		for (int num_obj : nums_obj) {
			std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
			outfile << num_sol << "," << num_obj;
			NS::benchmark1 generator(num_sol, num_obj, 0.5);
			std::pair<int, int> meas_Fast_Sort({ 0,0 }), meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS({ 0,0 }), meas_ENS_NDT({ 0,0 }), meas_Filter_Sort({ 0,0 });
			std::vector<int> rank_Fast_Sort(num_sol), rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_Filter_Sort(num_sol);


#ifdef USING_MULTI_THREAD
			std::vector<std::vector<std::vector<double>>> datas(num_run);
			for (int i = 0; i < num_run; ++i)
				generator.read_data(datas[i], i);
			std::vector<std::thread> thrd;

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j+1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::fast_sort, std::cref(datas[i]), std::ref(rank_Fast_Sort), std::ref(meas_Fast_Sort)));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j + 1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::corner_sort, std::cref(datas[i]), std::ref(rank_Corner_Sort), std::ref(meas_Corner_Sort)));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j + 1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::deductive_sort, std::cref(datas[i]), std::ref(rank_Deductive_Sort), std::ref(meas_Deductive_Sort)));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j + 1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::T_ENS, std::cref(datas[i]), std::ref(rank_T_ENS), std::ref(meas_T_ENS),-1));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j + 1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::ENS_NDT::Sort, std::cref(datas[i]), std::ref(meas_ENS_NDT)));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}

			for (int j = 0; j < 2; ++j) {
				for (int i = j * (num_run / 2); i < (j + 1)*num_run / 2; ++i) {
					thrd.push_back(std::thread(NS::filter_sort, std::cref(datas[i]), std::ref(rank_Filter_Sort), std::ref(meas_Filter_Sort)));
				}
				for (auto& t : thrd)
					t.join();
				thrd.clear();
			}
#else
			std::vector<std::vector<double>> data;
			for (int runID = 0; runID < num_run; ++runID) {
				generator.read_data(data, runID);
				NS::fast_sort(data, rank_Fast_Sort, meas_Fast_Sort);
				NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
				NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
				NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
				rank_ENS_NDT =  NS::ENS_NDT::Sort(data, meas_ENS_NDT);
				NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
			}
#endif // USING_MULTI_THREAD

			outfile << "," << meas_Fast_Sort.first / num_run;
			outfile << "," << meas_Corner_Sort.first / num_run;
			outfile << "," << meas_Deductive_Sort.first / num_run;
			outfile << "," << meas_T_ENS.first / num_run;
			outfile << "," << meas_ENS_NDT.first / num_run;
			outfile << "," << meas_Filter_Sort.first / num_run;
			outfile << std::endl;
		}
	}
	outfile.close();

	outfile.open("result/ComparisonBetweenAlgorithms/benchmark2.csv");
	outfile << "num sol,num obj,num fro,Fast Sort,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,Filter Sort" << std::endl;
	for (int num_sol : nums_sol) {
		for (int num_obj : nums_obj) {
			for (int num_fro : nums_fro) {
				std::cout << "benchmark2_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
				outfile << num_sol << "," << num_obj << "," << num_fro;
				NS::benchmark2 generator(num_sol, num_obj, num_fro, 0.5);
				std::pair<int, int> meas_Fast_Sort({ 0,0 }), meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS({ 0,0 }), meas_ENS_NDT({ 0,0 }), meas_Filter_Sort({ 0,0 });
				std::vector<int> rank_Fast_Sort(num_sol), rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_Filter_Sort(num_sol);
				std::vector<std::vector<double>> data;
				for (int runID = 0; runID < num_run; ++runID) {
					generator.read_data(data, runID);
					NS::fast_sort(data, rank_Fast_Sort, meas_Fast_Sort);
					NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
					NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
					NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
					rank_ENS_NDT = NS::ENS_NDT::Sort(data, meas_ENS_NDT);
					NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
				}
				outfile << "," << meas_Fast_Sort.first / num_run;
				outfile << "," << meas_Corner_Sort.first / num_run;
				outfile << "," << meas_Deductive_Sort.first / num_run;
				outfile << "," << meas_T_ENS.first / num_run;
				outfile << "," << meas_ENS_NDT.first / num_run;
				outfile << "," << meas_Filter_Sort.first / num_run;
				outfile << std::endl;
			}
		}
	}
	outfile.close();

	outfile.open("result/ComparisonBetweenAlgorithms/benchmark3.csv");
	outfile << "num sol,num obj,num fro,Fast Sort,Corner Sort,Deductive Sort,T-ENS,ENS-NDT,Filter Sort" << std::endl;
	for (int num_sol : nums_sol) {
		for (int num_obj : nums_obj) {
			for (int num_fro : nums_fro) {
				std::cout << "benchmark3_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std::endl;
				outfile << num_sol << "," << num_obj << "," << num_fro;
				NS::benchmark3 generator(num_sol, num_obj, num_fro, 0.5);
				std::pair<int, int> meas_Fast_Sort({ 0,0 }), meas_Corner_Sort({ 0,0 }), meas_Deductive_Sort({ 0,0 }), meas_T_ENS({ 0,0 }), meas_ENS_NDT({ 0,0 }), meas_Filter_Sort({ 0,0 });
				std::vector<int> rank_Fast_Sort(num_sol), rank_Corner_Sort(num_sol), rank_Deductive_Sort(num_sol), rank_T_ENS(num_sol), rank_ENS_NDT(num_sol), rank_Filter_Sort(num_sol);
				std::vector<std::vector<double>> data;
				for (int runID = 0; runID < num_run; ++runID) {
					generator.read_data(data, runID);
					NS::fast_sort(data, rank_Fast_Sort, meas_Fast_Sort);
					NS::corner_sort(data, rank_Corner_Sort, meas_Corner_Sort);
					NS::deductive_sort(data, rank_Deductive_Sort, meas_Deductive_Sort);
					NS::T_ENS(data, rank_T_ENS, meas_T_ENS);
					rank_ENS_NDT = NS::ENS_NDT::Sort(data, meas_ENS_NDT);
					NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
				}
				outfile << "," << meas_Fast_Sort.first / num_run;
				outfile << "," << meas_Corner_Sort.first / num_run;
				outfile << "," << meas_Deductive_Sort.first / num_run;
				outfile << "," << meas_T_ENS.first / num_run;
				outfile << "," << meas_ENS_NDT.first / num_run;
				outfile << "," << meas_Filter_Sort.first / num_run;
				outfile << std::endl;
			}
		}
	}
	outfile.close();

	return 0;
}

//***********************************
// generate benchmark data set
//***********************************

//int main() {
//	std::vector<int> nums_sol({ 20000 });
//	std::vector<int> nums_obj({ 2,3,4,5,6,7,8 });
//	std::vector<int> nums_fro({ 1,2,3,4,5,6,7,8 });
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std::endl;
//	//		NS::benchmark1 generator(num_sol, num_obj, 0.5);
//	//		generator.output_data_set(50);
//	//	}
//	//}
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

//***************************************************
// The test for analysing three parts of Filter Sort
//***************************************************

//int main() {
//	//std::vector<int> nums_sol({ 5000,10000,15000,20000 });
//	//std::vector<int> nums_obj({ 2,3,4,5,6,7,8 });
//	//std::vector<int> nums_fro({ 1,2,3,4,5,6,7,8 });
//	std::vector<int> nums_sol({ 5000 });
//	std::vector<int> nums_obj({ 4 });
//	std::vector<int> nums_fro({ 10,20,30,40 });
//	const int num_run(50);
//
//	std::ofstream outfile;
//
//	//outfile.open("result/ComparisonBetweenInnerParts/benchmark1.csv");
//	//outfile << "num sol,num obj,normal,c1,c2,c3" << std::endl;
//	//for (int num_sol : nums_sol) {
//	//	for (int num_obj : nums_obj) {
//	//		std::cout << "benchmark1_sol" << num_sol << "_obj" << num_obj << std:: endl;
//	//		outfile << num_sol << "," << num_obj;
//	//		NS::benchmark1 generator(num_sol, num_obj, 0.5);
//	//		std::pair<int, int> meas_Filter_Sort({ 0,0 }), meas_Filter_Sort_c1({ 0,0 }), meas_Filter_Sort_c2({ 0,0 }), meas_Filter_Sort_c3({ 0,0 });
//	//		std::vector<int> rank_Filter_Sort(num_sol), rank_Filter_Sort_c1(num_sol), rank_Filter_Sort_c2(num_sol), rank_Filter_Sort_c3(num_sol);
//	//		std::vector<std::vector<double>> data;
//	//		for (int runID = 0; runID < num_run; ++runID) {
//	//			generator.read_data(data, runID);
//	//			NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
//	//			NS::filter_sort_c1(data, rank_Filter_Sort_c1, meas_Filter_Sort_c1);
//	//			NS::filter_sort_c2(data, rank_Filter_Sort_c2, meas_Filter_Sort_c2);
//	//			NS::filter_sort_c3(data, rank_Filter_Sort_c3, meas_Filter_Sort_c3);
//	//		}
//	//		outfile << "," << meas_Filter_Sort.first / num_run;
//	//		outfile << "," << meas_Filter_Sort_c1.first / num_run;
//	//		outfile << "," << meas_Filter_Sort_c2.first / num_run;
//	//		outfile << "," << meas_Filter_Sort_c3.first / num_run;
//	//		outfile << std::endl;
//	//	}
//	//}
//	//outfile.close();
//
//	outfile.open("result/ComparisonBetweenInnerParts/benchmark4.csv");
//	outfile << "num sol,num obj,num fro,normal,c1,c2,c3" << std::endl;
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			for (int num_fro : nums_fro) {
//				std::cout << "benchmark4_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std:: endl;
//				outfile << num_sol << "," << num_obj << "," << num_fro;
//				NS::benchmark4 generator(num_sol, num_obj, num_fro, 0.5);
//				std::pair<int, int> meas_Filter_Sort({ 0,0 }), meas_Filter_Sort_c1({ 0,0 }), meas_Filter_Sort_c2({ 0,0 }), meas_Filter_Sort_c3({ 0,0 });
//				std::vector<int> rank_Filter_Sort(num_sol), rank_Filter_Sort_c1(num_sol), rank_Filter_Sort_c2(num_sol), rank_Filter_Sort_c3(num_sol);
//				std::vector<std::vector<double>> data;
//				for (int runID = 0; runID < num_run; ++runID) {
//					generator.read_data(data, runID);
//					NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
//					NS::filter_sort_c1(data, rank_Filter_Sort_c1, meas_Filter_Sort_c1);
//					NS::filter_sort_c2(data, rank_Filter_Sort_c2, meas_Filter_Sort_c2);
//					NS::filter_sort_c3(data, rank_Filter_Sort_c3, meas_Filter_Sort_c3);
//				}
//				outfile << "," << meas_Filter_Sort.first / num_run;
//				outfile << "," << meas_Filter_Sort_c1.first / num_run;
//				outfile << "," << meas_Filter_Sort_c2.first / num_run;
//				outfile << "," << meas_Filter_Sort_c3.first / num_run;
//				outfile << std::endl;
//			}
//		}
//	}
//	outfile.close();
//
//	outfile.open("result/ComparisonBetweenInnerParts/benchmark5.csv");
//	outfile << "num sol,num obj,num fro,normal,c1,c2,c3" << std::endl;
//	for (int num_sol : nums_sol) {
//		for (int num_obj : nums_obj) {
//			for (int num_fro : nums_fro) {
//				std::cout << "benchmark5_sol" << num_sol << "_obj" << num_obj << "_fro" << num_fro << std:: endl;
//				outfile << num_sol << "," << num_obj << "," << num_fro;
//				NS::benchmark5 generator(num_sol, num_obj, num_fro, 0.5);
//				std::pair<int, int> meas_Filter_Sort({ 0,0 }), meas_Filter_Sort_c1({ 0,0 }), meas_Filter_Sort_c2({ 0,0 }), meas_Filter_Sort_c3({ 0,0 });
//				std::vector<int> rank_Filter_Sort(num_sol), rank_Filter_Sort_c1(num_sol), rank_Filter_Sort_c2(num_sol), rank_Filter_Sort_c3(num_sol);
//				std::vector<std::vector<double>> data;
//				for (int runID = 0; runID < num_run; ++runID) {
//					generator.read_data(data, runID);
//					NS::filter_sort(data, rank_Filter_Sort, meas_Filter_Sort);
//					NS::filter_sort_c1(data, rank_Filter_Sort_c1, meas_Filter_Sort_c1);
//					NS::filter_sort_c2(data, rank_Filter_Sort_c2, meas_Filter_Sort_c2);
//					NS::filter_sort_c3(data, rank_Filter_Sort_c3, meas_Filter_Sort_c3);
//				}
//				outfile << "," << meas_Filter_Sort.first / num_run;
//				outfile << "," << meas_Filter_Sort_c1.first / num_run;
//				outfile << "," << meas_Filter_Sort_c2.first / num_run;
//				outfile << "," << meas_Filter_Sort_c3.first / num_run;
//				outfile << std::endl;
//			}
//		}
//	}
//	outfile.close();
//	return 0;
//}

//int main() {
//	NS::benchmark5 generator(5000,2,5,0.5);
//	generator.output_data_set(30);
//	std::vector<int> rank;
//	std::pair<int,int> meas;
//	NS::filter_sort(generator.get_data(), rank, meas);
//	NS::NumCandi_and_SeqSum(generator.get_data(), rank);
//	return 0;
//}