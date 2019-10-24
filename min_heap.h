#include <cstdlib>
#include <cstdint>
#include <utility>

#define	Parent(i)	(((i) - 1) / 2)
#define	Child(i)	(2 * (i) + 1)

class Min_heap {
	int64_t n;	// number of nodes
	int64_t *pt;	// pointers to nodes
	int64_t *ind;	// indices of nodes
	int64_t *val;	// values

	void swap_nodes(int64_t i, int64_t j) {
		swap(pt[ind[i]], pt[ind[j]]);
		swap(val[i], val[j]);
		swap(ind[i], ind[j]);
	}

	void bubble_up(int64_t i) {
		int64_t j = Parent(i);
		while (i > 0) {
			if (val[j] > val[i]) {
				swap_nodes(i, j);
				i = j;
				j = Parent(i);
			}
			else break;
		}
	}

	void bubble_down(int64_t j) {
		while (j < n) {
			int64_t i = j;
			int64_t j1 = Child(i);
			int64_t j2 = j1 + 1;
			j = (j2 < n && val[j2] < val[j1]) ? j2 : j1;

			if (j < n && val[j] < val[i]) {
				swap_nodes(i, j);
			}
		}
	}

public:
	Min_heap(int64_t nElements, int64_t *elements) {
		n = nElements;
		val = elements;
		pt = new int64_t[n];
		ind = new int64_t[n];

		int64_t i;
		for (i = 0; i < n; i++) {
			ind[i] = pt[i] = i;
		}

		for (i = Parent(n - 1); i >= 0; i--) {
			bubble_down(i);
		}
	}

	~Min_heap() {
		delete [] pt;
		delete [] ind;
	}

	int64_t extract_min() {
		swap_nodes(0, --n);
		bubble_down(0);
		return ind[n];
	}

	int64_t update(int64_t k) {
		int64_t i = pt[k];
		if (i < n) {
			val[i]--;
			bubble_up(i);
		}
		return i;
	}
};
