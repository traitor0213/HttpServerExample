#ifndef _SERVER_H
#define _SERVER_H

#define BUFFER_SIZE 256

typedef struct PROCEDURE_INFO
{
    SOCKET ResponseSocket;
    BOOL *MoveGarantie; 
} PROCEDURE_INFO, *LPPROCEDURE_INFO;

//for multi threading
HANDLE hMutex;

//for GUI
char **UserName;
char **ActivateApplication;
char **Time;
int InformationIndex;

//for CNC
char *Command;
int CommandIndex;
int CommandRange;
int SessionNumber;
int MAXIMUM_INDEX;

void ExitFromMemoryError()
{
    printf("not enough memory!");
    getchar();
    exit(0);
}

#endif