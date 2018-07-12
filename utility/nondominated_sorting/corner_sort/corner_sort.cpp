#include "corner_sort.h"
#include "../merge_sort.h"
#include <thread>
#include <mutex>
std::mutex cs_p_mutex;

namespace NS {
	void corner_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement) {

		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		int NumComp(0);
		CornerSort cs(data);
		if (rank.size() != data.size()) rank.resize(data.size());

		Total_start_time = std::chrono::system_clock::now();
		cs.sort(rank, NumComp);
		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();

		measurement.first += time;
		measurement.second += NumComp;
	}

	void corner_sort_p(const int numTask, const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement)	{
		std::chrono::time_point<std::chrono::system_clock> Total_start_time;
		Total_start_time = std::chrono::system_clock::now();

		const size_t N = data.size(); // Number of solution
		if (N == 0) throw("Data is empty");
		const size_t M = data.front().size(); // Number of obective
		//if (rank.size() != N) rank.resize(N);
		rank = std::vector<int>(N, -1);
		std::vector<std::vector<int>> SeqByObj(M);
		for (int i = 0; i < M; ++i) {
			merge_sort(data, SeqByObj[i], i);
		}
		std::vector<List> SeqByObj_Lists(M); // Put vectors 'SeqByObj' into Lists
		std::vector<std::vector<Node*>> PosInObjLists(N); // PosInObjLists[i] stores solution[i]'s all Nodes' addresses
		for (int i = 0; i < M; ++i) {
			for (int SolIdx : SeqByObj[i])
				PosInObjLists[SolIdx].push_back(SeqByObj_Lists[i].push_back(SolIdx));
		}
		size_t num_ranked(0);
		size_t cur_rank_num(0);
		while (num_ranked < N) {
			size_t obj_selected(0);
			std::vector<List> SeqByObjLists_copy(M);
			std::vector<std::vector<Node*>> PosInObjLists_copy(N, std::vector<Node*>(M));
			for (size_t j = 0; j < M; j++) {
				for (const Node* iter = SeqByObj_Lists[j].begin(); iter != nullptr; iter = iter->m_next) {
					PosInObjLists_copy[iter->m_value][j] = SeqByObjLists_copy[j].push_back(iter->m_value);
				}
			}
			while (!SeqByObjLists_copy[0].empty()) {
				size_t corner = SeqByObjLists_copy[obj_selected%M].begin()->m_value;
				num_ranked++;
				rank[corner] = cur_rank_num;
				for (size_t j = 0; j < M; ++j) {
					SeqByObj_Lists[j].erase(PosInObjLists[corner][j]);
					SeqByObjLists_copy[j].erase(PosInObjLists_copy[corner][j]);
				}
				
				std::vector<size_t> tocheck;
				for (Node* iter = SeqByObjLists_copy[0].begin(); iter != nullptr; iter = iter->m_next) 
					tocheck.push_back(iter->m_value);

				std::vector<size_t> remove;
				for (size_t idx : tocheck) {
					bool dominate(true);
					for (size_t j = 0; j < M; j++) {
						if (data[idx][j] < data[corner][j]) {
							dominate = false;
							break;
						}
					}
					if (dominate) {
						remove.push_back(idx);
					}
				}

				//int TaskSize = tocheck.size();
				//int num_task = numTask;
				//if (num_task > TaskSize) num_task = TaskSize;
				//std::vector<std::vector<size_t>> removes(num_task);
				//std::vector<std::thread> thrd;
				//for (int i = 0; i < num_task; ++i) {
				//	std::vector<int> idxs;
				//	for (int k = i; k < TaskSize; k += num_task)
				//		idxs.push_back(tocheck[k]);
				//	thrd.push_back(std::thread(Corner_Sort::parallel_check, corner, M, std::cref(data), std::move(idxs), std::ref(removes[i])));
				//}
				//for (auto&t : thrd) t.join();

				//for (auto& remove : removes)
					for (size_t idx : remove) 
						for (size_t j = 0; j < M; ++j)
							SeqByObjLists_copy[j].erase(PosInObjLists_copy[idx][j]);
				obj_selected++;
			}
			cur_rank_num++;
		}

		int time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - Total_start_time).count();
		measurement.first += time;
	}

	void Corner_Sort::parallel_check(const size_t corner, const size_t M, const std::vector<std::vector<double>>& data, const std::vector<int> && idxs, std::vector<size_t>& remove) {
		for (size_t idx : idxs) {
			bool dominate(true);
			for (size_t j = 0; j < M; j++) {
				if (data[idx][j] < data[corner][j]) {
					dominate = false;
					break;
				}
			}
			if (dominate) {
				remove.push_back(idx);
			}
		}
	}

	CornerSort::CornerSort(const std::vector<std::vector<double>>& data) : n(data.size()), mv_comp(n,0), comp(mv_comp.data()) {
		if (n == 0) throw("data is empty");
		m = data.front().size();
		POP = new const double*[n];
		for (int i = 0; i < n; ++i)
			POP[i] = data[i].data();
	}

	void CornerSort::sort(std::vector<int>& rank, int &NumComp) {
		unsigned int i, j, cout = 0, markcout = 0, cur, obj_index, flag;
		int r = -1;
		cs_node *head = NULL, *mh = NULL, *p = NULL, *newone = NULL;
		//initialize linked list(rank and mark)
		for (i = 0; i < n; i++)
		{
			newone = (cs_node*)malloc(sizeof(cs_node));
			newone->index = i;
			newone->ranknext = NULL;
			if (i == 0)
			{
				head = newone;
				p = head;
				p->rankpre = NULL;
			}
			else
			{
				p->ranknext = newone;
				newone->rankpre = p;
				p = newone;
			}
		}

		while (head != NULL)
		{
			r = r + 1;//arrange the r-th ranking
					  //unmark all the unranked solutions
			p = head;
			while (p != NULL)
			{
				p->marknext = p->ranknext;
				p->markpre = p->rankpre;
				p = p->ranknext;
			}
			mh = head;
			while (mh != NULL)//until all the solutions are marked
			{
				for (obj_index = 0; mh != NULL && obj_index < m; ++obj_index)
				{
					// find solution of the best objective obj_index among unmarked ones
					cur = min_obj(obj_index, &head, &mh, comp, m, NumComp);// delete it in both mark and rank linked lists
					rank[cur] = r;
					cout++;
					p = mh;
					//mark the dominated solutions by cur
					while (p != NULL)
					{
						flag = 0;
						for (j = 0; j < m; ++j)
						{
							NumComp++;
							if (comp[j] == 0) {
								if (j != obj_index && POP[cur][j] > POP[p->index][j])
								{
									flag = 1;
									break;
								}
							}
							else {
								if (j != obj_index && POP[cur][j] < POP[p->index][j])
								{

									flag = 1;
									break;
								}
							}

						}
						if (flag == 0)
						{
							if (p->markpre == NULL)
							{
								mh = mh->marknext;
								if (mh != NULL)
								{
									mh->markpre = NULL;
								}
							}
							else if (p->marknext == NULL)
							{
								p->markpre->marknext = NULL;
							}
							else
							{
								p->markpre->marknext = p->marknext;
								p->marknext->markpre = p->markpre;
							}
						}
						p = p->marknext;
					}
				}
			}
		}
	}

	unsigned int CornerSort::min_obj(unsigned int obj_index, cs_node ** head, cs_node ** mh, int * comp, const unsigned int m, int & NumComp)	{
		unsigned int i = 0, cur;
		cs_node *p = *mh, *q = NULL;
		cur = p->index;
		q = p;
		while (p->marknext != NULL)
		{
			if (comp[obj_index] == 0) // minimization 
			{
				NumComp++;
				if (POP[p->marknext->index][obj_index] < POP[cur][obj_index])
				{
					cur = p->marknext->index;
					q = p->marknext;
				}
				else if (POP[p->marknext->index][obj_index] == POP[cur][obj_index])
				{
					unsigned int i = 0;
					while (i < m)
					{
						unsigned int temp_obj_index = (obj_index + i) % m;
						NumComp++;
						if (POP[p->marknext->index][temp_obj_index] < POP[cur][temp_obj_index]) {
							cur = p->marknext->index;
							q = p->marknext;
							break;
						}
						else if (POP[p->marknext->index][temp_obj_index] > POP[cur][temp_obj_index]) {
							break;
						}
						else
							i++;
					}
				}
			}
			else
			{
				NumComp++;
				if (POP[p->marknext->index][obj_index] > POP[cur][obj_index])
				{
					cur = p->marknext->index;
					q = p->marknext;
				}
				else if (POP[p->marknext->index][obj_index] == POP[cur][obj_index])
				{
					unsigned int i = 0;
					while (i < m)
					{
						unsigned int temp_obj_index = (obj_index + i) % m;
						NumComp++;
						if (POP[p->marknext->index][temp_obj_index] > POP[cur][temp_obj_index]) {
							cur = p->marknext->index;
							q = p->marknext;
							break;
						}
						else if (POP[p->marknext->index][temp_obj_index] < POP[cur][temp_obj_index]) {
							break;
						}
						else
							i++;
					}
				}
			}

			p = p->marknext;
		}
		if (q->markpre == NULL)
		{
			*mh = (*mh)->marknext;
			if ((*mh) != NULL)
			{
				(*mh)->markpre = NULL;
			}
		}
		else if (q->marknext == NULL)
		{
			q->markpre->marknext = NULL;
		}
		else
		{
			q->markpre->marknext = q->marknext;
			q->marknext->markpre = q->markpre;
		}
		if (q->rankpre == NULL)
		{
			*head = (*head)->ranknext;
			if ((*head) != NULL)
			{
				(*head)->rankpre = NULL;
			}
			free(q);
		}
		else if (q->ranknext == NULL)
		{
			p = q;
			q->rankpre->ranknext = NULL;
			free(p);
		}
		else
		{
			p = q;
			q->rankpre->ranknext = q->ranknext;
			q->ranknext->rankpre = q->rankpre;
			free(p);
		}
		return(cur);
	}
}


