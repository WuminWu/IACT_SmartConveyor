#ifndef MC_MQUEUE_H
#define MC_MQUEUE_H
#include <mqueue.h>
#include "common.h"

#define MAX_SIZE    1024

int CreateMqueue( mqd_t *hClientArray, mqd_t *hServerArray, char *MQUEUE_NAME[], int mqueueNum);
int DestroyMqueue( mqd_t *hClientArray,  mqd_t *hServerArray, int mqueueNum);

#endif
