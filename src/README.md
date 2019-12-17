## About

> Finding shortest paths and tweaking the flows blocks is the ultimate goal, the way of choosing the paths might need some extra information, like a residual edges which will serve when the BFS blocks due to visited node. Although, we skip all nodes, but this is not what would work (I guess), we need to check the residual edges of visited nodes instead of ignoring them. Since, there's no order in which edge will be taken first, and the first shortest path might cut other possible `good paths`.

## BFS and the shortest paths

Traversing the graph using `BFS` will ensure getting from the *source*, towards the *sink* in the fastest possible way, since we are traversing the graph by layers. However, we need to *retrace the actual path from the source towards the sink*, this could be done by maintaining an array of parents; by indexing the nodes, we can trace back the parent of each node while doing `BFS` using something like `parent[edge->node_dst] = edge->node_src`, and at the end of `BFS`, loop over the array or parents starting at the *sink*'s index.  Keep doing this until no more paths leading to the *sink* are found.

> If `parents[sink] == -1`, there's no path (left) leading to the *sink*. Since we have traversed all unvisited nodes without getting into the *sink*

## Expected output based on test_farm

> [Graph in test_farm](http://graphonline.ru/en/?graph=zXqpFcUZcljoTFec) ![CQeMKMmqwJiwURGv](https://user-images.githubusercontent.com/4406704/70102976-138ad000-163a-11ea-9287-988829b52e3b.png)

In the first run of `BFS`, the shortest path is `1-2-4-0` (or `1-3-4-0`) where `1` is the *source* and `0` is the *sink*, now nodes `2` and `4` are *visited* (or exist in a path). The next call of BFS will give `1-3-5-6-0`, which is not the "shortest path", but it's _the second shortest path_ after the first one.

So two paths are expected:

  * `1-2-4-0`
  * `1-3-5-6-0`

## Notes on finding the maximum flow in a flow graph

> There are so many algorithms to find the *best possible way in order to achieve maximum flow through a flow graph*
>
> * [Ford-Fulkerson](https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm) Keep looking for augmenting paths until no more augmenting paths could be found. Where how to actually find them is unspecified detail.
> *  [Edmonds-Karp](https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm)
> *  [Dinic's Algorithm](https://en.wikipedia.org/wiki/Dinic's_algorithm)

In order to solve this, we shall add a residual edges to `struct s_edges`, so that

----------

## About augmenting paths

> Given a graph `G(V, E)`, where `V` is the set of vertices (`|V| = N`) and `E` is the set of edges (directed pair of vertices `src, dst` in `E`).

An augmenting path is a set of edges that leads to the `sink` from a given `source`, such that they fulfil a maximum flow through the network. In our case, we have a the capacity of all edges is `1`, thus we have to just find a list of augmenting paths without thinking about flow adjustments that would be in a network where edges have different capacities.

## Idea

One key requirement is to have a *residual graph in order to avoid collusions*, since we cannot have more than one ant at each vertex at each step. So this altering `struct s_edge` so that each edge holds its backward edge.

```C
/* both directed edges, ie. each edges and it's residual, are stored in a list
 * both filled as they occur. (if we get both we might end up having duplicates!?)
 */
struct s_edge
{
	struct s_edge	 *residual;   /* backward edge: <dst, src> */
        struct s_node    *src, *dst;  /* actual edge */
        struct s_edge    *next;
};
```

The straight forward approach is to run a `BFS(G)` starting at `start` with a clear purpose in mind: *get as many shortest paths* as possible. while is we face a blocking flow while traversing the graph, we check residual edges too to see if they have a way through that leads to the `sink`. When `BFS(G)` can no longer find paths, we traverse we shall *re-wire* the edges which have collusions.

### About the re-wiring operation

> Given a set of augmenting paths `P` where each `p_i` is a set of edges.

Running a *bfs-like* operation by following all edges, let `current` be list, and `re_wire` be a queue

1. first, push all `start`'s edges to `current`.
1. remove all `current`'s edges which are leading to a source node of their residual edge, respectively, while enqueue *only the edges* to `re_wire`, *since each edge holds its residual, so no need o do extra work handling both of them*.
1. if `re_wire` has something
1.1 for each element in `re_wire` do
1.1.1 swap destinations vertices of each edge and it's residual
1.1.1 dequeue re_wire
1.2  swap each edge with it's dest
