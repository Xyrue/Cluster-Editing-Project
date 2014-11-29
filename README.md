Cluster Editing Project
=======================

A C implementation of an algorithm that solves the clustering editing project using ILP (integer linear programming).

### Introduction
Over the last decade high throughput technologies for measuring biological data have become available, changing the landscape of biological research. This enables, for the first time, a global, simultaneous view of proteins behavior and function. 
<br>Proteins are the main building blocks of the cell and are responsible for its structure and function. They work by interacting with one another to build molecular machines. 
<br>One can describe these interactions via a network G=(V,E), where the nodes V represent proteins and the edges E represent the interactions. 

The goal of this project is to try and infer the function of unknown proteins based on the functions of known proteins
that are “close” to them in the network. This is based on the observation that interacting, or close by proteins tend to share the same function. The basic idea is described by the following 3-step process:

**Step 1.** Mark the network's proteins whose function is known.
<br>**Step 2.** Cluster the network to maximize its similarity to a graph of cliques.
<br>**Step 3.** For uncharacterized proteins, infer their functions according to their cluster members.

### The Cluster Editing Problem

A cluster graph is a graph consisting of a vertex-disjoint union of cliques. The cluster editing problem for weighted graphs is defined as follows: 
<br>Given a network G=(V,E,s), with costs s(u,v) over all vertex pairs, find a set of edge modifications of minimum cost which results in a cluster graph.
<br>**Note that to create a cluster graph we may remove an existing edge e with the cost s(e) or add a non-existing edge (“ghost”) g with a cost C (which will be given to the program as a parameter).**
