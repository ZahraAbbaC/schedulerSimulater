//
// Created by zahra on 6/30/19.
//

#ifndef PROJECT1_LIST_H
#define PROJECT1_LIST_H
struct node * add_task(struct node * head, struct task *t);
struct node * add_to_ready(struct node * head, struct task *t);
struct node * rmv(struct node * head, struct task *t);
#endif //PROJECT1_LIST_H
