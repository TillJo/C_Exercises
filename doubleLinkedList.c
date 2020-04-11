#include "doubleLinkedList.h"
#include <stdio.h>


extern void list_init(struct listHead *head){
    head->next = head;
    head->prev = head;
}

extern void list_add(struct listHead *new, struct listHead *head){
    new->prev = head;
    new->next = head->next;
    head->next->prev = new;
    head->next = new;
}

extern void list_add_tail(struct listHead *new, struct listHead *head){
    new -> prev = head->prev;
    head->prev->next = new;
    head->prev = new;
    new->next = head;
}

extern struct listHead* list_del(struct listHead *entry){
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = 0;
    entry->next = 0;
    return entry;
}

extern void list_move(struct listHead *entry, struct listHead *head){
    struct listHead *new = list_del(entry);
    list_add(new, head);
}

extern void list_move_tail(struct listHead *entry, struct listHead *head){
    struct listHead *new = list_del(entry);
    list_add_tail(new, head);
}

extern int list_empty(struct listHead *head){
    if((head->prev=head) && (head->next==head)){
        return 1;
    } else {
        return 0;
    }
}
extern void print_list(struct listHead *head){

    struct listHead* iterator = head;
    int counter = 0;

    while (iterator->next != head) {
        printf( "%dth-element with pid: %i ", counter, ((struct procInfo*) iterator->next)->pid);
        iterator = iterator->next;
        counter++;
    }

    printf("\n");
}


/**
 * Only exits for test cases.
 */
int main(int argc, char const *argv[]){

    struct procInfo frist;
    struct procInfo secound;
    struct procInfo third;
    struct procInfo fourth;
    struct procInfo fifth;

    struct listHead anker;
    struct listHead ankerForEmptyTest;

    frist.pid = 1;
    secound.pid = 2;
    third.pid = 3;
    fourth.pid = 4;
    fifth.pid = 5;


    list_init(&anker);
    list_add_tail(&frist.head, &anker);
    list_add_tail(&secound.head, &anker);
    list_add_tail(&third.head, &anker);
    list_add_tail(&fourth.head, &anker);
    printf("\nTest of list_add_tail: add four elements at end of list\n");
    print_list(&anker);
    list_add(&fifth.head,&anker);
    printf("Test of list_add: add element with pid 5 to begin of list\n");
    print_list(&anker);
    list_del(&third.head);
    printf("Test of list_del: delete element with pid 3\n");
    print_list(&anker);
    list_move(&secound.head,&anker);
    printf("Test of list_move: delete and add element with pid 2 to start of list\n");
    print_list(&anker);
    printf("Test of list_move_tail: delete and add element with pid 5 to end of list\n");
    list_move_tail(&fifth.head,&anker);
    print_list(&anker);
    printf("Test of list_empty: Return 0 if list isn't empty\n");
    printf( "%i\n",list_empty(&anker));
    printf("Test of list_empty: Return 1 if list is empty\\n");
    list_init(&ankerForEmptyTest);
    printf("%i\n",list_empty(&ankerForEmptyTest));
}
