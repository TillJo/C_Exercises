//
// Created by till on 11.04.20.
//

#ifndef CEXERCISE_SHELL_H
#define CEXERCISE_SHELL_H

void inputLoop();
void printInfo(char **userInputDivorced);
void infoByJobID(int jobId);
void startJob(char **userInputDivorced);
void killProcessByID(int jobId);
void killProcess(char **userInputDivorced);
void list();
void killAllChildren();
void sysCall(char **userInputDivorced);
void handleWaitCmd(char **userInputDivorced);
char * stringRemoveChars(char *string, char *spanset);
char *getUserInput();
#endif
