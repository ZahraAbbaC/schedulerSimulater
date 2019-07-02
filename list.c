#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

struct Task {
    int id;
    int r;//arrival_time
    int s;//start scheduling
    int c;//execution_time
    int d;//deadline
    int ad;//absolute_deadline
    float u;//utilization
    int execution_count;
    bool success;
};
struct Node{
    struct Task task;
    struct Node * next;
};

//
//struct Node * add_Task(struct Node* head, struct Task * t)
//{
//    struct Node* current;
//    struct Node* temp = (struct Node *) malloc (sizeof(struct Node));
//    /* Special case for the head end */
//    if (head == NULL || (head)->Task->r >= t->r)
//    {
//        temp->next = head;
//        temp->Task = t;
//        head = temp;
//    }
//    else
//    {
//        /* Locate the Node before the point of insertion */
//        current = head;
//        while (current->next!=NULL &&
//               current->next->Task->r < t->r)
//        {
//            current = current->next;
//        }
//        temp->next = current->next;
//        current->next = temp;
//    }
//    return head;
//}

//struct Node * add(struct Node * head, struct Task *t){
//
//    struct Node * first = head;
//    struct Node * prev = NULL;
//    struct Node * temp = (struct Node *) malloc (sizeof(struct Node));
//
//    if (head == NULL){
//        head = (struct Node *) malloc(sizeof(struct Node));
//        head -> task = t;
//        head -> next = NULL;
//        return head;
//    }
//    else{
//        if (t->r<head->task->r) {
//            temp->task = t;
//            temp->next = head;
//            return temp;
//        }
//        while(t->r >= head->task->r){
//            if(head->next==NULL){
//                temp->task=t;
//                head->next = temp;
//                return first;
//            }
//            prev=head;
//            head=head->next;
//        }
//        temp->task=t;
//        if(prev) temp->next=prev->next;
//        else temp->next=first;
//        if(prev) prev->next=temp;
//        else return temp;
//        return first;
//    }
//}


struct Node * add_task(struct Node * head, struct Task t){

    struct Node * first = head;
    struct Node * prev = NULL;
    struct Node * temp = (struct Node *) malloc (sizeof(struct Node));

    if (head == NULL){
        head = (struct Node *) malloc(sizeof(struct Node));
        head -> task = t;
        head -> next = NULL;
        return head;
    }
    else{
        if (t.r<head->task.r) {
            temp->task = t;
            temp->next = head;
            return temp;
        }
        while(t.r >= head->task.r){
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

struct Node * add_to_ready(struct Node * head, struct Task t){

    struct Node * first = head;
    struct Node * prev = NULL;
    struct Node * temp = (struct Node *) malloc (sizeof(struct Node));

    if (head == NULL){
        head = (struct Node *) malloc(sizeof(struct Node));
        head -> task = t;
        head -> next = NULL;
        return head;
    }
    else{
        if (t.ad<head->task.ad) {
            temp->task = t;
            temp->next = head;
            return temp;
        }
        while(t.ad >= head->task.ad){
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

struct Node * rmv(struct Node * head, struct Task t){
    struct Node * first = head;
    struct Node * prev = head;

    for(;head; prev=head,head=head->next){
        if(t.id==head->task.id){
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
