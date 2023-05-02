## Problem: Ukladanka
(C++)
### Problem statement
Given initially an empty n x m board consisting of n*m boxes and k blocks. Each block is assigned an identifier specified by a capital letter of the English alphabet and is a coherent set of boxes, with two boxes adjacent if they share a common side. Write a program to check whether a certain number of blocks can be placed on the board so that each field of the board is covered by exactly one block. No block can project beyond the board. Each block can be used at most once. The blocks can be rotated in multiples of 90 degrees. If the board can be covered, the result is to cover the board.

You may assume that 1≤n,m,k≤10. In tests worth a total of 4 points there is an additional k≤8 . Among them, in tests worth a total of 3 points there is an additional k≤5 .

#### Input

The first line contains the numbers n, m, k separated by spaces. Next follows a description of k blocks. The i-th block is described as n lines of m characters each, without spaces; a dot denotes a field not belonging to the block, and the i-th capital letter of the alphabet denotes a field belonging to the block. There is a line break between the descriptions of successive blocks.

#### Output

The first line of the output should contain one word YES or NO. The word YES means that the desired board coverage exists, and the word NIE means that it does not. If the answer is YES, then further on there should be a description of the covered board, in the form of n lines of m characters. Each character should be a capital letter denoting the identifier of the block to be covered. If there are multiple valid ways to cover, one arbitrary one should be output.

#### Example input
```
4 5 4
A....
A....
AAAAA
.....

..B..
.BBB.
.BBB.
.....

.....
.CC..
.CC..
.....

.....
.DD..
..D..
.DDD.
```

#### Example output
```
YES
AAAAA
BBDDA
BBBDA
BBDD
```

Translated with www.DeepL.com/Translator (free version)