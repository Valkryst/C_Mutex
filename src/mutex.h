/**
 * Author: Valkryst
 *
 * Purpose
 *      A set of convenience functions to allow for easier use of mutexes.
 */
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

int lockMutex(pthread_mutex_t *mutex);
int unlockMutex(pthread_mutex_t *mutex);

pthread_mutex_t* createMutex();
int destroyMutex(pthread_mutex_t * mutex);

#endif
