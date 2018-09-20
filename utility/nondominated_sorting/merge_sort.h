#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <vector>

namespace NS {
	template<typename T>
	bool greater(const std::vector<T>& a, const std::vector<T>& b, const int obj_idx) {
		std::size_t obj_num = a.size();
		for (std::size_t i = 0; i < obj_num; ++i) {
			int idx = (i + obj_idx) % obj_num;
			if (a[idx] > b[idx])
				return true;
			else if (a[idx] < b[idx])
				return false;
		}
		return true;
	}


	template<typename T>
	void merge(const std::vector<std::vector<T>>& data, const int obj_idx, std::vector<int>& A, std::vector<int>& B, int l, int m, int r) {
		int i = l, j = m;
		for (int k = l; k < r; ++k) {
			if (i < m && (j >= r || !greater(data[A[i]], data[A[j]], obj_idx))) {
				B[k] = A[i];
				i++;
			}
			else {
				B[k] = A[j];
				j++;
			}
		}
	}

	template<typename T>
	void mergeSort(const std::vector<std::vector<T>>& data, const int obj_idx, std::vector<int>& B, std::vector<int>& A, int l, int r)
	{
		if (r - l < 2)
			return;
		int m = (r + l) / 2;
		mergeSort(data, obj_idx, A, B, l, m);
		mergeSort(data, obj_idx, A, B, m, r);

		merge(data, obj_idx, B, A, l, m, r);
	}

	template<typename T>
	void merge_sort(const std::vector<std::vector<T>>& data, std::vector<int>& index, const int obj_idx, int low = -1, int high = -1, bool ascending = true) {
		if (low == -1) {
			low = 0;
			high = data.size() - 1;
		}
		const int N = high - low + 1;
		if (index.size() == 0 || index.size() != N)		index.resize(N);

		std::vector<int> A(data.size());
		for (auto i = A.begin(); i != A.end(); ++i) *i = i - A.begin();
		std::vector<int> B(A);
		mergeSort(data, obj_idx, B, A, low, high+1);
		for (int i = 0; i < N; ++i)
			index[i] = A[i + low];
		if (!ascending) {
			for (std::size_t i = 0; i < N / 2; i++)
				std::swap(index[i], index[N - 1 - i]);
		}
	}
}

namespace OFEC {
	template<typename T>
	void merge(const T& data, std::vector<int>& A, std::vector<int>& B, int l, int m, int r) {
		int i = l, j = m;
		for (int k = l; k < r; ++k) {
			if (i < m && (j >= r || data[A[i]] <= data[A[j]])) {
				B[k] = A[i];
				i++;
			}
			else {
				B[k] = A[j];
				j++;
			}
		}
	}

	template<typename T>
	void mergeSort(const T& data, std::vector<int>& B, std::vector<int>& A, int l, int r)
	{
		if (r - l < 2)
			return;
		int m = (r + l) / 2;
		mergeSort(data, A, B, l, m);
		mergeSort(data, A, B, m, r);

		merge(data, B, A, l, m, r);
	}

	template<typename T>
	void merge_sort(const T& data, int size, std::vector<int>& index, int low = -1, int high = -1, bool ascending = true) {
		if (low == -1) {
			low = 0;
			high = data.size() - 1;
		}
		const int N = high - low + 1;
		if (index.size() == 0 || index.size() != N)		index.resize(N);

		std::vector<int> A(data.size());
		for (auto i = A.begin(); i != A.end(); ++i) *i = i - A.begin();
		std::vector<int> B(A);
		mergeSort(data, B, A, low, high + 1);
		for (int i = 0; i < N; ++i)
			index[i] = A[i + low];
		if (!ascending) {
			for (std::size_t i = 0; i < N / 2; i++)
				std::swap(index[i], index[N - 1 - i]);
		}
	}
}

#endif // !MERGE_SORT_H

