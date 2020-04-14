#include <stdio.h>
#include "doubleLinkedList.h"


extern void initList(struct listHead *head) {
    head->next = head;
    head->prev = head;
}

extern void addToBegin(struct listHead *new, struct listHead *head) {
    new->prev = head;
    new->next = head->next;
    head->next->prev = new;
    head->next = new;
}

extern void addToTail(struct listHead *new, struct listHead *head) {
    new->prev = head->prev;
    head->prev->next = new;
    head->prev = new;
    new->next = head;
}

extern struct listHead *delFromList(struct listHead *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev = 0;
    entry->next = 0;
    free(entry);
    return entry;
}

extern void moveToBegin(struct listHead *entry, struct listHead *head) {
    struct listHead *new = delFromList(entry);
    addToBegin(new, head);
}

extern void moveToTail(struct listHead *entry, struct listHead *head) {
    struct listHead *new = delFromList(entry);
    addToTail(new, head);
}

extern int isEmpty(struct listHead *head) {
    if ((head->prev == head) && (head->next == head)) {
        return 1;
    } else {
        return 0;
    }
}


extern void printList(struct listHead *head) {

    struct listHead *iterator = head->next;
    int counter = 0;

    while (iterator != head) {
        printf("%dth-element with pid: %i ", counter, ((struct procInfo *) iterator)->pid);
        iterator = iterator->next;
        counter++;
    }

    printf("\n");
}

extern struct procInfo *getJobById(int numberOfJob, struct listHead *head) {
    int idx = -1;
    struct listHead *tempHead = head->next;
    while (idx < numberOfJob) {
        tempHead = tempHead->next;
        idx++;
    }
    return (struct procInfo *) tempHead;
}


extern void mapToList(void (*f)(), struct listHead *head) {
    struct listHead *iterator = head->next;
    int counter = 0;

    while (iterator != head) {
        if (iterator != head) {
            struct procInfo* temp = (struct procInfo *) iterator;
            f(temp);
            counter++;
        }
        iterator = iterator->next;
    }
}


/**
 * Only exits for test cases.
 */
int mains(int argc, const char **argv) {

    struct procInfo frist;
    struct procInfo second;
    struct procInfo third;
    struct procInfo fourth;
    struct procInfo fifth;

    struct listHead head;

    frist.pid = 1;
    second.pid = 2;
    third.pid = 3;
    fourth.pid = 4;
    fifth.pid = 5;


    initList(&head);
    addToTail(&frist.head, &head);
    addToTail(&second.head, &head);
    addToTail(&third.head, &head);
    addToTail(&fourth.head, &head);
    printf("\nTest of addToTail: add four elements at end of list\n");
    printList(&head);
    addToBegin(&fifth.head, &head);
    printf("Test of addToBegin: add element with pid 5 to begin of list\n");
    printList(&head);
    delFromList(&third.head);
    printf("Test of delFromList: delete element with pid 3\n");
    printList(&head);
    moveToBegin(&second.head, &head);
    printf("Test of moveToBegin: delete and add element with pid 2 to start of list\n");
    printList(&head);
    printf("Test of moveToTail: delete and add element with pid 5 to end of list\n");
    moveToTail(&fifth.head, &head);
    printList(&head);
    printf("Test of isEmpty: Return 0 if list isn't empty\n");
    printf("%i\n", isEmpty(&head));
    printf("Test of isEmpty: Return 1 if list is empty\\n");
    struct listHead headForEmptyList;
    initList(&headForEmptyList);
    printf("%i\n", isEmpty(&headForEmptyList));
}