#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"
#define TETA 0.5
#define NUMBER_OF_CORES 4

struct task {
    int id;
    int r;//arrival time
    int s;//start scheduling
    int c;//execution time
    int d;//deadline
    int ad;//absolute deadline
    int p;//progress
    float u;//utilization
    int execution_count;
    bool success;
};

struct node {
    struct task * task;
    struct node * next;
};

struct core{
    struct task *task;
    bool busy;
    bool faulty;
};

int time = -1;
struct core cores[NUMBER_OF_CORES];
int number_of_cores = NUMBER_OF_CORES;
int number_of_tasks;
struct node * list;
struct node * ready_list;

struct node * get_list(struct node * head){
   
    FILE* file = fopen("/home/zahra/Uni/OS/project1/tasks.txt","r");
    if(file==NULL)perror("tasks.txt");
    int i,r,c,d;
    head = NULL;
    for (i=0; 0<fscanf(file,"%d%d%d",&r,&c,&d) ;i++ ) {
        struct task * t = (struct task *) malloc (sizeof(struct task));
        t->r=r; t->c=c; t->d=d; t->ad=t->r+t->d; t->p=0;
        t->u=(float)t->c/(float)t->d; t->id = i+1;
        t->execution_count=1;t->success=false;
        head=add_task(head,t);
    }
    fclose(file);
    number_of_tasks = i;
    return head;
}

void update_tc(struct task * t){
    float f = (float) 1.2 * t->c;
    int i = (int) f;
    if(f>i)t->c = i+1;
    else t->c = i;
}

struct node * update_ready_list(){
    struct node * l = list;
    for(; time>=l->task->r; l=l->next){
        ready_list = add_to_ready(ready_list,l->task);
        list = rmv(list,list->task);
        if (!(l->next))break;
    }
    return ready_list;
}

void update_cores(){
    for(int i=0; i<number_of_cores; i++){
        if(cores[i].busy) {
            if ( time - cores[i].task->s >= cores[i].task->c ) {//execution finished
                printf("task %d finished executing on core %d \n", cores[i].task->id, i);
                cores[i].task = NULL;
                cores[i].busy = false;
                cores[i].faulty = false;
            }
        }
    }

}

bool core_fault (int core){
    int r =rand()%number_of_cores;
    if (r == core) return true;
    if (cores[core].busy) cores[core].task->success=false;
    return false;
}

bool execute(struct task *task,int core){
    cores[core].task = task;

    cores[core].busy = true;
    if(core_fault(core)) {
        cores[core].faulty = true;
        return false;//add to ready list and inc execution_count
    }
    task->success = true;
    return true;
}

int find_idle_core(){
    int i;
    for (i=0; i<number_of_cores; i++)
//        if (!(cores[i].task)){
        if (!(cores[i].busy)) {
            return i;//i'th core is idle
        }
    return -1;
}

int min_execution_time() {
    int min = time - cores[0].task->s;
    for(int i=0; i<number_of_cores; i++) {

        if (min < (time - cores[i].task->s ))
            min = time - cores[i].task->s;
    }
}
//
//int min_execution_time(){
//    int min = cores[0].task->c;
//    for(int i=0; i<number_of_cores; i++){
//        if (cores[i].busy) {
//            if (min > cores[i].task->c) min = cores[i].task->c;min = time = cores[i].task->s;
//        }
//    }
//    return min;
//}

void end(){
    //check cores
    int c,i;
    int min;

    if (cores[0].busy) min = time - cores[0].task->s;
    for(i=0; i<number_of_cores; i++){
        if(cores[i].busy) {
            if (min < (time - cores[i].task->s)) {
                min = time - cores[i].task->s;
                c = i;
            }
        }
    }
    time += min;
    cores[c].busy = false;
    cores[c].task = NULL;
    cores[c].faulty = false;
    printf("--- time : %d ------\n", time);
//    printf("core %d finished executing task %d\n", c, cores[c].task->id);
}
void print_list(){
    struct node * l=list;
    printf("list:\n");
    for(;l;l=l->next)printf("r: %d, ad: %d\n",l->task->r, l->task->ad);
}
void print_ready(){
    struct node * f=ready_list;
    printf("readylist:\n");
    for(;f;f=f->next)printf("r: %d, ad: %d\n",f->task->r, f->task->ad);
}

//TODO:akhare kar time bayad beshe be andaze i ke akharin task run she
int main() {

    int inc = 1;
    list = NULL;
    list = get_list(list);

    int executed_tasks=0;
    int core,core2;
    bool feasibility = true;
    float task_execution_time;

    while(list && list->task!=NULL && executed_tasks<number_of_tasks) {/*list is not empty and total executed tasks!=number of total tasks*/
        time += inc;
        printf("-----time: %d -----------\n",time);
        print_list();
        update_ready_list();
        print_ready();
        update_cores();
        //return 0;
        while (ready_list && ready_list->task) {
            struct task *t = ready_list->task;//smallest ad
            printf("task: %d", t->id);
            core = find_idle_core();
            if (core != -1) {//if there is an idle core
                inc = 1;
                if (t->execution_count == 1) {//first time
                    if (time + t->c > t->ad){
                        printf("deadline expired\n");
                        ready_list = rmv(ready_list,t);
                        feasibility = false;
                    }
                    else {
                        if (t->u < TETA) {//not critical - needs one core
                            printf("(not critical)\n");
                            if (!execute(t, core)) {//the core is faulty - checkpointing
                                printf("checkpointing\n");
                                update_tc(t);
                            }
                            printf("start execution on core %d\n", core);
                            t->s = time;
                            ready_list = rmv(ready_list,t);
                            executed_tasks+=1;
                        } else {//critical
                            printf("(critical)\n");
                            ready_list = rmv(ready_list,t);
                            execute(t, core);
                            executed_tasks+=1;
                            printf("start execution on core %d\n", core);
                            t->s = time;
                            t->execution_count += 1;
                            core2 = find_idle_core();
                            if (core2 != -1){//there exists another idle core
                                printf("duplicate on core %d\n", core2);
                                execute(t, core2);
                                printf("start execution on core %d\n", core2);
                                t->s = time;
                            } else {
                                printf("duplicate in readylist\n");
                                add_to_ready(ready_list, t);
                                //executed_tasks -=1;
                            }
                        }
                    }
                } else if (t->execution_count > 1) {//second try (execution)
                    printf("critical\n");
                    if (t->success){
                        printf("first try was successful\n");
                        ready_list = rmv(ready_list,t);
                    }
                    else {
                        printf("(second try)\n");
                        if (time + t->c < t->ad) {
                            printf("deadline expired");
                            execute(t, core);
                            //executed_tasks+=1;
                            t->s = time;
                            if (!t->success){feasibility = false;printf("both primary and backup copy of the critical task are faulty\n");}
                            ready_list = rmv(ready_list,t);
                        } else feasibility = false;
                    }
                }

            } else{//all cores are busy
                printf("all cores are busy\n");
                inc = min_execution_time();
            }
        }

    }

//    for(int a=0; a < number_of_cores; a++) {
//        if (cores[a].busy)
//            end();
//    }

    for(int a=0; a < number_of_cores; a++){
        //time += 1;
        printf("--- time : %d -----------\n", time);
        update_cores();
    }

    printf("feasibility: ");
    if(feasibility) printf("true");
    else printf("false");

    return 0;
}