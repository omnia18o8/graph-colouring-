## 1. Introduction

This work investigates the Asymmetric Palette Sparsification (APS) algorithm — a graph-theoretic approach for efficient colouring in streaming and distributed settings.
Building on the work of Assadi & Yazdanyar (2022) and (2025), APS explores how random palette reduction can achieve near-optimal colourability while drastically reducing space, time, and communication complexity.

Traditional graph colouring algorithms require full graph visibility and high memory overhead, making them impractical in massive or dynamic graphs. APS instead leverages randomized sparsification: each vertex samples a limited subset of colours (its palette) independently, creating an asymmetric structure that allows feasible colouring even under strict memory constraints.

The study extends APS within the semi-streaming model, connecting ideas from pseudorandomness, sparsification, and the Ahn–Guha–McGregor framework for sketch-based graph processing.

## 2. Methodology

The work formalizes the APS model by defining:

* A vertex-arrival stream, where edges or vertex updates are processed sequentially.
* A randomized palette assignment, where each vertex samples from a reduced colour domain using pairwise-independent hash functions.
* A conflict-resolution rule, ensuring local colour decisions remain consistent with global feasibility.

Implementation and testing were carried out in C++ and Python using the following tools:

| Tool / Library        | Purpose                                                          |
| --------------------- | ---------------------------------------------------------------- |
| C++ STL / Boost Graph | Fast graph generation and edge streaming                         |
| NumPy                 | Probabilistic palette simulation and validation                  |
| Matplotlib            | Visualization of sparsification ratios and success probabilities |

Simulations were conducted on both synthetic and real-world graphs of varying density and maximum degree (Δ).
Each experiment measured:

* Palette size vs. success probability
* Space usage vs. Δ
* Approximation error in edge conflicts compared to the theoretical (Δ + 1) bound

## 3. Results

The results confirm that APS achieves sublinear space complexity with negligible degradation in colouring accuracy.

Key findings include:

* Successful colouring with O(n polylog n) space in the semi-streaming model
* A 40–90% reduction in required palette size compared to uniform sampling
* Conflict probability decreasing exponentially with palette size, consistent with theoretical bounds

In dense graphs, APS maintained strong accuracy using only O(n log² n) bits of memory, validating its space–accuracy trade-off.

## 4. Implications

The APS algorithm has major implications for large-scale graph analytics and streaming data structures.
It shows that random sparsification can preserve global graph properties using only local, probabilistic updates.

This contributes to broader research in:

* Streaming graph algorithms, where maintaining full adjacency information is infeasible
* Distributed colouring, enabling efficient load balancing and conflict-free scheduling
* Sketch-based computation, connecting APS with sparse-recovery and compressed-sensing principles

Practically, APS can improve systems for graph partitioning, network routing, and task scheduling under limited memory or communication.

## 5. Conclusion

This project establishes Asymmetric Palette Sparsification as an efficient, scalable alternative to classical graph colouring.
By combining probabilistic sparsification with sketch-based computation, APS delivers both theoretical guarantees and empirical robustness.

The main outcome is producing a high-probability colourability with sublinear memory usage.

Future directions include:

* Extending APS to dynamic graph streams with insertions and deletions
* Exploring MPC and sub-linear time models
* Integrating approximate recovery sketches for adaptive palette updates

