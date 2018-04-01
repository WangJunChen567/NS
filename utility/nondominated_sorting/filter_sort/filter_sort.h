#ifndef LINKSORT_H
#define LINKSORT_H
#include <vector>
#include "../../../core/definition.h"
#include <chrono>

namespace NS {
	struct fs_node
	{
		fs_node(const int value, fs_node* last = nullptr, fs_node* next = nullptr) : m_value(value), m_last(last), m_next(next) {}
		const int m_value;
		fs_node* m_last;
		fs_node* m_next;
};
	class fs_list
	{
	public:
		fs_list() : m_begin(nullptr), m_end(nullptr) {}
		fs_node* push_back(const int value) {
			fs_node* new_node(new fs_node(value));
			if (m_begin != nullptr) {
				new_node->m_last = m_end;
				m_end->m_next = new_node;
				m_end = new_node;
			}
			else {
				m_begin = new_node;
				m_end = new_node;
			}
			return new_node;
		}
		void erase(fs_node* node) {
			if (node != m_begin && node != m_end) {
				node->m_last->m_next = node->m_next;
				node->m_next->m_last = node->m_last;
			}
			else if (node == m_begin && node != m_end) {
				node->m_next->m_last = nullptr;
				m_begin = node->m_next;
			}
			else if (node == m_end && node != m_begin) {
				node->m_last->m_next = nullptr;
				m_end = node->m_last;
			}
			else {
				m_begin = nullptr;
				m_end = nullptr;
			}
			delete node;
		}
		fs_node* begin() { return m_begin; }
		fs_node* end() { return m_end; }
	private:
		fs_node* m_begin;
		fs_node* m_end;
	};
	void filter_sort(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	void filter_sort_c1(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	void filter_sort_c2(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	void filter_sort_c3(const std::vector<std::vector<double>>& data, std::vector<int>& rank, std::pair<int, int>& measurement);
	void NumCandi_and_SeqSum(const std::vector<std::vector<double>>& data, const std::vector<int>& rank);
#ifdef USING_CONCURRENT
	void ParallelFilter(const std::vector<int>&& candidates, std::vector<LS_list>& SeqByObj_Lists, const std::vector<int>& MaxIdxs, const std::vector<int>& MinIdxs, const int N, const std::vector<std::vector<int>>& SolStas, bool* InCurRankCandiate);
	void ParallelQuickSort(const std::vector<std::vector<double>>& data, std::vector<std::vector<int>>& SeqByObj, const std::vector<int>&& ObjIdxs);
#endif // USING_CONCURRENT

}

#endif // !LINKSORT_H
