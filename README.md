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

## Standard Wedge Sampling

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

## Low-Hinge Wedge Sampling

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
