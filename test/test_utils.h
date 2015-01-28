#pragma once

/* #define UNIQUE_FILE(prefix, suffix) \ */
/*     (prefix) ## __COUNTER__ ## "." ## (suffix) */
/* #define TEST_FILE UNIQUE_FILE("test", "data") */

#define MAX_TEST_FILE_NAME_LENGTH 350

#define FILE_NAME_HANDLE __file_name__

#define WITH_TEST_FILE_NAME(stmt)				\
    do {							\
	char FILE_NAME_HANDLE[MAX_TEST_FILE_NAME_LENGTH];	\
	getTestFileName(FILE_NAME_HANDLE);			\
	(stmt);							\
    } while (0)

char *getTestFileName(char *result);
char *getUniqueFileName(const char *prefix, const char *suffix, char *result);
