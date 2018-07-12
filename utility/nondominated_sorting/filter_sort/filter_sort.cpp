#include "filter_sort.h"
#include "../../functional.h"
#include "../merge_sort.h"
#include <limits.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <string.h>

namespace NS {
	int filter_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		//int NumComp(0);
		Total_start_time = std::chrono::system_clock::now();
		const int N = data.size(); // number of solutions
		if (N == 0)
			return 0;
		const int M = data[0].size(); // number of obectives
		if (rank.size() != N)
			rank.resize(N);
		std::vector<std::vector<int>> seq(M); // "seq[j]" denotes the sequence of solutions sorted by the j-th objective
		for (int j = 0; j < M; ++j) {
			merge_sort(data, seq[j], j);
		}
		std::vector<List> List_seq(M); // same as "seq" but in form of <List>, used to speedup travesal and modification
		std::vector<std::vector<Node*>> add_in_List_seq(N); // "add_in_List_seq[i]" stores solution i's addresses in "List_seq" 
		for (int j = 0; j < M; ++j) {
			for (int i : seq[j])
				add_in_List_seq[i].push_back(List_seq[j].push_back(i));
		}
		std::vector<std::vector<int>> o(N); // "o[i][j]" denotes the sequence of solution i in the j-th objective
		for (int j = 0; j < M; ++j) {
			int order = 0;
			for (Node* iter = List_seq[j].begin(); iter != nullptr; iter = iter->m_next) {
				o[iter->m_value].push_back(order);
				order++;
			}
		}
		std::vector<int> w(N); // "w[i]" denotes the worst order of solution i
		std::vector<int> b(N); // "b[i]" denotes the best order of solution i
		std::vector<int> s(N); // "s[i]" denotes the sum of ordinal numbers of solution i
		for (int i = 0; i < N; ++i) {
			int max_val = -1;
			int min_val = N;
			for (int j = 0; j < M; ++j) {
				if (o[i][j] < min_val) {
					min_val = o[i][j];
					b[i] = j;
				}
				if (o[i][j] > max_val) {
					max_val = o[i][j];
					w[i] = j;
				}
				s[i] += o[i][j];
			}
		}
		std::vector<int> f(N); // sequence of solution sorted by "s"
		OFEC::quick_sort(s, N, f);
		List List_f; // same as "f" but in form of List
		for (const int i : f)
			add_in_List_seq[i].push_back(List_f.push_back(i));
		int ranknum = 0; // current front rank number
		int numranked = 0; // number of ranked solutions  
		std::vector<int> c; // indexes of candidate solutions
		c.reserve(N);
		bool* in_c = new bool[N]; // whether in current front
		//memset(in_c, false, N);
		int filter; // the solution chosen to generate CurRankCandidate
		while (numranked < N) {
			// set filter
			filter = List_f.begin()->m_value; c.clear(); memset(in_c, false, N);
			// generate CurCandidate by filter
			for (size_t j = 0; j < M; ++j) {
				for (auto iter = List_seq[j].begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == filter)
						break;
					else if (!in_c[iter->m_value]) {
						in_c[iter->m_value] = true;
						c.push_back(iter->m_value);
					}
				}
			}
			// filter the CurCandidate
			for (size_t candidate : c) {
				bool dominated(false); // whether candidate is in current rank 
				for (auto iter = List_seq[b[candidate]].begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value != candidate) {
						if (o[iter->m_value][w[iter->m_value]] > o[candidate][w[iter->m_value]])
							continue;
						// check whether solution[iter->m_value] donminate solution[candidate] or not
						bool FlagDominate(true);
						for (int i = 0; i < M; ++i) {
							if (i != w[iter->m_value] && o[iter->m_value][i] > o[candidate][i]) {
								FlagDominate = false;
								break;
							}
						}
						if (FlagDominate) {
							dominated = true;
							break;
						}
					}
					else
						break;
				}
				if (dominated)
					in_c[candidate] = false;
			}
			// set rank for current Front
			std::vector<int> frontrank;
			for (auto candidate : c) {
				if (in_c[candidate]) {
					rank[candidate] = ranknum;
					numranked++;
					frontrank.push_back(candidate);
				}
			}
			rank[filter] = ranknum;
			numranked++;
			frontrank.push_back(filter);
			// if terminate
			if (numranked == N)
				break;
			// delete ranked solutions from List_seq
			for (auto candidate : frontrank) {
				for (int i = 0; i < M; ++i)
					List_seq[i].erase(add_in_List_seq[candidate][i]);
				List_f.erase(add_in_List_seq[candidate][M]);
			}
			// goto next rank
			ranknum++;
		}
		delete[] in_c;
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();
		measurement.first += time;
		//measurement.second += NumComp;
		//std::stringstream os;
		//os << "result/sum_seq_2obj_concave.csv";
		//std::ofstream outfile(os.str());
		//outfile << "sum_seq" << std::endl;
		//for (int i = 0; i < N; ++i)
		//	outfile << SumVals[i] << std::endl;
		//outfile.close();
		return ranknum+1;
	}

	int filter_sort_m1(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		Total_start_time = std::chrono::system_clock::now();
		const int N = data.size(); // number of solution
		if (N == 0)
			return 0;
		const int M = data[0].size(); // number of obective
		if (rank.empty() || rank.size() != N)
			rank.resize(N);
		std::vector<std::vector<int>> SeqByObj(M);
		for (int i = 0; i < M; ++i) {
			merge_sort(data, SeqByObj[i], i);
		}
		std::vector<List> SeqByObj_Lists(M); // same as SeqByObj but in form of LS_list
		std::vector<std::vector<Node*>> PosInObjLists(N); // PosInObjLists[i] stores solution[i]'s all LS_node addresses 
		for (int i = 0; i < M; ++i) {
			for (int SolIdx : SeqByObj[i])
				PosInObjLists[SolIdx].push_back(SeqByObj_Lists[i].push_back(SolIdx));
		}
		std::vector<std::vector<int>> SolStas(N); // SolStas[i] stores solution[i]'s all single objective sequence numbers
		for (int i = 0; i < M; ++i) {
			int SeqNum = 0;
			for (Node* iter = SeqByObj_Lists[i].begin(); iter != nullptr; iter = iter->m_next) {
				SolStas[iter->m_value].push_back(SeqNum);
				SeqNum++;
			}
		}
		std::vector<int> MaxIdxs(N); // MaxIdxs[i] means index of solution[i]'s maximum objective sequence number
		std::vector<int> MinIdxs(N); // MinIdxs[i] means index of solution[i]'s minimum objective sequence number
		for (int i = 0; i < N; ++i) {
			int max_val = -1;
			int min_val = N; // value of solution's minimum single objective sequence number
			for (int ObjIdx = 0; ObjIdx < M; ++ObjIdx) {
				if (SolStas[i][ObjIdx] < min_val) {
					min_val = SolStas[i][ObjIdx];
					MinIdxs[i] = ObjIdx;
				}
				if (SolStas[i][ObjIdx] > max_val) {
					max_val = SolStas[i][ObjIdx];
					MaxIdxs[i] = ObjIdx;
				}
			}
		}
		int CurRankNumber = 0; // current rank number
		int num_not_ranked = N; // number of solutions not ranked
		std::vector<int> CurCandidate; // the candidate solutions of current front
		CurCandidate.reserve(N);
		bool* InCurFront = new bool[N]; // whether in current front
		memset(InCurFront, false, N);
		int filter; // the solution chosen to generate CurRankCandidate
		while (num_not_ranked > 0) {
			// set filter
			filter = SeqByObj_Lists[rand()%M].begin()->m_value;
			// generate CurCandidate by filter
			for (auto& SeqByObj_List : SeqByObj_Lists) {
				for (auto iter = SeqByObj_List.begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == filter)
						break;
					else if (!InCurFront[iter->m_value]) {
						InCurFront[iter->m_value] = true;
						CurCandidate.push_back(iter->m_value);
					}
				}
			}
			// filter the CurCandidate
			for (auto candidate : CurCandidate) {
				bool FlagInCurRank(true); // whether candidate is in current rank 
				for (auto iter = SeqByObj_Lists[MinIdxs[candidate]].begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == candidate)
						break;
					else {
						if (SolStas[iter->m_value][MaxIdxs[iter->m_value]] > SolStas[candidate][MaxIdxs[iter->m_value]])
							continue;
						// check whether solution[iter->m_value] donminate solution[candidate] or not
						bool FlagDominate(true);
						for (int i = 0; i < M; ++i) {
							if (i != MaxIdxs[iter->m_value] && SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								//if (SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								FlagDominate = false;
								break;
							}
						}
						if (FlagDominate) {
							FlagInCurRank = false;
							break;
						}
					}
				}
				if (!FlagInCurRank)
					InCurFront[candidate] = false;
			}
			// set rank for current Front
			std::vector<int> curFront;
			for (auto candidate : CurCandidate) {
				if (InCurFront[candidate]) {
					rank[candidate] = CurRankNumber;
					num_not_ranked--;
					curFront.push_back(candidate);
				}
			}
			rank[filter] = CurRankNumber;
			num_not_ranked--;
			curFront.push_back(filter);
			// if terminate
			if (num_not_ranked == 0)
				break;
			// delete ranked solutions from lists
			for (auto candidate : curFront) {
				for (int i = 0; i < M; ++i)
					SeqByObj_Lists[i].erase(PosInObjLists[candidate][i]);
			}
			// goto next rank
			CurRankNumber++;
			CurCandidate.clear();
			memset(InCurFront, false, N);
		}
		delete[] InCurFront;
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();
		measurement.first += time;
		measurement.second += NumComp;
		return CurRankNumber + 1;
	}

	int filter_sort_m2(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement)
	{
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		Total_start_time = std::chrono::system_clock::now();
		const int N = data.size(); // number of solution
		if (N == 0)
			return 0;
		const int M = data[0].size(); // number of obective
		if (rank.empty() || rank.size() != N)
			rank.resize(N);
		std::vector<std::vector<int>> SeqByObj(M);
		for (int i = 0; i < M; ++i) {
			merge_sort(data, SeqByObj[i], i);
		}
		std::vector<List> SeqByObj_Lists(M); // same as SeqByObj but in form of LS_list
		std::vector<std::vector<Node*>> PosInObjLists(N); // PosInObjLists[i] stores solution[i]'s all LS_node addresses 
		for (int i = 0; i < M; ++i) {
			for (int SolIdx : SeqByObj[i])
				PosInObjLists[SolIdx].push_back(SeqByObj_Lists[i].push_back(SolIdx));
		}
		std::vector<std::vector<int>> SolStas(N); // SolStas[i] stores solution[i]'s all single objective sequence numbers
		for (int i = 0; i < M; ++i) {
			int SeqNum = 0;
			for (Node* iter = SeqByObj_Lists[i].begin(); iter != nullptr; iter = iter->m_next) {
				SolStas[iter->m_value].push_back(SeqNum);
				SeqNum++;
			}
		}
		std::vector<int> MinIdxs(N); // MinIdxs[i] means index of solution[i]'s minimum objective sequence number
		std::vector<int> SumVals(N); // SumVals[i] means sum of solution[i]'s all objective sequence numbers
		for (int i = 0; i < N; ++i) {
			int min_val = N; // value of solution's minimum single objective sequence number
			for (int ObjIdx = 0; ObjIdx < M; ++ObjIdx) {
				if (SolStas[i][ObjIdx] < min_val) {
					min_val = SolStas[i][ObjIdx];
					MinIdxs[i] = ObjIdx;
				}
				SumVals[i] += SolStas[i][ObjIdx];
			}
		}
		std::vector<int> SeqBySumVals(N); // sequence of solution sorted by sum of all single objective sequence numbers
		OFEC::quick_sort(SumVals, N, SeqBySumVals);
		List SeqBySumVals_Lists; // Same as SeqByMinVals but in form of LS_list
		for (const int SolIndex : SeqBySumVals)
			PosInObjLists[SolIndex].push_back(SeqBySumVals_Lists.push_back(SolIndex));
		int CurRankNumber = 0; // current rank number
		int num_not_ranked = N; // number of solutions not ranked
		std::vector<int> CurCandidate; // the candidate solutions of current front
		CurCandidate.reserve(N);
		bool* InCurFront = new bool[N]; // whether in current front
		memset(InCurFront, false, N);
		int filter; // the solution chosen to generate CurRankCandidate
		while (num_not_ranked > 0) {
			// set filter
			filter = SeqBySumVals_Lists.begin()->m_value;
			// generate CurCandidate by filter
			for (auto& SeqByObj_List : SeqByObj_Lists) {
				for (auto iter = SeqByObj_List.begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == filter)
						break;
					else if (!InCurFront[iter->m_value]) {
						InCurFront[iter->m_value] = true;
						CurCandidate.push_back(iter->m_value);
					}
				}
			}
			// filter the CurCandidate
			for (auto candidate : CurCandidate) {
				bool FlagInCurRank(true); // whether candidate is in current rank 
				for (auto iter = SeqByObj_Lists[MinIdxs[candidate]].begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == candidate)
						break;
					else {
						// check whether solution[iter->m_value] donminate solution[candidate] or not
						bool FlagDominate(true);
						for (int i = 0; i < M; ++i) {
							if (SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								//if (SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								FlagDominate = false;
								break;
							}
						}
						if (FlagDominate) {
							FlagInCurRank = false;
							break;
						}
					}
				}
				if (!FlagInCurRank)
					InCurFront[candidate] = false;
			}
			// set rank for current Front
			std::vector<int> curFront;
			for (auto candidate : CurCandidate) {
				if (InCurFront[candidate]) {
					rank[candidate] = CurRankNumber;
					num_not_ranked--;
					curFront.push_back(candidate);
				}
			}
			rank[filter] = CurRankNumber;
			num_not_ranked--;
			curFront.push_back(filter);
			// if terminate
			if (num_not_ranked == 0)
				break;
			// delete ranked solutions from lists
			for (auto candidate : curFront) {
				for (int i = 0; i < M; ++i)
					SeqByObj_Lists[i].erase(PosInObjLists[candidate][i]);
				SeqBySumVals_Lists.erase(PosInObjLists[candidate][M]);
			}
			// goto next rank
			CurRankNumber++;
			CurCandidate.clear();
			memset(InCurFront, false, N);
		}
		delete[] InCurFront;
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();
		measurement.first += time;
		measurement.second += NumComp;
		return CurRankNumber + 1;
	}

	int filter_sort_p(const int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		Total_start_time = std::chrono::system_clock::now();
		const int N = data.size(); // number of solution
		if (N == 0)
			return 0;
		const int M = data[0].size(); // number of obective
		if (rank.empty() || rank.size() != N)
			rank.resize(N);
		std::vector<std::vector<int>> SeqByObj(M);



		int TaskSize = M;
		std::vector<std::thread> thrd;
		for (int i = 0; i < numTask; ++i) {
			std::vector<int> ObjIdxs;
			for (int idx = i; idx < TaskSize; idx += numTask)
				ObjIdxs.push_back(idx);
			thrd.push_back(std::thread(Filter_Sort::parallel_mergesort, std::cref(data), std::ref(SeqByObj), std::move(ObjIdxs)));
		}
		for (auto&t : thrd)
			t.join();



		std::vector<List> SeqByObj_Lists(M); // same as SeqByObj but in form of LS_list
		std::vector<std::vector<Node*>> PosInObjLists(N); // PosInObjLists[i] stores solution[i]'s all LS_node addresses 
		for (int i = 0; i < M; ++i) {
			for (int SolIdx : SeqByObj[i])
				PosInObjLists[SolIdx].push_back(SeqByObj_Lists[i].push_back(SolIdx));
		}
		std::vector<std::vector<int>> SolStas(N); // SolStas[i] stores solution[i]'s all single objective sequence numbers
		for (int i = 0; i < M; ++i) {
			int SeqNum = 0;
			for (Node* iter = SeqByObj_Lists[i].begin(); iter != nullptr; iter = iter->m_next) {
				SolStas[iter->m_value].push_back(SeqNum);
				SeqNum++;
			}
		}
		std::vector<int> MaxIdxs(N); // MaxIdxs[i] means index of solution[i]'s maximum objective sequence number
		std::vector<int> MinIdxs(N); // MinIdxs[i] means index of solution[i]'s minimum objective sequence number
		std::vector<int> SumVals(N); // SumVals[i] means sum of solution[i]'s all objective sequence numbers
		for (int i = 0; i < N; ++i) {
			int max_val = -1;
			int min_val = N; // value of solution's minimum single objective sequence number
			for (int ObjIdx = 0; ObjIdx < M; ++ObjIdx) {
				if (SolStas[i][ObjIdx] < min_val) {
					min_val = SolStas[i][ObjIdx];
					MinIdxs[i] = ObjIdx;
				}
				if (SolStas[i][ObjIdx] > max_val) {
					max_val = SolStas[i][ObjIdx];
					MaxIdxs[i] = ObjIdx;
				}
				SumVals[i] += SolStas[i][ObjIdx];
			}
		}
		std::vector<int> SeqBySumVals(N); // sequence of solution sorted by sum of all single objective sequence numbers
		OFEC::quick_sort(SumVals, N, SeqBySumVals);
		List SeqBySumVals_Lists; // Same as SeqByMinVals but in form of LS_list
		for (const int SolIndex : SeqBySumVals)
			PosInObjLists[SolIndex].push_back(SeqBySumVals_Lists.push_back(SolIndex));
		int CurRankNumber = 0; // current rank number
		int num_not_ranked = N; // number of solutions not ranked
		std::vector<int> CurCandidate; // the candidate solutions of current front
		CurCandidate.reserve(N);
		bool* InCurFront = new bool[N]; // whether in current front
		memset(InCurFront, false, N);
		int filter; // the solution chosen to generate CurRankCandidate
		while (num_not_ranked > 0) {
			// set filter
			filter = SeqBySumVals_Lists.begin()->m_value;
			// generate CurCandidate by filter
			for (auto& SeqByObj_List : SeqByObj_Lists) {
				for (auto iter = SeqByObj_List.begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == filter)
						break;
					else if (!InCurFront[iter->m_value]) {
						InCurFront[iter->m_value] = true;
						CurCandidate.push_back(iter->m_value);
					}
				}
			}
			// filter the CurCandidate
			TaskSize = CurCandidate.size();
			thrd.clear();
			for (int i = 0; i < numTask; ++i) {
				std::vector<int> candidates;
				for (int idx = i; idx < TaskSize; idx += numTask)
					candidates.push_back(CurCandidate[idx]);
				thrd.push_back(std::thread(Filter_Sort::parallel_filter, std::move(candidates), std::cref(SeqByObj_Lists), std::cref(MaxIdxs), std::cref(MinIdxs), M, std::cref(SolStas), InCurFront));
			}
			for (auto&t : thrd)
				t.join();
			// set rank for current Front
			std::vector<int> curFront;
			for (auto candidate : CurCandidate) {
				if (InCurFront[candidate]) {
					rank[candidate] = CurRankNumber;
					num_not_ranked--;
					curFront.push_back(candidate);
				}
			}
			rank[filter] = CurRankNumber;
			num_not_ranked--;
			curFront.push_back(filter);
			// if terminate
			if (num_not_ranked == 0)
				break;
			// delete ranked solutions from lists
			for (auto candidate : curFront) {
				for (int i = 0; i < M; ++i)
					SeqByObj_Lists[i].erase(PosInObjLists[candidate][i]);
				SeqBySumVals_Lists.erase(PosInObjLists[candidate][M]);
			}
			// goto next rank
			CurRankNumber++;
			CurCandidate.clear();
			memset(InCurFront, false, N);
		}
		delete[] InCurFront;
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();
		measurement.first += time;
		measurement.second += NumComp;
		//std::stringstream os;
		//os << "result/sum_seq_2obj_concave.csv";
		//std::ofstream outfile(os.str());
		//outfile << "sum_seq" << std::endl;
		//for (int i = 0; i < N; ++i)
		//	outfile << SumVals[i] << std::endl;
		//outfile.close();
		//std::cout << CurRankNumber << std::endl;
		//system("pause");
		return CurRankNumber + 1;
	}

	namespace Filter_Sort {
		void output_num_cand(const std::vector<std::vector<double>>& data, const std::vector<int>& rank)
		{
			std::vector<int> filters; // to stores all filters's indexs
			const int N = data.size(); // number of solution
			if (N == 0)
				return;
			const int M = data[0].size(); // number of obective
			std::vector<std::vector<int>> SeqByObj(M);
			for (int i = 0; i < M; ++i) {
				merge_sort(data, SeqByObj[i], i);
			}
			std::vector<List> SeqByObj_Lists(M); // same as SeqByObj but in form of LS_list
			for (int i = 0; i < M; ++i) {
				for (int SolIdx : SeqByObj[i])
					SeqByObj_Lists[i].push_back(SolIdx);
			}
			std::vector<int> first_front;
			for (std::size_t i = 0; i < rank.size(); ++i)
				if (rank[i] == 0)
					first_front.push_back(i);
			std::vector<int> NumCandi;
			std::vector<int> CurRankCandidate; // the candidate solutions of current rank
			std::vector<int> InCurRankCandiate(N, 0); // whether in CurRankCandidate
			for (int filter : first_front) {
				CurRankCandidate.clear();
				InCurRankCandiate = std::vector<int>(N, 0);
				// generate CurRankCandidate by link
				for (auto& SeqByObj_List : SeqByObj_Lists) {
					for (auto iter = SeqByObj_List.begin(); iter != nullptr; iter = iter->m_next) {
						if (iter->m_value == filter)
							break;
						else if (!InCurRankCandiate[iter->m_value]) {
							InCurRankCandiate[iter->m_value] = 1;
							CurRankCandidate.push_back(iter->m_value);
						}
					}
				}
				NumCandi.push_back(CurRankCandidate.size());
			}
			//output numcandi
			std::stringstream os;
			os << "result/num_cand_2obj_concave.csv";
			std::ofstream outfile(os.str());
			outfile << "index,num_cand" << std::endl;
			for (int i = 0; i < first_front.size(); ++i)
				outfile << first_front[i] << "," << NumCandi[i] << std::endl;
		}

		void parallel_filter(const std::vector<int>&& candidates, const std::vector<List>& SeqByObj_Lists, const std::vector<int>& MaxIdxs, const std::vector<int>& MinIdxs, const int m, const std::vector<std::vector<int>>& SolStas, bool* InCurFront) {
			for (int candidate : candidates) {
				bool FlagInCurRank(true); // whether candidate is in current rank 
				for (const NS::Node* iter = SeqByObj_Lists[MinIdxs[candidate]].begin(); iter != nullptr; iter = iter->m_next) {
					if (iter->m_value == candidate)
						break;
					else {
						if (SolStas[iter->m_value][MaxIdxs[iter->m_value]] > SolStas[candidate][MaxIdxs[iter->m_value]])
							continue;
						// check whether solution[iter->m_value] donminate solution[candidate] or not
						bool FlagDominate(true);
						for (int i = 0; i < m; ++i)
							if (i != MaxIdxs[iter->m_value] && SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								//if (SolStas[iter->m_value][i] > SolStas[candidate][i]) {
								FlagDominate = false;
								break;
							}
						if (FlagDominate) {
							FlagInCurRank = false;
							break;
						}
					}
				}
				if (!FlagInCurRank)
					InCurFront[candidate] = false;
			}
		}

		void parallel_mergesort(const std::vector<std::vector<double>>& data, std::vector<std::vector<int>>& SeqByObj, const std::vector<int>&& ObjIdxs) {
			for (int ObjIdx : ObjIdxs)
				merge_sort(data, SeqByObj[ObjIdx], ObjIdx);
		}
	}
}