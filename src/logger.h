/**
 * Author: Valkryst
 *
 * Purpose
 *      A convenience function to allow for easier error logging.
 */
#ifndef LOGGER_H
#define LOGGER_H

#define LOGGER_MAXIMUM_MESSAGE_LENGTH 512
#define LOGGER_MAXIMUM_STRERROR_MESSAGE_LENGTH 128

void printError(int errorNumber, const char fileName[], const char functionName[], const char message[]);

#endif
