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
