#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

struct task {
    int r;//arrival_time
    int s;//start scheduling
    int c;//execution_time
    int d;//deadline
    int ad;//absolute_deadline
    float u;//utilization
    int execution_count;
    bool success;
};
struct node{
    struct task *task;
    struct node * next;
};
struct node * add_task(struct node * head, struct task *t){

    struct node * first = head;
    struct node * prev = NULL;
    struct node * temp = (struct node *) malloc (sizeof(struct node));

    if (head == NULL){
        head = (struct node *) malloc(sizeof(struct node));
        head -> task = t;
        head -> next = NULL;
        return head;
    }
    else{
        if (t->r<head->task->r) {
            temp->task = t;
            temp->next = head;
            return temp;
        }
        while(t->r >= head->task->r){
            if(head->next==NULL){
                temp->task=t;
                head->next = temp;
                return first;
            }
            prev=head;
            head=head->next;
        }
        temp->task=t;
        if(prev) temp->next=prev->next;
        else temp->next=first;
        if(prev) prev->next=temp;
        else return temp;
        return first;
    }
}
struct node * add_to_ready(struct node * head, struct task *t){

    struct node * first = head;
    struct node * prev = NULL;
    struct node * temp = (struct node *) malloc (sizeof(struct node));

    if (head == NULL){
        head = (struct node *) malloc(sizeof(struct node));
        head -> task = t;
        head -> next = NULL;
        return head;
    }
    else{
        if (t->ad<head->task->ad) {
            temp->task = t;
            temp->next = head;
            return temp;
        }
        while(t->ad >= head->task->ad){
            if(head->next==NULL){
                temp->task=t;
                head->next = temp;
                return first;
            }
            prev=head;
            head=head->next;
        }
        temp->task=t;
        if(prev) temp->next=prev->next;
        else temp->next=first;
        if(prev) prev->next=temp;
        else return temp;
        return first;
    }
}

struct node * rmv(struct node * head, struct task *t){
    struct node * first = head;
    struct node * prev = head;

    for(;head; prev=head,head=head->next){
        if(t==head->task){
            if (head==first){
                free(head);
                return first->next;
            }
            prev->next = head->next;
            free(head);
            return first;
        }
    }
}
