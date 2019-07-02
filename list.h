//
// Created by zahra on 6/30/19.
//

#ifndef PROJECT1_LIST_H
#define PROJECT1_LIST_H

struct Task {
    int id;
    int r;//arrival time
    int s;//start scheduling
    int c;//execution time
    int d;//deadline
    int ad;//absolute deadline
    float u;//utilization
    int execution_count;
    bool success;
};

struct Node {
    struct Task task;
    struct Node * next;
};

struct Node * add_task(struct Node * head, struct Task t);
struct Node * add_to_ready(struct Node * head, struct Task t);
struct Node * rmv(struct Node * head, struct Task t);
#endif //PROJECT1_LIST_H
