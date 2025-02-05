## Statement

Given a tree of size n, proceed q queries. For each query, 
given a subset of vertices on tree, print the virtual tree of it.

## Input Format

```plain
n q
a[0] b[0]
a[1] b[1]
...
a[n - 2] b[n - 2]
k[0] p[0][0] ... p[0][k[0] - 1]
k[1] p[1][0] ... p[1][k[1] - 1]
...
k[q] p[q - 1][0] ... p[q - 1][k[q - 1] - 1]
```

a and b describe the edges of the tree. 
k is the size of the subset and p is the given subset.

## Output Format

For each query, print:

```plain
s
x[0] fa[0]
x[1] fa[1]
...
x[s - 1] fa[s - 1]
```

s is the size of the virtual tree. x are the nodes of virtual tree and fa[p] is the father of p. If p is the root,
let fa[p] = -1.