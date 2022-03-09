/**
 * Author: Valkryst
 *
 * Purpose
 *      A convenience function to allow for easier error logging.
 *
 * Notes
 *      1. The _XOPEN_SOURCE define and string.h include are required for the
 *         strerror_r function. When _XOPEN_SOURCE >= 600, it ensures that
 *         strerror_r returns an errno when it completes.
 *
 *      2. We use strerror_r instead of strerror because the former is thread
 *         safe and we cannot guarantee that these logging functions are used
 *         in a single-threaded environment.
 */
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <string.h>

#include "logger.h"

/**
 * Prints a formatted error message to stderr.
 *
 * @param errno The errno value associated with the error, or 0 if there is none.
 * @param fileName The file name (e.g. example.c) in which the error occurred.
 * @param functionName The function name (e.g. exampleFunc) in which the error occurred.
 * @param message A descriptive error message. If NULL, this is set to "See previous error.".
 */
void printError(int errorNumber, const char fileName[], const char functionName[], const char message[]) {
    if (NULL == fileName) {
        fileName = "undefined";
    }

    if (NULL == functionName) {
        functionName = "undefined";
    }

    if (NULL == message) {
        message = "See previous error.";
    }

    if (0 != errorNumber) {
        char errnoMessage[LOGGER_MAXIMUM_STRERROR_MESSAGE_LENGTH];
        if ((errorNumber = strerror_r(errorNumber, errnoMessage, LOGGER_MAXIMUM_STRERROR_MESSAGE_LENGTH)) != 0) {
            fprintf(
				stderr,
                "%s#%s\n\r\tmessage: %s\n",
                fileName,
                functionName,
                message
			);

            if (strerror_r(errorNumber, errnoMessage, LOGGER_MAXIMUM_STRERROR_MESSAGE_LENGTH) != 0) {
                fprintf(
					stderr,
                    "logger.c#printError\n\r\tmessage: Could not retrieve errno message using strerror_r.\n\r"
				);
            } else {
                fprintf(
					stderr,
                    "logger.c#printError\n\r\terrno message: %s\n\r",
                    errnoMessage
				);
            }

            return;
        }

        fprintf(
			stderr,
            "%s#%s:\n\r\terrno message: %s\n\r\tmessage: %s\n\r",
            fileName,
            functionName,
            errnoMessage,
            message
		);
    } else {
        fprintf(
			stderr,
            "%s#%s:\n\r\tmessage: %s\n\r\r",
            fileName,
            functionName,
            message
		);
    }
}