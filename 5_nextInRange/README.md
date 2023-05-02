## Problem: Nastepne w zakresie
(C++)
### Problem statement
Note: The complexity of the init function should be at worst O(|X|log2|X|).

Your task is to implement a data structure that allows you to efficiently answer specific queries about a sequence of numbers X.

For a fixed sequence X=x0,x1,...,xn-1 we are interested in the function:
```
nextInRange(i, [a, b])
```
which determines the smallest index j such that j≥i and xj∈[a,b]. If no such index exists, the result of the function should be -1.
That is:
```
nextInRange(i,[a,b])=min{j≥i:xj∈[a,b]}
```
or -1 (if no such index exists)

Your module should provide the following functions:

`void init(const vector<int> &x)` - initialise the string X by the values of the vector x (note! the string can contain any values that fall into the int type)

`int nextInRange(int i, int a, int b)` - calculate the value of nextInRange(i, [a, b]). You can assume that 0≤i<|X| and INT_MIN≤a≤b≤INT_MAX .

`void done()` - release all memory used to handle the string X.

The declarations of the given functions can be found in the file next.h. Your task is to implement the given functions in the next.cpp file.

Compile command: `g++ @optionsCpp main.cpp next.cpp -o main.e`

For your solution to get the maximum score, the time cost of the nextInRange function must be O(log|X|). If the time cost of this function is O(log2|X|), you can receive 4 points.

Translated with www.DeepL.com/Translator (free version)