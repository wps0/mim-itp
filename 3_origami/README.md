## Problem: Origami
(C)
### Problem statement
Write a program for origami fans to investigate how many layers a cleverly put together piece of paper has at a given point. The program should load a description of the creation of successive origami cards and questions about the number of layers at the given points of the cards from stdin. In the first line of input there are two numbers n and q denoting the number of cards and the number of questions respectively. Then in the input there are n lines describing successive cards and q questions about the number of layers in the set points of the given cards.

The description of the i-th card (1 ≤ i ≤ n) is in line i + 1. This description has one of three forms:

    P x1 y1 x2 y2
    K x y r
    Z k x1 y1 x2 y2

The first form represents a closed rectangle with sides parallel to the axis of the coordinate system with the lower left corner at point P1 = (x1, y1) and the upper right corner at point P2 = (x2, y2). Point P1 must therefore be out of focus to the left and down from point P2. When a pin is driven into this card inside (or on the edges of) the rectangle, the card will be pierced 1 time, 0 times otherwise.

The second figure represents a closed circle with centre at point (x, y) and radius r.

The third form represents a sheet of paper formed by bending the kth sheet (1 ≤ k < i) along a straight line passing through the points P1 = (x1, y1) and P1 = (x2, y2) - these must be different points. The paper is folded in such a way that from the right side of the straight line (looking in the direction from P1 to P2) it is translated to the left side. For a paper folded in this way, a puncture on the right side of the straight gives 0 layers; a puncture exactly on the straight should return as much as a puncture of the paper before folding; on the left side, as much as before folding plus a puncture of the unfolded paper at the point that overlapped the puncture point.

The j-th query (1 ≤ j ≤ q) is in line n + j + 1 and is of the form k x y, where k is the card number (1 ≤ k ≤ n) and (x, y) is the point at which we drive the pin to determine the number of layers of the k-th card at that point.

The program should output q lines to stdout - answers to queries about the number of layers.

Notes: The numbers denoting the coordinates of the points or the radius of the circle are floating-point numbers. There is no restriction on the time complexity of the solution. In particular, exponential solutions will be allowed. Our tests are not malicious due to the precision of the calculations.

#### Example input
```
4 5
P 0 0 2.5 1
Z 1 0.5 0 0.5 1
K 0 1 5
Z 3 0 1 -1 0
1 1.5 0.5
2 1.5 0.5
2 0 0.5
3 4 4
4 4 4
```

#### Example output
```
1
0
2
1
2
```
You may assume that 1≤n,m,k≤10. In tests worth a total of 4 points there is an additional k≤8 . Among them, in tests worth a total of 3 points there is an additional k≤5 .

Translated with www.DeepL.com/Translator (free version)