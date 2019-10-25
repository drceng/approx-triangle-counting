#include "graph.h"
#include <sys/time.h>
#include <time.h>

// find the number of closed wedges for a number of sampled wedges
int64_t count_closed_wedges(Graph *g, int64_t num_trials) {
	srand (time(NULL));
	int64_t num_closed = 0;

	while (num_trials > 0) {
		if (g->closed_random_wedge()) {
			num_closed++;
		}
		num_trials--;
	}

	return num_closed;
}

void record_time(struct timeval &t_begin) {
	struct timeval t_end;
	gettimeofday(&t_end, NULL);
	int64_t secs = t_end.tv_sec - t_begin.tv_sec;
	int64_t millis = secs * 1000 + (t_end.tv_usec - t_begin.tv_usec) / 1000;
	secs = millis / 1000;

	printf("--- %ldh%ldm%ld.%03lds\n\n", secs / 3600,
			(secs % 3600) / 60,
			secs % 60, millis % 1000);

	t_begin = t_end;
}

void edge_based_wedge_sampling(Graph *g, EdgeList *el, double prob) {
	int64_t r = 0;

	for (Edge e : el->E) {
		int64_t low_deg = g->random_triangle_including(e);
		r += low_deg;
	}

	double t = r / (3 * prob);
	printf("Approximate number of triangles = %ld\n", int64_t(t + 0.5));
}

int64_t wedge_sampling(Graph *g, int overcount) {
	int64_t w = g->assign_weights();
	printf("Number of all wedges = %ld\n", w);

	int64_t num_trials = 2500;
	int64_t num_closed = 0;

	while (num_closed < 2500) {
		num_trials *= 2;
		num_closed = count_closed_wedges(g, num_trials);
		printf("Number of closed/total sampled wedges = %ld/%ld\n", num_closed, num_trials);
	}

	double r = num_closed / (double) num_trials;
	double t = r * w / overcount;
	printf("Approximate number of triangles = %ld, approximate ratio = %lf\n", int64_t(t + 0.5), r);

	return num_trials;
}

int main(int argc,char** argv) {
	EdgeList *el;
	Graph *g;

	struct timeval t0, t1;
	gettimeofday(&t0, NULL);
	t1 = t0;

	// PHASE 1 : Read the graph as a list of edges
	printf("Reading graph as an edgelist from file %s\n",argv[1]);
	el = new EdgeList(argv[1]);
	printf("Number of vertices = %ld\n", el->n);
	printf("Number of edges = %ld\n", el->m);
	record_time(t1);

	// PHASE 2 : Convert to adjacency list representation
	printf("Creating adjacency lists\n");
	g = new Graph(*el);
	record_time(t1);

	//// BEGIN ALTERNATIVE PHASE 3
	// PHASE 3-a : Sort adjacency lists
	printf("STANDARD WEDGE SAMPLING:\n");
	printf("Sorting adjacency lists:\n");
	g->sort_adjacency_lists();
	record_time(t1);

	// PHASE 3-b : Standard wedge sampling
	printf("Appoximating the number of triangles:\n");
	int64_t num_trials = wedge_sampling(g, 3);
	record_time(t1);

	// PHASE 3-c : Edge-based wedge sampling
	printf("EDGE-BASED WEDGE SAMPLING:\n");
	double prob = (double)num_trials / el->m;
	EdgeList *sub_el = el->subgraph(prob);
	printf("Number of edges in the subgraph = %ld\n", sub_el->m);

	edge_based_wedge_sampling(g, sub_el, prob);
	record_time(t1);
	//// END ALTERNATIVE PHASE 3

	delete el; // No need for the list of edges anymore
	delete sub_el; // No need for the subgraph either

	// PHASE 3-a : Compute a degeneracy ordering, direct edges, and relabel vertices
	printf("LOW-HINGE WEDGE SAMPLING:\n");
	printf("Computing a degeneracy ordering and reorganizing\n");
	g->direct_edges_using_degeneracy();
	record_time(t1);

	// PHASE 3-b : Low-hinge wedge sampling
	printf("Appoximating the number of triangles:\n");
	wedge_sampling(g, 1);
	record_time(t1);

	delete g;

	printf("Total time elapsed: ");
	record_time(t0);

	return 0;
}
