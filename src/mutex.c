/**
 * Author: Valkryst
 *
 * Purpose
 *      A set of convenience functions to allow for easier use of mutexes.
 *
 * Notes
 *      * All mutexes are created with the PTHREAD_MUTEX_ERRORCHECK attribute
 *        type. See source #1 for more information.
 *
 *      * The _GNU_SOURCE define is required for the pthread_mutexattr_settype
 *        function. See source #2 for more information.
 *
 * Sources
 *      1. https://linux.die.net/man/3/pthread_mutexattr_settype
 *      2. https://stackoverflow.com/a/18376848/13279616
 */
#define _GNU_SOURCE

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#include "logger.h"
#include "mutex.h"

/**
 * Allocates a mutex attribute and sets its type to PTHREAD_MUTEX_ERRORCHECK.
 *
 * @return The mutex attribute or NULL if an error occurred.
 */
pthread_mutexattr_t *createMutexAttribute() {
    pthread_mutexattr_t *attribute = (pthread_mutexattr_t*) malloc(sizeof(pthread_mutexattr_t));
    if (NULL == attribute) {
        printError(
			errno,
			__FILE__,
			__PRETTY_FUNCTION__,
			"Could not allocate memory for attribute."
		);
        return NULL;
    }

    if (0 != (errno = pthread_mutexattr_init(attribute))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not initialize attribute."
	    );
        free(attribute);
        return NULL;
    }

    if (0 != (errno = pthread_mutexattr_settype(attribute, PTHREAD_MUTEX_ERRORCHECK))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not set attribute type."
	    );
        free(attribute);
        return NULL;
    }

    return attribute;
}

/**
 * Destroys and frees a mutex attribute.
 *
 * @param attribute A mutex attribute.
 * @return 0 if successful or 1 if an error occurred.
 */
int destroyMutexAttribute(pthread_mutexattr_t *attribute) {
    if (NULL == attribute) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not destroy attribute as it is null."
	    );
        return 1;
    }

    if (0 != (errno = pthread_mutexattr_destroy(attribute))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not destroy attribute."
	    );
        return 1;
    }

    free(attribute);
    return 0;
}

/**
 * Attempts to lock a mutex.
 *
 * @param mutex A mutex.
 * @return 0 if successful or 1 if an error occurred.
 */
int lockMutex(pthread_mutex_t *mutex) {
    if (NULL == mutex) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not lock mutex as it is null."
	    );
        return 1;
    }

    if (0 != (errno = pthread_mutex_lock(mutex))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not lock mutex."
	    );
        return 1;
    }

    return 0;
}

/**
 * Attempts to unlock a mutex.
 *
 * @param mutex A mutex.
 * @return 0 if successful or 1 if an error occurred.
 */
int unlockMutex(pthread_mutex_t *mutex) {
    if (NULL == mutex) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not unlock mutex as it is null."
	    );
        return 1;
    }

    if ((errno = pthread_mutex_unlock(mutex)) != 0) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not unlock mutex."
	    );
        return 1;
    }

    return 0;
}

/**
 * Allocates and initializes a mutex with the PTHREAD_MUTEX_ERRORCHECK attribute
 * type.
 *
 * @return The allocated mutex or NULL if an error occurred.
 */
pthread_mutex_t* createMutex() {
    pthread_mutexattr_t *attribute = createMutexAttribute();
    if (NULL == attribute) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not create attribute."
	    );
        return NULL;
    }

    pthread_mutex_t *mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (NULL == mutex) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not allocate memory for mutex."
	    );

        if (0 != destroyMutexAttribute(attribute)) {
            printError(0, __FILE__, __PRETTY_FUNCTION__, NULL);
        }
        return NULL;
    }

    if (0 != (errno = pthread_mutex_init(mutex, attribute))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not initialize mutex."
	    );

        if (0 != destroyMutexAttribute(attribute)) {
            printError(0, __FILE__, __PRETTY_FUNCTION__, NULL);
        }

        free(mutex);
        return NULL;
    }

    if (0 != destroyMutexAttribute(attribute)) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    NULL
	    );
        free(mutex);
        return NULL;
    }

    return mutex;
}

/**
 * Destroys and frees a mutex.
 *
 * @param mutex A mutex.
 * @return 0 if successful or 1 if an error occurred.
 */
int destroyMutex(pthread_mutex_t * mutex) {
    if (NULL == mutex) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not destroy mutex as it is null."
	    );
        return 1;
    }

    /**
     * Destroying a locked mutex is undefined behaviour, so we must check if it
     * is locked.
     *
     * Sources
     *      * https://linux.die.net/man/3/pthread_mutex_destroy
     *      * https://linux.die.net/man/3/pthread_mutex_trylock
     */
    if (0 != (errno = pthread_mutex_trylock(mutex))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not destroy mutex as it is locked."
	    );
        return 1;
    }

    if (0 != unlockMutex(mutex)) {
	    printError(
		    0,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    NULL
	    );
        return 1;
    }

    if (0 != (errno = pthread_mutex_destroy(mutex))) {
	    printError(
		    errno,
		    __FILE__,
		    __PRETTY_FUNCTION__,
		    "Could not destroy mutex."
	    );
        return 1;
    }

    free(mutex);
    return 0;
}