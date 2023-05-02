## Problem: Kolejki
(C++)
### Problem statement
Your task is to effectively simulate the events that occur during an office working day. There are m windows in an office, numbered from 0 to m-1. There is a queue of visitors to each window. When a clerk is free, he serves the first person in the queue, who then leaves and does not return that day. In addition, various other situations occur in the office, as described by the functions below. Your task is to implement these functions.

void open_office(int m) - This function will only be called once, first. Calling this function tells your library that there are m windows in the office (and one special window - more on that later). There is no interested party in the office yet.

interesant *new_interested(int k) - A new interested party enters the office and immediately queues at the end of the queue at window number k. The function should create a new element of type interested and give a pointer to this element as a result.

int number(interesant* i) - Returns the number of interesant i. Every interested party gets a number right away when they come to the office. The numbers start from 0 and are consecutive integers.

interester *obsluz(int k) - The clerk serving window number k tries to serve the next interester. If the queue for the window is non-empty, the first interested party in the queue is served and leaves the office; then the result of the function should be a pointer to that interested party. Otherwise, no one is served and the result of the function should be NULL.

void change_window(stakeholder *i, int k) - Stakeholder i realises that he is queuing for the wrong window. He leaves his queue and queues at the end of the queue for window k, which he now thinks is the correct one. You can assume that just before this function was called, stakeholder i was queued in some queue and it was not the queue for window k.

void close_window(int k1, int k2) - The clerk manning window number k1 goes on break. All customers queuing for this window are directed to window number k2 and queue up at the end of the queue at window k2 in the same order as they queued for window k1. We do not know how long the clerk's interruption will last; it may be that there are customers queuing at window k1 on the same day and that they are served.

std::vector<interest*> fast_track(interester *i1, interester *i2) - The clerk opens a special window for a while, where all cases can be handled quickly. A group of interested parties who have been queuing up to now realises what is happening and immediately moves to the special window, where they are immediately served in the same order in which they stood in their previous queue, after which they leave the office and the special window closes. The group of interested parties is given by the first (i1) and last (i2) interested party in the group; if i1=i2, only interested party i1 is served. The result of the function should be a vector of the interests served consecutively in the special window, in order of service.

void chief(int k) - Time and again, the head of the office looks out of his office and out of boredom rearranges the people queued up. More precisely, each time he chooses one window number k and orders all the people queued for that window to reverse their queue order. Although such an operation is essentially incomprehensible, the interested parties follow the warden's orders immediately for fear of getting an even stranger idea. If there was at most one interested party in the queue for that window, the warden's operation has no effect.

std::vector<interest*> close_office() - This function will be called once, at the end of the interaction with the library. It marks the end of the office day. All remaining interested parties are quickly served and the office closes for the day. The result of the function should be a vector of all the visitors who have stayed in the office up to that point, in order: first all those queuing for window 0 (in order of service), then all those queuing for window 1 (in order of service), and so on.

The declarations of the given functions can be found in the kol.h file. Your task is to complete the definition of the struct type stakeholder in the kol.h file (without changing anything else in this file) and to implement the given functions in the kol.cpp file.

Compile command:
g++ @optionsCpp main.cpp kol.cpp -o main.e.

In order for your solution to receive the maximum score, the time cost of each function must be proportional to the size of the parameters and output. The exception is the open_office function, which can run in O(m) time.

Translated with www.DeepL.com/Translator (free version)