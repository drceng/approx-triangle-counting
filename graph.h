#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

// Edge representation
struct Edge {
	int64_t s;
	int64_t t;
};

// Graph representation as a list of edges
struct EdgeList {
	int64_t n;	// number of vertices in the graph
	int64_t m;	// number of edges in the graph
	vector<Edge> E;	// list of edges in the graph

	// Creates an empty graph with the given number of vertices
	EdgeList(int64_t n) {
		this->n = n;
		m = 0;
	}

	// Reads and stores the edge list from the given text file
	EdgeList(char* edgelist) {
		FILE *file;
		Edge e;

		file = fopen(edgelist,"r");
		while (fscanf(file,"%ld %ld", &(e.s), &(e.t)) == 2) {
			int64_t max = (e.s > e.t) ? e.s : e.t;
			n = (max > n) ? max : n;
			E.push_back(e);
		}
		fclose(file);

		n++;
		m = E.size();
		E.shrink_to_fit();
	}

	EdgeList *subgraph(double p) {
		EdgeList *el = new EdgeList(n);
		double l = log(1.0 - p);
		double div = double(RAND_MAX) + 2;

		int64_t i = -1;
		while (true) {
			// random number in strictly open interval (0,1)
			double r = (double(rand()) + 1) / div;
			// computing the number of trials for the next edge to be included
			i += ceil(log(r) / l);
			if (i >= m) break;
			el->m++;
			el->E.push_back(E[i]);
		}

		return el;
	}
};

// Graph representation as adjacency lists
class Graph {
	bool directed;	// flag for edges stored as directed/undirected
	int64_t n;	// number of vertices in the graph
	int64_t m;	// number of edges in the graph
	int64_t *d;	// degrees of vertices
	int64_t *cd;	// cumulative degrees of vertices
	int64_t *adj;	// adjacency lists
	int64_t *cw;	// cumulative weights for random sampling
//	int64_t *rank;	// inverse map for degeneracy ordering of vertices

	// check whether w and u are adjacent
	bool adjacent(int64_t, int64_t);
	
	// return a random vertex based on the weight distribution cw
	int64_t random_vertex();
public:
	Graph(EdgeList &g);
	~Graph();

	// sort the adjacency list of every vertex in increasing order
	void sort_adjacency_lists();

	// compute degeneracy, direct edges, and relabel vertices
	void direct_edges_using_degeneracy();

	// assign weights to vertices for uniform wedge sampling
	int64_t assign_weights();

	// extend given edge to a random wedge from the low-degree end-vertex
	// if this wedge is closed then return (low-degree - 1) else return 0
	int64_t random_triangle_including(Edge);

	// pick a random wedge and check whether it is closed or not
	bool closed_random_wedge();
};
