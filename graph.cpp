#include "graph.h"
#include "min_heap.h"

// check whether w and u are adjacent
bool Graph::adjacent(int64_t w, int64_t u) {
	if (w < u) swap(w, u);
	vector<int64_t> range(adj + cd[u], adj + cd[u + 1]);
	return binary_search(range.begin(), range.end(), w);
}

// return a random vertex based on the weight distribution cw
int64_t Graph::random_vertex() {
	int64_t r, lo, hi, mid;

	r = rand() + rand() * (int64_t) RAND_MAX;
	r = r % cw[n - 1];

	lo = 0;
	hi = n - 1;
	while (lo <= hi) {
		mid = lo + (hi - lo) / 2;
		if (mid > 0 && cw[mid - 1] > r) hi = mid - 1;
		else if (cw[mid] <= r) lo = mid + 1;
		else return mid;
	}
	return -1;
}

// constructor
Graph::Graph(EdgeList &g) {
	directed = false;
	n = g.n;
	m = g.m;
	d = new int64_t[n] {0};
	cd = new int64_t[n + 1];
	adj = new int64_t[2 * m];
	cw = new int64_t[n];
//	rank = new int64_t[n];

	// calculate degrees
	int64_t i, s, t;
	for (i = 0; i < m; i++) {
		d[g.E[i].s]++;
		d[g.E[i].t]++;
	}

	// calculate cumulative degrees, and reset degrees to prepare for adjacency lists
	cd[0] = 0;
	for (i = 0; i < n; i++) {
		cd[i + 1] = cd[i] + d[i];
		d[i] = 0;
	}

	// store adjacencies of every vertex in a single array
	for (i = 0; i < m; i++) {
		s = g.E[i].s;
		t = g.E[i].t;
		adj[ cd[s] + d[s]++ ] = t;
		adj[ cd[t] + d[t]++ ] = s;
	}
}

Graph::~Graph() {
	delete [] d;
	delete [] cd;
	delete [] adj;
	delete [] cw;
//	delete [] rank;
}

// sort the adjacency list of every vertex in increasing order
void Graph::sort_adjacency_lists() {
	int64_t *new_adj = new int64_t[2 * m];	// sorted adjacency lists
	int64_t i, j, v;

	for (i = 0; i < n; i++) {
		d[i] = 0;
	}

	for (i = 0; i < n; i++) {
		for (j = cd[i]; j < cd[i + 1]; j++) {
			v = adj[j];
			new_adj[ cd[v] + d[v]++ ] = i;
		}
	}

	delete [] adj;
	adj = new_adj;
}

// compute degeneracy, direct edges, and relabel vertices
// this ensures that all adjacency lists are sorted, needed for binary search
void Graph::direct_edges_using_degeneracy() {
	if (directed) return;

	int64_t i, j, k, v;
	int64_t *cum_d = new int64_t[n + 1];	// cumulative outdegrees
	int64_t *dadj = new int64_t[m];		// adjacency lists for the directed and relabeled graph

	// create a min heap using the degrees of the vertices
	Min_heap heap(n, d);

	int64_t total = 0; // total outdegree so far
	cum_d[0] = 0;
	for (i = 0; i < n; i++) {
		// prepare for adjacency lists
		cum_d[i + 1] = total;

		// next vertex k in the degeneracy ordering, with a new label of i
		k = heap.extract_min();
		// rank[i] = k; // inverse mapping to restore i to k

		// iterating over the vertices adjacent to k
		for (j = cd[k]; j < cd[k + 1]; j++) {
			v = n - heap.update(adj[j]);

			// if the vertex adj[j] has already been removed from the heap before k
			// then its new label would be (v - 1), otherwise the new label is unknown
			if (v <= i) {
				// include i as an outgoing edge of vertex (v - 1)
				dadj[cum_d[v]++] = i;
			} else {
				// new outdegree encountered from vertex k (new label i) to adj[j] (unknown new label)
				total++;
			}
		}
	}

	// replace undirected data with directed version
	directed = true;

	delete [] cd;
	delete [] adj;

	cd = cum_d;
	adj = dadj;

	// update degrees
	for (i = 0; i < n; i++) {
		d[i] = cd[i + 1] - cd[i];
	}
}

// assign weights to vertices for uniform wedge sampling
int64_t Graph::assign_weights() {
	int64_t i, cwt;

	cwt = 0;
	for (i = 0; i < n; i++) {
		cwt += d[i] * (d[i] - 1) / 2;
		cw[i] = cwt;
	}

	return cwt;
}

// extend given edge to a random wedge from the low-degree end-vertex
// if this wedge is closed then return (low-degree - 1) else return 0
int64_t Graph::random_triangle_including(Edge e) {
	int64_t s, t;
	if (d[e.s] <= d[e.t]) {
		s = e.s;
		t = e.t;
	} else {
		s = e.t;
		t = e.s;
	}
	if (d[s] < 2) return 0;

	int64_t i = rand() % (d[s] - 1);
	int64_t u = adj[cd[s] + i];
	if (t <= u) u = adj[cd[s] + i + 1];

	return adjacent(t, u) ? (d[s] - 1) : 0;
}

// pick a random wedge and check whether it is closed or not
bool Graph::closed_random_wedge(){
	int64_t v = random_vertex();
	int64_t u = rand() % d[v];
	int64_t w = rand() % (d[v] - 1);
	if (w >= u) w++;

	u = adj[cd[v] + u];
	w = adj[cd[v] + w];

	return adjacent(w, u);
}
