#ifndef DLL_H
#define DLL_H

#include <stdbool.h>

struct listHead {
    struct listHead *next, *prev;
};

struct procInfo {
    struct listHead head;
    int pid;
    int jobID;
    int priority;
    bool isExitStatusSet;
    int exitStatus;
    char *name;
};

extern void initList(struct listHead *head);

extern void addToBegin(struct listHead *new, struct listHead *head);

extern void addToTail(struct listHead *new, struct listHead *head);

extern struct listHead* delFromList(struct listHead *entry);

extern void moveToBegin(struct listHead *entry, struct listHead *head);

extern void moveToTail(struct listHead *entry, struct listHead *head);

extern int isEmpty(struct listHead *head);

extern void printList(struct listHead *head);

extern void mapToList(void (*f)() , struct listHead *head);

extern struct procInfo *getJobById(int numberOfJob, struct listHead *head);

#endif
