# approx-triangle-counting

Approximating the number of triangles in a graph by sampling a small number of
"low-hinge" wedges. Compile using the below command:

```bash
g++ -o atc graph.cpp approx_triangle_counting.cpp
```

Usage:

```bash
./atc list-of-edges.txt 
```

## Standard Wedge Sampling [1]

Our algorithm improves on the standard technique of sampling adjacent edge
pairs (wedges) uniformly at random for approximating the number of triangles.
This standard wedge sampling technique can be summarized as follows:

- Given a graph *G = (V,E)*, let *W =* &sum;*<sub>v &isin; V</sub> &omega;(v)*
  be the total number of wedges in *G* where *&omega;(v) = d(v) (d(v)-1) / 2*
is the number of wedges for which *v* is the middle (hinge) vertex and where
*d(v)* is the degree of *v*.

- Uniformly sample *k* wedges. Each wedge can be sampled by first sampling a
  vertex *v* with probability *&omega;(v) / W* and then by uniformly sampling
one out of the *&omega;(v)* wedges of *v*.

- Check whether each sampled wedge is closed (closed wedge = triangle), and
  obtain a ratio *r*

<p align="center">
r = number of closed wedges / k
<p>

Output *T = r W / 3* as an approximation of the total number of triangles.
Here, division by 3 is essential since each triangle can be counted by 3
different wedges.

## Low-Hinge Wedge Sampling [2]

### Main Idea

We classify wedges into three categories: "low-hinge", "mid-hinge", and
"high-hinge". Instead of sampling from any of the three categories of wedges,
we sample exclusively from low-hinge wedges.

### Low-Hinge Wedges

A total ordering &#8826; on *V* turns *G* into a directed acyclic graph where
an edge between *v* and *u* is directed from *v* to *u* if and only if *v*
&#8826; *u*. Using this DAG, a wedge is "low-hinge" if both edges of the wedge
are directed away from the hinge vertex. Therefore, the number of low-hinge
wedges for which *v* is the hinge vertex is *&omega;<sup>+</sup>(v)* =
*d<sup>+</sup>(v) (d<sup>+</sup>(v)-1) / 2* where *d<sup>+</sup>(v)* is the
outdegree of *v*.

### Degeneracy Ordering

Given *G = (V,E)*, a degeneracy ordering is a total ordering on *V* that is
obtained by repeatedly removing a vertex of minimum degree in the remaining
subgraph. This procedure defines the total ordering &#8826; where *v* &#8826;
*u* if and only if *v* is removed before *u*. Note that a graph may have
multiple degeneracy orderings.

### Algorithm

- Given *G = (V,E)*, let &#8826; be a degeneracy ordering obtained by the above
  procedure, and let *W<sup>+</sup>* = &sum;*<sub>v &isin; V</sub>
&omega;<sup>+</sup>(v)* be the total number of low-hinge wedges based on
&#8826;

- Uniformly sample *k<sup>+</sup>* low-hinge wedges. For each one, first sample
  a vertex v with probability *&omega;<sup>+</sup>(v) / W<sup>+</sup>* and then
uniformly sample one out of the *&omega;<sup>+</sup>(v)* low-hinge wedges of
*v*.

- Check whether each sampled low-hinge wedge is closed, and obtain a ratio
  *r<sup>+</sup>*

<p align="center">
r<sup>+</sup> = number of closed low-hinge wedges / k<sup>+</sup>
</p>

Output *T<sup>+</sup> = r<sup>+</sup> W<sup>+</sup>* as an approximation of the
total number of triangles. Note that the output does not require division by 3
since for each triangle, there is only one low-hinge wedge.

## Empirical Analysis

The number of samples needed to bound the error depends on the concentration of
closed wedges in the sample space, i.e, the closed-to-total-wedges ratio. For
standard wedge sampling, this is the global clustering coefficient *C*, which
is approximated by *r* above. Empirical evidence shows that the corresponding
ratio of closed-to-total low-hinge wedges is much higher than *C*, and this
ratio is approximated by *r<sup>+</sup>* above. Consequently, we observe that
the low-hinge wedge sampling method requires much fewer samples than the
standard wedge sampling method in order to achieve similar error bounds.

## Edge-Based Wedge Sampling [3]

We also implement another recently proposed method for approximating triangle
counts in large graphs and that is the edge-based wedge sampling method, which
can be summarized as follows:

- Given *G = (V,E)*, pick every edge in *E* with probability *p*.

- For each picked edge *e*, consider the lower degree end-vertex *v* of *e*,
  and uniformly sample one neighbor out of the remaining *d(v) - 1* neighbors
of *v* in order to extend *e* into a wedge.

- Check whether the sampled wedge is closed, and if so add *d(v) - 1* to the
  estimated number of triangles *T<sup>s</sup>*.

Output *T<sup>s</sup> / 3p* as an approximation of the total number of
triangles. Note that *T<sup>s</sup>* approximates the size of the set *{(v,u,w)
: (v,u) is picked and (v,w),(u,w) &isin; E}*. Here, the output does require
division by 3 because a triangle has three edges, each of which can be the edge
picked in the first step.

This method performs slightly better with respect to the standard wedge
sampling method but the low-hinge wedge sampling algorithm outperforms all of
these methods.

## References

[1] 
C. Seshadhri, Ali Pinar, and Tamara G. Kolda.
Wedge sampling for computing clustering coefficients and triangle counts on large graphs.
Statistical Analysis and Data Mining 7, 4 (2014). 294–307.

[2]
[Ata Turk and Duru Türko&#287;lu.
Revisiting Wedge Sampling for Triangle Counting.
In The World Wide Web Conference, WWW 2019. 1875–1885.][WWW]

[3] 
[Duru Türko&#287;lu and Ata Turk.
Edge-Based Wedge Sampling to Estimate Triangle Counts in Very Large Graphs.
In 2017 IEEE International Conference on Data Mining, ICDM 2017. 455–464.][ICDM]

To use our algorithms, we kindly request you to cite as follows (bibtex format):

```
[2]:
@inproceedings{Turk:2019,
  author = {Ata Turk and Duru T{\"{u}}rko{\u{g}}lu},
  title = {Revisiting Wedge Sampling for Triangle Counting},
  booktitle = {The World Wide Web Conference},
  series = {WWW '19},
  year = {2019},
  location = {San Francisco, CA, USA},
  pages = {1875--1885},
  url = {http://doi.acm.org/10.1145/3308558.3313534},
  doi = {10.1145/3308558.3313534},
} 
```

```
[3]:
@inproceedings{Turkoglu:2017,
  author    = {Duru T{\"{u}}rko{\u{g}}lu and Ata Turk},
  title     = {Edge-Based Wedge Sampling to Estimate Triangle Counts in Very Large Graphs},
  booktitle = {2017 {IEEE} International Conference on Data Mining, {ICDM} 2017},
  pages     = {455--464},
  year      = {2017}
  doi = {10.1109/ICDM.2017.55},
  url = {https://doi.ieeecomputersociety.org/10.1109/ICDM.2017.55},
}
```

[WWW]: http://doi.acm.org/10.1145/3308558.3313534
[ICDM]: https://doi.ieeecomputersociety.org/10.1109/ICDM.2017.55
