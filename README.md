Chapter 1
INTRODUCTIONS
There are two main balanced binary search tree schemes: height-balanced and weightbalanced. The height-balanced scheme maintains the height of the whole tree in O(log n)
where n is the number of nodes in the tree. Red-black trees by Bayer [1], Guibas and
Sedgewick [4] and AVL trees are examples of this scheme in which the worst-case cost of
every operation is O(log n) time. The weight-balanced scheme ensures the size of subtrees
rooted at siblings for every node in the tree approximately equal. Nievergelt and Reingold
[6] first introduced a tree using such scheme to implement Search and Update operations in
O(log n) time as well. The technique used in Scapegoat trees combines those two schemes.
By maintaining weight-balanced, it also maintains height-balanced for a Search operation
and by detecting a height-unbalanced subtree and rebuilding the subtree, it also ensures
weight-balanced after an update operation. Before going deep into operations in Scapegoat
trees, let’s get ourselves familiar with notations and definitions of balanced binary search
trees.
1.1 Binary Search Tree Notations
If n is a node in a Scapegoat tree T then
• n.key is the value of the key stored at the node n
• n.left refers to the left child of n
• n.right refers to the right child of n
• n.height refers to the height of the subtree rooted at n or the longest distance in terms
of edges from n to some leaf.
• n.depth refers to the depth of node n or the distance in terms of edges from the root
to n
• n.parent refers to the parent node of node n
• n.sibling refers to the other child, other than n, of parent of n
3
5
2 8
0 6 9
7
Figure 1.1: A binary search tree with α = 0.6
• n.size refers to the number of nodes of subtree rooted at n. This notation is used for
proofs of correctness and time complexity
• n.hα is computed as n.hα = ⌊log1/α (n.size)⌋
• n.size() is the procedure to compute the size of the binary search tree rooted at node
n. This notation is used in the descriptions of operations on Scapegoat trees.
What is in bold font, for example n.key, is what is really stored at each node. The others,
except for the procedures, are just values used for discussions in proofs of correctness and
time complexity of operations on Scapegoat trees. Below are notations related to a binary
search tree T:
• T.root refers to the root of the tree T
• T.size refers to the current number of nodes in the tree T
• T.maxSize refers to the maximum number of nodes or the maximum number of T.size
is or was since T was completely rebuilt because whenever the whole tree T is rebuilt,
T.maxSize is set to T.size
• T.hα is computed as following: T.hα = ⌊log1/α (T.size)⌋
• T.height refers to the height of T or the longest path in terms of edges from T.root to
some leaf in T
Similar to notations of a node, what is in bold font is what is stored in tree T. T.height is
just used in concepts and proofs.
1.2 Weight Balanced Binary Search Tree
Definition 1.2.1. A node n in a binary search tree is α-weight-balanced for some α such
that 1/2 ≤ α < 1 if (n.lef t).size ≤ α · n.size and (n.right).size ≤ α · n.size.
4
Table 1: Examples of notations for a binary tree node 8 in Figure 1.1
n.key n.parent n.sibling n.left n.right n.height n.depth n.size n.hα n.size()
8 5 2 6 9 2 1 4 2 4
Table 2: Examples of notations for a binary search tree in Figure 1.1
T.root T.size T.height T.hα
5 7 3 3
7
5
3 6
8
9
10
Figure 1.2: A weight-unbalanced binary search tree with α = 0.6
5
5
3 7
8
9
6
Figure 1.3: A height-unbalanced binary search tree with α = 0.55
Example 1.2.1. Node 8 in Figure 1.2 is not 0.6-weight-balanced because its right subtree
has 2 nodes which is greater than 0.6 ∗ 3 = 1.8 where 3 is the total nodes of the subtree rooted
at 8.
Definition 1.2.2. A binary search tree T is α-weight-balanced for some α such that 1/2 ≤
α < 1 if all of its nodes are α-weight-balanced.
Example 1.2.2. The tree in Figure 1.2 is not 0.6-weight-unbalanced binary search tree
because it contains node 8 which is not 0.6-weight-balanced.
1.3 Height Balanced Binary Search Tree
Definition 1.3.1. A binary search tree is α-height-balanced for some α such that 1/2 ≤ α <
1 if T.height ≤ T.hα.
Example 1.3.1. The tree in Figure 1.2 is 0.6-height-balanced binary search tree because it
has 7 nodes and its height is 3 ≤ ⌊log1/0.6
(7)⌋ = 3.
1.4 Scapegoat Tree Related Definitions
Definition 1.4.1. A node n in a binary search tree T is a deep node for some α such that
1/2 ≤ α < 1 if n.depth > T.hα.
Definition 1.4.2. A node s is called Scapegoat node of a newly inserted deep node n if s
is an ancestor of n and s is not α-weight-balanced.
Example 1.4.1. If we have just inserted node 9 into the tree T in Figure 1.3 then node 9
is a deep node because its height is 3 which is greater than T.hα = ⌊log1/0.55 (6)⌋ = 2. And
in this case, root 5 would be chosen as Scapegoat node.
6
7
5
3 6
9
10
Figure 1.4: An incomplete binary search tree
Definition 1.4.3. A binary search tree is loosely α-height-balanced for some α such that
1/2 ≤ α < 1 if T.height ≤ (T.hα + 1).
Example 1.4.2. The tree T in Figure 1.3 is a good example of loosely 0.55-height-balanced
with 3 = T.height ≤ (T.hα + 1) = (⌊log1/0.55 (6)⌋ + 1) = 3
Definition 1.4.4. A binary search tree T is complete if inserting any node into T increases
its height.
Example 1.4.3. The tree in Figure 1.4 is incomplete. But after inserting node 8 into the
tree, it becomes complete.
With all the above definitions, we could now envision the notion of a Scapegoat tree. A
Scapegoat tree is just a regular binary search tree in which the height of the tree is always
loosely α-height-balanced. Such balance is maintained after an Insert operation of a deep
node or a Delete operation that leads to T.size < α · T.maxsize. In the case of inserting
a deep node, a Scapegoat node s will be detected. Both cases will result in rebuilding the
subtree rooted at s into a 1/2-weight-balanced binary search one.
7
Chapter 2
OPERATIONS ON SCAPEGOAT
TREES
This chapter will present how to do Search, Insert, Delete operations on a Scapegoat tree.
An update i.e. Insert or Delete will come with an example which is taken out from the Demo
for this project. So before going deep into the operations, let’s take a look at a Scapegoat
tree from Demo:
Figure 2.1: A sample Scapegoat tree from the Demo
Figure 2.1 presents a Scapegoat tree T with α = 0.57 of size 7, hα = 3 and T had
size(maxSize) 8 before node 10 was just deleted. These three indexes are at the top of
the tree. Notice that for each node in T, there are also three indexes surrounding it. The
index prefixed by s is the size of the subtree rooted at that node while the one prefixed by h
is the height of the node in T. The index prefixed by ha below each node is the hα of that
node.
8
2.1 How to search for a key
The Search operation in Scapegoat tree T is done like regular Search in a binary search tree.
As we will prove later that the height of a Scapegoat tree T is always loosely-height-balanced
in term of the number of nodes T.size after Insert or Delete operations or T.height <
⌊log1/α (T.size)⌋ + 1 then worst-case running time is O(log (T.size)) time. Below is the
simple recursive procedure Search:
Procedure 2.1.1 Search(root, k)
Input: root is the root of some tree T to search for an integer key k
Output: n is a node in T such that n.key = k or null if there is no such n
1: if root = null or root.key = k then
2: return root
3: else if k ≤ root.lef t.key then
4: return Search(root.lef t, k)
5: else
6: return Search(root.right, k)
7: end if
2.2 How to insert a new key
The Insert in Scapegoat tree T is also done like the regular Insert operation in a binary
search tree when T is still height-balanced. When T is not height-balanced then by Main
Theorem (3.1.1), a Scapegoat node s will be detected and rebuilding will be taken place at
the subtree rooted at s. Proof of the Main Theorem also shows how to find the Scapegoat
given newly inserted node n that makes T height-unbalanced. The Scapegoat s will be the
first ancestor of n such that s.height > s.hα. The procedure F indScapegoat(n) implemented
in the following will return the Scapegoat s giving the newly inserted deep node n:
After the Scapegoat s is detected if applicable, the procedure RebuildT ree(size, root) will
get called. RebuildT ree will rebuild a new 1/2-weight-balanced subtree from the subtree
rooted at Scapegoat node returned. Details of procedure RebuildT ree will be presented in
the rebuilding section. The Insert procedure also makes use of InsertKey(k) which is a
modified version of regular insertion in a binary search tree that will return the height for
the newly inserted node for comparison with T.hα to detect whether a newly inserted node
is a deep node or not:
Example 2.2.1. Figure 2.2 shows a Scapegoat tree T with α = 0.57 before inserting node
29. If node 29 is inserted, it would be the right child of node 22 and because the height of the
tree is 4 > T.hα = 3 then node 29 is a deep node. Scapegoat node 8 will be detected and the
whole tree T will be rebuilt into 1/2-weight-balanced binary search tree in Figure 2.3.
9
Procedure 2.2.1 F indScapegoat(n)
Input: n is a node and n 6= null
Output: A node s which is a parent of n and is a Scapegoat node
1: size = 1
2: height = 0
3: while (n.parent <> null) do
4: height = height + 1
5: totalSize = 1 + size + n.sibling.size()
6: if height > ⌊log1/α(totalSize)⌋ then
7: return n.parent
8: end if
9: n = n.parent
10: size = totalSize
11: end while
Figure 2.2: A Scapegoat tree before insertion
Figure 2.3: A Scapegoat tree after insertion
10
Procedure 2.2.2 Insert(k)
Input: The integer key k
Output: true if the insertion is successful, false if there exists a node n such that n.key = k
1: height = InsertKey(k)
2: if height = −1 then
3: return false;
4: else if height > T.hα then
5: scapegoat = F indScapegoat(Search(T.root, k))
6: RebuildT ree(n.size(), scapegoat)
7: end if
8: return true
2.3 How to delete a key
Deleting a node in a Scapegoat tree T is done by first deleting the node in a regular binary
search tree then compare T’s current size(T.size) to T’s maximum size(T.maxSize) that T
obtained since its latest total rebuilding. If T.size < α ∗ T.maxSize (1) then the whole tree
T will be rebuilt into 1/2-weight-balanced binary search tree and T.maxSize = T.size. The
intuition behind that is when (1) is satisfied, T might not be α-weight-balanced so it might
not be loosely α-height-balanced either. But we need to maintain T α-height-balanced or
loosely α-height-balanced then T needs to be rebuilt.
Procedure 2.3.1 Delete(k)
Input: The integer key k
Output: There is no node n in T such that n.key = k
1: deleted = DeleteKey(k)
2: if deleted then
3: if T.size < (T.α · T.maxSize) then
4: RebuildT ree(T.size, T.root)
5: end if
6: end if
Example 2.3.1. Figure 2.4 shows a Scapegoat tree before a series of deletions of node 9,
10, 16 with α = 0.57. As seen, the tree is still 0.57-height-balanced but after the series of
deletion it is loosely 0.57-height-balanced but not 0.57-height-balanced in Figure 2.5. But if
we remove one more node, node 1, the whole tree would be rebuilt into 1/2-weight-balanced
in Figure 2.6.
2.4 Rebuild Scapegoat Subtree
The procedures below are basically the same as the procedures of rebuilding 1/2-weightbalanced tree in the original paper :”Scapegoat Trees”[Igal Galperin and Ronald L. Rivest,
1993] [3] but with adaptation to my notations and an elimination of a dummy variable.
11
Figure 2.4: A Scapegoat tree before series of deletion
Figure 2.5: A Scapegoat tree after series of deletions
Figure 2.6: A Scapegoat tree after series of deletions and rebuilding
12
The procedures are presented here because later I will provide proofs of time complexity of
rebuilding procedures which are not clearly mentioned in the original paper. The idea of
rebuilding 1/2-weight-balanced tree is straight. First flatten tree into the list of nodes in
nondecreasing order of their keys. Then divide the list into three parts, among which the list
of first half number of nodes and the second half number of nodes will be recursively rebuilt
into 1/2-weight-balanced trees and the new root would be the middle node. Its left subtree
is the 1/2-weight-balanced tree returned from recursive call for first half list of nodes. Its
right subtree is the 1/2-weight-balanced tree returned from recursive call for second half list
of nodes. The procedure Flatten T ree(root, head) will return the list of nodes of a binary
search tree rooted at root in nondecreasing order of their keys appending by the list headed
by a node head. The nodes in the list returned are linked by the right fields:
Procedure 2.4.1 Flatten T ree(root, head)
Input: root is the root of some tree T
Output: The list of all nodes in T in nondecreasing order in terms of their keys headed by
node head. The node that contains the smallest key in the tree T will be returned.
1: if root = null then
2: return head
3: end if
4: root.right = Flatten T ree(root.right, head)
5: return Flatten T ree(root.lef t, root)
The procedure Build Height Balanced T ree(size, head) will build a 1/2-weight-balanced
tree from the flatten list of all the nodes in a binary search tree T. The procedure will return
the last node of the flatten list. Now, the procedure Rebuild T ree(size, scapegoat) just
makes use of Flatten T ree(root, head) procedure to flatten the subtree rooted at scapegoat
and Build Height Balanced T ree(size, head) to rebuild the flatten list into 1/2-weightbalanced binary search tree T. Because the call Build Height Balanced T ree(size, head)
will return the last node of the flatten list so in order to retrieve the root of the 1/2-weightbalanced tree then just traverse the parents of node head until we reach the root and this
could be done in O(log (T.size)) time because T is 1/2-weight-balanced then T is also 1/2-
height-balanced.
13
Procedure 2.4.2 Build Height Balanced T ree(size, head)
Input: The list of size nodes in nondecreasing order in terms of their keys headed by node
head
Output: A 1/2-weight-balanced tree built from the list above. The last node of the list will
be returned.
1: if size = 1 then
2: return head
3: else if size = 2 then
4: (head.right).lef t = head
5: return head.right
6: end if
7: root = (Build Height Balanced T ree(⌊(size − 1)/2⌋, head)).right
8: last = Build Height Balanced T ree(⌊(size − 1)/2⌋, root.right)
9: root.lef t = head
10: return last
Procedure 2.4.3 Rebuild T ree(size, scapegoat)
Input: A scapegoat node scapegoat that is the root of a subtree of size nodes
Output: A 1/2-weight-balanced subtree built from all the nodes of the subtree rooted at
scapegoat. The root of the rebuilt subtree will be returned
1: head = Flatten T ree(scapegoat, null)
2: Build Height Balanced T ree(size, head)
3: while head.parent!=null do
4: head = head.parent
5: end while
6: return head
14
Chapter 3
CORRECTNESS
3.1 Main Theorem
Theorem 3.1.1. If T is an α-weight-balanced binary search tree, then T is α-height-balanced
as well.
Proof. We could prove the other way around: If T is not α-height-balanced then T is not
α-weight-balanced either. If T is not α-height-balanced then there is node n0 such that
n0 > T.hα (2). Denote the deepest ancestor of n0 which is not α-height-balanced as ni or
let n1, ..., ni be ancestors of n0 such that n1 is the parent of n0 and so on, then ni
is the first
ancestor of n0 that satisfies (2). Such ni always exists because T.root always satisfies (2).
By the way of choosing such ni
, the following inequalities are satisfied:
i > ni
.hα
⇒ i > ⌊log1/α (ni
.size)⌋
⇒ i > log1/α (ni
.size)
(3)
and
i − 1 ≤ ni−1.hα
⇒ i − 1 ≤ ⌊log1/α (ni−1.size)⌋
⇒ i − 1 ≤ log1/α (ni−1.size)
⇒ 1 − i ≥ − log1/α (ni−1.size)
(4)
(3) + (4) gives
1 > log1/α (ni
.size) − log1/α (ni−1.size)
⇒ 1 > log1/α
ni
.size
ni−1.size
Due to 1
2 ≤ α < 1 or
1
α
> 1 and ni
.size > ni−1.size
Then
ni−1.size > α · ni
.size
Therefore, the node ni
is not α-weight-balanced or T is not α-weight-balanced.
15
The proof above shows us not only how to find the Scapegoat node s for rebuilding the
subtree rooted at s when a deep node n is detected but also such node s always exists. Let
d be the distance in term of edges from n to s, s is the deepest ancestor of n such that
d > s.hα.
3.2 Insert
Lemma 3.2.1. A 1/2-weight-balanced binary search tree T has the smaller than or equal
height of any binary search tree T
′ of the same size.
Proof. First notice that
⌊log2
(T
′
.size)⌋ ≤ T
′
.height
⇒ ⌊log2
(T.size)⌋ ≤ T
′
.height
Due to T.size = T
′
.size
And by Main Theorem (3.1.1), T is 1/2-height-balanced:
T.height ≤ ⌊log2
(T.size)⌋
Therefore,
T.height ≤ T
′
.height
Lemma 3.2.2. If T.root is not α-weight-balanced node then its heavy subtree contains at
least 2 more nodes than its light subtree.
Proof. Denote the sizes of the heavy, light subtrees of the root, and the whole tree by h,l,
and t, respectively. We have:
h + l + 1 = t
and
h > α · t
⇒ h > α · (h + l + 1)
⇒ h · (1 − α) > α · l + α
⇒ h ·
1 − α
α
> l + 1
Since 1/2 ≤ α < 1 or 1 <
1
α ≤ 2 and 0 < 1 − α ≤ 1/2 then 1−α
α < 1.
Therefore,
h > h ·
1 − α
α
> l + 1
and h and l are positive integers then h ≥ l + 2
Lemma 3.2.3. If T is not α-weight-balanced and T contains only one node at depth T.height
then rebuilding T decreases its height.
16
Proof. Let n be the only node at depth T.height. Let T
′
l
, T
′
h be the light and heavy subtrees
of a subtree T
′
rooted at Scapegoat node s which is α-weight-unbalanced and is an ancestor
of n, by proof of Main Theorem (3.1.1), s always exists. If n ∈ T
′
l
then T
′ after removing n
is not complete tree of height T
′
.height − 1 because T
′
h
contains at least 2 more nodes than
T
′
l by Lemma(3.2.2) but T
′
h
.height < T′
l
.height. In the other case n /∈ T
′
l
or n ∈ T
′
h
, T
′
is
still not complete subtree of height T
′
l
.height − 1 after removing n because T
′
h has at least
2 more nodes than T
′
l but T
′
h
.height > T′
l
.height. So rebuilding tree T results in rebuilding
T
′
into 1/2-weight-balanced binary search tree that causes it decrease its height by Lemma
(3.2.1) or T decreases its height.
Theorem 3.2.4. If a Scapegoat tree T was created from a 1/2-weight-balanced tree by a
sequence of Insert operations then T is α-height-balanced.
Proof. This proof is done by induction. If there are no Insert operations then by Main
Theorem (3.1.1), the theorem follows. Suppose that it is true that if a Scapegoat tree T
was created from a 1/2-weight-balanced tree by any sequence of n Insert operations then
T is α-height-balanced. We have to prove it also holds for an extra Insert done after those
sequences. If the extra Insert does not cause T to rebuild, T is still α-height-balanced because
it was and the height of newly inserted node is not over T.hα. If the extra Insert does cause
T to rebuild or the depth of the newly inserted node n: n.depth > T.hα and n is the only
node at depth T.hα + 1 then by Lemma (3.2.3), the theorem is followed.
3.3 Delete
Lemma 3.3.1. Let T be a α-weight-balanced binary search tree and let T’ be the tree after
inserting a node n into T then T
′
.height ≤ max(T
′
.hα, T.height)
Proof. If rebuilding is not triggered after the insertion of n, then the depth of n is at most
T
′
.hα or T
′
.height ≤ T
′
.hα or T
′
.height ≤ max(T
′
.hα, T.height). If the insertion of n causes
T to rebuild or n.depth > T′
.hα, there are still two cases to take care. In first case where there
were already some other nodes at n.depth, since rebuilding tree into 1/2-weight-balanced does
not make the tree deeper by Lemma (3.2.1) or T
′
.height ≤ T.height ≤ max(T
′
.hα, T.height).
In the other case where n is the only node at n.depth > T′
.hα, by Lemma (3.2.3), rebuilding
tree does not make the tree deeper, we’re done.
Lemma 3.3.2. Let T be a loosely α-height-balanced binary search tree such that T.height =
T.hα + 1 and let T
′
be the tree after insert a node n into T such that T
′
.hα = T.hα + 1 then
T
′
is α-height-balanced.
Proof. We have
T.height = T.hα + 1
T
′
.hα = T.hα + 1
then
T.height = T
′
.hα
17
Then Lemma (3.3.1) gives
T
′
.height ≤ max(T
′
.hα, T.height)
⇒ T
′
.height ≤ max(T
′
.hα, T′
.hα)
⇒ T
′
.height ≤ T
′
.hα
Therefore, T’ is α-height-balanced by Definition (1.3.1),
Lemma 3.3.3. A Scapegoat tree T built from a sequence of Insert and Delete operations
from an empty tree is always loosely α-height-balanced.
Proof. Divide the sequence of Insert and Delete operations into subsequences of consecutive
operations o1, ..., ok such that at the end of each sequence the tree has to be rebuilt and during
the executions of operations, no rebuilding is taken place. So we need to show that during
such sequence of operations, the tree is α-height-balanced or loosely α-height-balanced.
In the case that during any sequence of Insert and Delete operations, if T.hα does not
change then max(T.hα, T.height) is not increased and the tree is still at least loosely αheight-balanced. This is because Delete operation could not increase max(T.hα, T.height)
nor the Insert operation:
Let T
′ be the tree after inserting a node into T which is a tree after an operation oi (1 ≤ i ≤ k)
in the sequence. Now we have to prove that max(T
′
.hα, T′
.height) ≤ max(T.hα, T.height).
By Lemma (3.3.1):
T
′
.height ≤ max(T
′
.hα, T.height)
⇒ max(T
′
.hα, T′
.height) ≤ max(T
′
.hα, T.height)
⇒ max(T
′
.hα, T′
.height) ≤ max(T.hα, T.height)
In the other case, T.hα is changed during sequence of Insert and Delete operations. Denote
o
′
1
, ..., o′
l be the sequence of operations that change T.hα and let T
′ be the tree after inserting
a node into T which is a tree before some operation o
′
i
(1 ≤ i ≤ l). For an Insert operation,
if T is α-height-balanced then T
′
is loosely α-height-balanced because T.hα is increased at
most 1 and the same for T.height. Otherwise if T is loosely α-height-balanced but not αheight-balanced, by Lemma (3.3.2), T’ is α-height-balanced. For a Delete operation, notice
that there are no two consecutive Delete operations in that sequence. This is due to if a
Delete operation change T.hα, it will decrease T.hα by 1 or the cutting size is α·T.maxSize.
If there is another Delete operation right, and we know the second will again cause T.hα
decrease by 1 then rebuilding should be taken between those two Delete operations. This
violates our assumption. So a Delete operation should be performed on α-height-balanced
tree. As we know a Delete operation does not increase T.height nor Tα then the tree after
will be at most loosely α-height-balanced. This completes the proof.
18
Chapter 4
TIME COMPLEXITY
4.1 Find Scapegoat and Rebuild
Theorem 4.1.1. The time to find the Scapegoat node s is O(s.size) time.
Proof. The F indScapegoat(n) procedure is triggered when n, a newly inserted node, has
n.depth > T.hα. Let n0, n1, ..., ni be the sequence of accessors of n the procedure examined
where ni
is s, then all nodes n0, n1, ...ni are examined once. Moreover, all nodes in the other
subtrees of n0, n1, ...ni are examined once by size() procedure. Therefore, all nodes in the
tree rooted at s is examined once or the time to find the Scapegoat node s is O(s.size)
time.
Lemma 4.1.2. The call Flatten T ree(root, head) takes O(root.size) time.
Proof. Notice that every node of the tree will be visited at most one because Flatten T ree
is recursively called for two children of root and will not come back to the parent nodes.
Therefore, the worst-case complexity is O(root.size) time.
Lemma 4.1.3. The call Build Height Balanced T ree(size, head) takes O(size) time.
Proof. It is similar to the procedure Flatten T ree(root, head) where every node of the tree
will be visited at most one. Procedure Build Height Balanced T ree(size, head) is recursively called for 2 halves of the original list and will not be called to process the visited nodes
in the list. Therefore, the worst-case complexity is O(size) time.
Theorem 4.1.4. The worst-case complexity of Rebuild T ree(size, scapegoat) is O(size)
time.
Proof. The procedure makes use of two procedures Flatten T ree and
Build Height Balanced T ree whose worst-case complexity is O(size) time and at
the end, traversing parents of last node returned by procedure Flatten T ree to find the root
of newly rebuilt 1/2-weight-balanced tree which could be done in O(log (size)) time because
the tree is also 1/2-height-balanced by Main Theorem (3.1.1). Therefore, the worst-case
complexity of Rebuild T ree(size, scapegoat) is O(size) time.
19
4.2 Search
Theorem 4.2.1. Worst-case complexity of any Search operation done in Scapegoat tree T
is O(log (T.size)).
Proof. By Theorem (3.3.3), the Scapegoat tree T is loosely α-height-balanced i.e. T.height ≤
T.hα + 1 or T.height = O(log (T.size)) therefore, the worst-case complexity of Search operation is O(log (T.size)) time.
4.3 Insert
Theorem 4.3.1. If a Scapegoat tree T is built from a sequence of n Insert operations and
m Search or Delete operations starting with an empty tree, then the amortized cost of Insert
is O(log n) time.
Proof. The proof is done using accounting method in the amortized analysis chapter in [2]
starting with accounting function defined for each node in a Scapegoat tree:
Φn =
(
0 if |(n.lef t).size − (n.right).size| ≤ 1
|(n.lef t).size − (n.right).size| otherwise
By that way, we have Φn = 0 when n is 1/2-weight-balanced node. When n is not
α-weight-balanced node or a Scapegoat node and suppose that (n.lef t).size > α · n.size,
we have:
Φn = |(n.lef t).size − (n.right).size|
⇒ Φn = |(n.lef t).size − (n.size − (n.lef t).size − 1)|
⇒ Φn = |2 · (n.lef t).size − n.size + 1|
⇒ Φn = 2 · (n.lef t).size − n.size + 1
⇒ Φn ≥ (2 · α − 1) · n.size + 1
and
Φn = |(n.lef t).size − (n.right).size|
⇒ Φn = |(n.size − (n.right).size − 1) − (n.right).size|
⇒ Φn = |(n.size − 2 · (n.right).size + 1|
⇒ Φn ≤ n.size
Therefore, Φn = O(log(n.size)). Now, in the first case that Insert operation i
th of node n
did not trigger rebuilding:
