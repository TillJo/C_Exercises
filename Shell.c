#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <zconf.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "makeargv.c"
#include "doubleLinkedList.h"
#include "doubleLinkedList.c"
#include "shell.h"

#define JOB_CMD "job"
#define LIST_CMD "list"
#define INFO_CMD "printInfo"
#define WAIT_CMD "wait"
#define KILL_CMD "kill"
#define QUIT_CMD "quit"

#define INVALID_JOB_MESSAGE "Job Number invalid\n"
#define INVALID_ID "Job doesn't exits \n"
#define RUNNING_JOB_MESSAGE "running status =  0) : "
#define FINISH_JOB_MESSAGE "finished status =  %i) : "
#define FAILED_JOB_MESSAGE "Job %s failed"

#define DELIMITERS " "
#define LINE_SEPERATOR "\n"
#define MAX_INPUT_SIZE 20

int countOfJobs = 0;
struct listHead *beginOfProcessList;

static void initDll();

static void clearUserInput(char **userInputDivorced);

static void printIdAndPid(const struct procInfo *jobToPrint);

static void printJobById(int jobId);

static void addJob(char *const *userInputDivorced, pid_t childPid);

static void printFinishedJob(struct procInfo *job, int status);

static bool handleUserInput(char *userInput);

static bool isInvalidJobId(int jobID);

static bool isInfoCmd(char *const *userInputDivorced, int length);

static bool isJobCmd(char *const *userInputDivorced, int length);

static bool isKillCmd(char *const *userInputDivorced, int length);

static bool isWaitCmd(char *const *userInputDivorced, int length);

static bool isQuitCmd(char *const *userInputDivorced, int length);

static void killChild(const struct procInfo *toKill);

static void printJob(struct procInfo *job);

static void waitForJob(const struct procInfo *tempHead);

void printStatus(const struct procInfo *job);

int main(int argc, const char **argv) {
    initDll();
    inputLoop();
    return EXIT_SUCCESS;
}

/**
 * Takes cmd input from user.
 */
void inputLoop() {
    char *input;
    do {
        printf("tsh>");
        input = getUserInput();
        handleUserInput(input);
        free(input);
    } while (input);
}

/**
 * Evaluate user input and what user want to do.
 * @param userInput input from user
 * @return false if user enter 'quit' else true
 */
bool handleUserInput(char *userInput) {
    char **userInputDivorced = malloc(MAX_INPUT_SIZE * sizeof(char *));
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        userInputDivorced[i] = malloc(MAX_INPUT_SIZE * sizeof(char));
    }
    int length = makeargv(userInput, DELIMITERS, &userInputDivorced);
    userInputDivorced[length - 1] = stringRemoveChars(userInputDivorced[length - 1], LINE_SEPERATOR);
    if (strcmp(userInputDivorced[0], LIST_CMD) == 0) {
        list();
    } else if (isInfoCmd(userInputDivorced, length)) {
        printInfo(userInputDivorced);
    } else if (isJobCmd(userInputDivorced, length)) {
        startJob(userInputDivorced);
    } else if (isKillCmd(userInputDivorced, length)) {
        killProcess(userInputDivorced);
    } else if (isWaitCmd(userInputDivorced, length)) {
        handleWaitCmd(userInputDivorced);
    } else if (isQuitCmd(userInputDivorced, length)) {
        killAllChildren();
        clearUserInput(userInputDivorced);
        return false;
    } else {
        sysCall(userInputDivorced);
    }
    clearUserInput(userInputDivorced);
    return true;
}

/**
 * Print info of job.
 * @param userInputDivorced job to print
 */
void printInfo(char **userInputDivorced) {
    int jobId = atoi(userInputDivorced[1]);
    infoByJobID(jobId);
}

/**
 * Check if id is valid and
 * @param jobId id of job
 */
void infoByJobID(int jobId) {
    if (isInvalidJobId(jobId)) {
        printf(INVALID_JOB_MESSAGE);
    } else {
        printJobById(jobId);
    }
}


void printJobById(int jobId) {
    struct procInfo *job = getJobById(jobId, beginOfProcessList);
    printJob(job);
}

void printJob(struct procInfo *job) {
    printIdAndPid(job);
    printStatus(job);
    printf("%s\n", job->name);
}

/**
 * If job is running print running otherwise print exit status.
 */
void printStatus(const struct procInfo *job) {
    int status;
    pid_t returnedPid = waitpid(job->pid, &status, WNOHANG);

    if (WIFEXITED(status) == 1 || returnedPid == -1) {
        printFinishedJob(job, status);
    } else {
        printf(RUNNING_JOB_MESSAGE);
    }
}

/**
 * Check if exit status is already set and print status. If not set will first using status and WEXITSTAUS to eval.
 * @param job job to print
 * @param status status of child for evaluate exit status
 */
void printFinishedJob(struct procInfo *job, int status) {
    if (job->isExitStatusSet) {
        printf(FINISH_JOB_MESSAGE, job->exitStatus);
    } else {
        int exitStatus = WEXITSTATUS(status);
        job->exitStatus = exitStatus;
        job->isExitStatusSet = true;
        printf(FINISH_JOB_MESSAGE, job->exitStatus);
    }
}

/**
 * Fork and make syscall in child.
 * @param userInputDivorced input from user separated by whitespace
 */
void startJob(char **userInputDivorced) {
    pid_t cPid = fork();
    if (cPid == 0) {
        if (execvp(userInputDivorced[1], &userInputDivorced[1]) < 0) {
            printf(FAILED_JOB_MESSAGE, userInputDivorced[1]);
            printf("[status = 1]\n");
            exit(1);
        }
    } else {
        addJob(userInputDivorced, cPid);
    }
}

/**
 * Add procInfo with information about job to double linked list.
 */
void addJob(char *const *userInputDivorced, pid_t childPid) {
    countOfJobs++;
    struct procInfo *newJob = malloc(sizeof(struct procInfo));
    newJob->jobID = countOfJobs;
    newJob->pid = childPid;
    newJob->name = malloc(sizeof(char) * MAX_INPUT_SIZE);
    newJob->isExitStatusSet = false;
    strcpy(newJob->name, userInputDivorced[1]);
    addToTail(&newJob->head, beginOfProcessList);
}

/**
 * Handle cmd from user to kill job by using id. Method first checks if id is valid.
 * @param userInputDivorced input from user separated by whitespace
 */
void killProcess(char **userInputDivorced) {
    int jobId = atoi(userInputDivorced[1]);
    if (isInvalidJobId(jobId)) {
        printf(INVALID_ID);
    } else {
        killProcessByID(jobId);
    }
}

/**
 * Kill child by id.
 * @param jobId id of job to kill
 */
void killProcessByID(int jobId) {
    struct procInfo *toKill = getJobById(jobId, beginOfProcessList);
    killChild(toKill);
}

/**
 * Check if child is killed otherwise kill child.
 * @param toKill
 */
void killChild(const struct procInfo *toKill) {
    if (!toKill->isExitStatusSet) {
        pid_t toDelete = toKill->pid;
        kill(toDelete, SIGKILL);
    }
}

/**
 * Using map to list to apply kill on all child respectively elements of double linked list.
 */
void killAllChildren() {
    mapToList(killChild, beginOfProcessList);
}

/**
 * Using map to list to apply printJob on all child respectively elements of double linked list. So print list of
 * all elements.
 */
void list() {
    mapToList(printJob, beginOfProcessList);
}

/**
 * Make syscall.
 */
void sysCall(char **userInputDivorced) {
    pid_t cPid = fork();
    if (cPid == 0) {
        if (execvp(userInputDivorced[0], &userInputDivorced[0]) < 0) {
            printf("\n[invalid command]\n");
            printf("[status = 1]\n");
            exit(1);
        }
    } else if (cPid < 0) {
        printf("Warning: Fork Failed\n");
    }
}

/**
 * Receives valid job id and wait for child to end.
 * @param jobId id of job used to wait for child.
 */
void waitForProcessByID(int jobId) {
    struct procInfo *tempHead = getJobById(jobId, beginOfProcessList);
    waitForJob(tempHead);
}

/**
 * Handle cmd from user to wait for job by using id. Method first checks if id is valid.
 * @param userInputDivorced input from user separated by whitespace
 */
void handleWaitCmd(char **userInputDivorced) {
    int jobId = atoi(userInputDivorced[1]);
    if (isInvalidJobId(jobId)) {
        printf(INVALID_ID);
    } else {
        waitForProcessByID(jobId);
    }
}

/**
 * Wait for job to finish.
 */
void waitForJob(const struct procInfo *tempHead) {
    int status;
    waitpid(tempHead->pid, &status, 0);
}

void initDll() {
    beginOfProcessList = malloc(sizeof(struct listHead));
    initList(beginOfProcessList);
}

void printIdAndPid(const struct procInfo *jobToPrint) {
    printf("%i (pid  ", jobToPrint->jobID);
    printf("%i ", jobToPrint->pid);
}

/**
 * Source: http://www.c-howto.de/tutorial/strings-zeichenketten/uebungen/
 * loesung-teil-3-string-remove-chars/
 */
char *stringRemoveChars(char *string, char *spanset) {
    char *ptr = string;
    ptr = strpbrk(ptr, spanset);

    while (ptr != NULL) {
        *ptr = 0;
        ptr = strpbrk(ptr, spanset);
    }

    return string;
}

char *getUserInput() {
    char buffer[MAX_INPUT_SIZE + 5];
    fgets(buffer, MAX_INPUT_SIZE, stdin);
    char *input = malloc(sizeof(char) * strlen(buffer) + 5);
    strcpy(input, buffer);
    return input;
}

void clearUserInput(char **userInputDivorced) {
    freemakeargv(userInputDivorced);
}

bool isInvalidJobId(int jobID) {
    return jobID > countOfJobs || jobID == 0;
}

bool isQuitCmd(char *const *userInputDivorced, int length) {
    return length > 0 && strcmp(userInputDivorced[0], QUIT_CMD) == 0;
}

bool isWaitCmd(char *const *userInputDivorced, int length) {
    return length > 1 && strcmp(userInputDivorced[0], WAIT_CMD) == 0 && isdigit(*userInputDivorced[1]);
}

bool isKillCmd(char *const *userInputDivorced, int length) {
    return length > 1 && strcmp(userInputDivorced[0], KILL_CMD) == 0 && isdigit(*userInputDivorced[1]);
}

bool isJobCmd(char *const *userInputDivorced, int length) {
    return length > 1 && strcmp(userInputDivorced[0], JOB_CMD) == 0;
}

bool isInfoCmd(char *const *userInputDivorced, int length) {
    return length > 1 && strcmp(userInputDivorced[0], INFO_CMD) == 0 && isdigit(*userInputDivorced[1]);
}