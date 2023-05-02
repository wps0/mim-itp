## Problem: Trzy rozne
(C)
### Problem statement
Along the motorway, which we can imagine as a straight line, n motels are distributed. We number the motels from 1 to n sequentially along the highway. Each motel belongs to some network of motels, which we describe with a number from 1 to n.

Bytie tells Bytie that while driving along the motorway he stopped three times for the night at a motel, each time staying at a motel of a different chain. Bitek wonders how far apart the three motels must have been. He is interested in the nearest and furthest three motels.

Formally, Bitek would like to determine the three motels A, B, C located in that order along the highway and belonging to different chains, for which:

    the maximum from the distance from A to B and from B to C is as small as possible (the nearest three),
    the minimum of the distances from A to B and from B to C is as large as possible (the furthest three).

Write a program that reads in from stdin the number n of motels, then the descriptions of the motels in order along the highway - for each motel its chain number and distance from the beginning of the highway - and writes out to stdout two numbers - as above, the maximum from the minima of the distances and the minimum from the maxima of the distances of the three motels belonging to different chains. If Bytie is mistaken and no three motels of different chains occur along the motorway, both numbers should be zero. The exact format of the input and output is described in the example below (the ends are the ends of the line).

You may assume that the number of motels will be positive and will not exceed one million, and that all numbers in the input will be non-negative and will fit in the int type. It may happen that several motels are located at the same point along the motorway, or even that Bytie has stayed overnight in two or three such motels.

Only solutions with linear time complexity can count on the maximum score. A solution that writes only one of the two required numbers correctly can receive half the points for the task.

#### Input
```
9
1 2
2 6
2 9
1 13
1 17
3 20
1 26
3 27
1 30
```

#### Output
```
7 10
```

Explanation: Motels 3, 4 and 6 (those at distances 9, 13 and 20 from the start of the motorway) all belong to different networks (2, 1 and 3 respectively) and minimise the maximum from the distance. Motels 2, 6 and 9 (those at distances 6, 20 and 30 from the start of the motorway) all belong to different networks (2, 3 and 1 respectively) and maximise the minimum from distance.

Translated with www.DeepL.com/Translator (free version)