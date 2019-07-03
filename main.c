#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"
#define TETA 0.5
#define NUMBER_OF_CORES 3

struct core{
    struct Task task;
    bool busy;
    bool faulty;
};

int time = -1;
struct core cores[NUMBER_OF_CORES];
int number_of_cores = NUMBER_OF_CORES;
int number_of_Tasks;
struct Node * list;
struct Node * ready_list;

struct Node * get_list(struct Node * head){
   
    FILE* file = fopen("/home/zahra/Uni/OS/project1/tasks.txt","r");
    if(file==NULL)perror("tasks.txt");
    int i,r,c,d;
    head = NULL;
    for (i=0; 0<fscanf(file,"%d%d%d",&r,&c,&d) ;i++ ) {
        struct Task t;
        t.r=r;
        t.c=c;
        t.d=d;
        t.ad=t.r+t.d;
        //t->p=0;
        t.u=(float)t.c/(float)t.d;
        t.id = i+1;
        t.execution_count=1;t.success=false;
        head=add_task(head,t);
    }
    //
    struct Node * h=head;
    printf("first list : \n");
    for (;h;h=h->next)printf("r: %d  ad: %d\n", h->task.r, h->task.ad);
    //
    fclose(file);
    number_of_Tasks = i;
    return head;
}

void update_tc(struct Task * t){
    float f = (float) 1.2 * t->c;
    int i = (int) f;
    if(f>i)t->c = i+1;
    else t->c = i;
}

struct Node * update_ready_list(){
    struct Node * l = list;
    for(; time>=l->task.r; l=l->next){
        ready_list = add_to_ready(ready_list,l->task);
        list = rmv(list,list->task);
        if (!(l->next))break;
    }
    return ready_list;
}

void update_cores(){
    for(int i=0; i<number_of_cores; i++){
        if(cores[i].busy) {
            if ( time - cores[i].task.s >= cores[i].task.c ) {//execution finished
                printf("--- time : %d -----------\n",time);
                printf("Task %d finished executing on core %d \n", cores[i].task.id, i);
                //cores[i].task = NULL;
                cores[i].busy = false;
                cores[i].faulty = false;
            }
        }
    }

}

bool core_fault (int core){
    int r =rand()%number_of_cores;
    if (r == core) return true;
    if (cores[core].busy) cores[core].task.success=false;
    return false;
}

bool execute(struct Task task,int core){
    cores[core].task = task;
    cores[core].busy = true;
    if(core_fault(core)) {
        cores[core].faulty = true;
        return false;//add to ready list and inc execution_count
    }
    task.success = true;
    return true;
}

int find_idle_core(){
    int i;
    for (i=0; i<number_of_cores; i++)
//        if (!(cores[i].Task)){
        if (!(cores[i].busy)) {
            return i;//i'th core is idle
        }
    return -1;
}

int min_execution_time() {//finds the minimum remained execution time
    int min = 0;
    if(cores[0].busy){
        min = cores[0].task.c - (time - cores[0].task.s);//execution time - progress
    }
    for(int i=1; i<number_of_cores; i++) {
        if(cores[i].busy) {
            if (min > (cores[i].task.c - (time - cores[i].task.s)))
                min = (cores[i].task.c - (time - cores[i].task.s));
        }
    }
    return min;
}
//
//int min_execution_time(){
//    int min = cores[0].Task->c;
//    for(int i=0; i<number_of_cores; i++){
//        if (cores[i].busy) {
//            if (min > cores[i].Task->c) min = cores[i].Task->c;min = time = cores[i].Task->s;
//        }
//    }
//    return min;
//}

void finish(){
    int inc = min_execution_time();
    if(inc == 0 )return;
    else {
        time += inc;
        update_cores();
    }
}

void print_list(){
    struct Node * l=list;
    printf("list:\n");
    for(;l;l=l->next)printf("id: %d, r: %d, ad: %d\n",l->task.id, l->task.r, l->task.ad);
}
void print_ready(){
    struct Node * f=ready_list;
    printf("readylist:\n");
    for(;f;f=f->next)printf("id: %d, r: %d, ad: %d\n",f->task.id, f->task.r, f->task.ad);
}

//TODO:akhare kar time bayad beshe be andaze i ke akharin Task run she
int main() {

    int inc = 1;
    list = NULL;
    list = get_list(list);

    int executed_Tasks=0;
    int core,core2;
    bool feasibility = true;
    float Task_execution_time;

//TODO:fek konam bayad or bashe na and
    while(list /* && list->task!=NULL*/ && executed_Tasks<number_of_Tasks) {/*list is not empty and total executed Tasks!=number of total Tasks*/
        time += inc;
        printf("-----time: %d -----------\n",time);
        print_list();
        update_ready_list();
        print_ready();
        update_cores();
        //return 0;
        while (ready_list /*&& ready_list->task*/) {
            struct Task t = ready_list->task;//smallest ad
            printf("Task: %d", t.id);
            core = find_idle_core();
            if (core != -1) {//if there is an idle core
                inc = 1;
                if (t.execution_count == 1) {//first time
                    if (time + t.c > t.ad){
                        printf("deadline expired\n");
                        ready_list = rmv(ready_list,t);
                        feasibility = false;
                    }
                    else {
                        if (t.u < TETA) {//not critical - needs one core
                            printf("(not critical)\n");
                            if (!execute(t, core)) {//the core is faulty - checkpointing
                                printf("checkpointing\n");
                                update_tc(&t);
                            }
                            printf("start execution on core %d\n", core);
                            t.s = time;
                            ready_list = rmv(ready_list,t);
                            executed_Tasks+=1;
                        } else {//critical
                            printf("(critical)\n");
                            ready_list = rmv(ready_list,t);
                            execute(t, core);
                            executed_Tasks+=1;
                            printf("start execution on core %d\n", core);
                            t.s = time;
                            t.execution_count += 1;
                            core2 = find_idle_core();
                            if (core2 != -1){//there exists another idle core
                                printf("duplicate on core %d\n", core2);
                                execute(t, core2);
                                printf("start execution on core %d\n", core2);
                                t.s = time;
                            } else {
                                printf("duplicate in readylist\n");
                                add_to_ready(ready_list, t);
                                //executed_Tasks -=1;
                            }
                        }
                    }
                } else if (t.execution_count > 1) {//second try (execution)
                    printf("critical\n");
                    if (t.success){
                        printf("first try was successful\n");
                        ready_list = rmv(ready_list,t);
                    }
                    else {
                        printf("(second try)\n");
                        if (time + t.c < t.ad) {
                            printf("deadline expired");
                            execute(t, core);
                            //executed_Tasks+=1;
                            t.s = time;
                            if (!t.success){feasibility = false;printf("both primary and backup copy of the critical Task are faulty\n");}
                            ready_list = rmv(ready_list,t);
                        } else feasibility = false;
                    }
                }

            } else{//all cores are busy
                printf("--- time : %d -----------\n", time);
                printf("all cores are busy\n");
                inc = min_execution_time();
                time += inc;
                update_cores();
            }
        }

    }

//    for(int a=0; a < number_of_cores; a++) {
//        if (cores[a].busy)
//            end();
//    }

    finish();



//    for(int a=0; a < number_of_cores; a++){
//
//        printf("--- time : %d -----------\n", time);
//        update_cores();
//    }

    printf("feasibility: ");
    if(feasibility) printf("true");
    else printf("false");

    return 0;
}