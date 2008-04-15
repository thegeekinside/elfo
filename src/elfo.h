/*
  elfo.h
  elfo

  Definitions to the elfo program.

  Created by Julio Carlos Sanchez Ortega (thegeek@thegeekinside.net) on 2007-02-01.
  Copyright 2007, 2008
*/

#ifndef _ELFO_H_
#define _ELFO_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef FALSE
#define FALSE (0)
#define TRUE  (!FALSE)
#endif

#if MSDOS
#define R_CNTRL   "rb"
#define W_CNTRL   "wb"
#else
#define R_CNTRL   "r+b"
#define W_CNTRL   "w+b"
#endif

#define UNIX_FORMAT     "unix"
#define DOS_FORMAT      "dos"

#define DEBUG    TRUE

/* ==================== */
/* = Debugging macros = */
/* ==================== */
#define ERR_MSG( fn ) { (void)fflush(stderr); \
                        (void)fprintf(stderr, __FILE__ ":%d:" #fn ": %s\n", \
			              __LINE__, strerror(errno)); }
#define METAPRINTF( fn, args, exp ) if( fn args exp ) ERR_MSG( fn )
#define PRINTF(args)    METAPRINTF(printf, args, < 0)
#define FPRINTF(args)   METAPRINTF(fprintf, args, < 0)
#define SCANF(args)     METAPRINTF(scanf, args, < 0)
#define FSCANF(args)    METAPRINTF(fscanf, args, < 0)
#define FFLUSH(args)    METAPRINTF(fflush, args, < 0)
#ifdef DEBUG
#define DPRINTF(s) PRINTF(s)
#else
#define DPRINTF(s)
#endif

/* ======================= */
/* = Version definitions = */
/* ======================= */
#define MAJOR_VERSION    0
#define MINOR_VERSION    6
#define PATCH_VERSION    0
#define PRERELEASE       ""

/* ==================== */
/* = Format constants = */
/* ==================== */
#define LINE_FEED          10
#define CARRIAGE_RETURN    13
#define SUBSTITUTE         26

/* ========================== */
/* = Structures definitions = */
/* ========================== */
struct options
{
    char format[5];
    char sourceFileName [FILENAME_MAX];
    char targetFileName [FILENAME_MAX];
};

/* ======================== */
/* = Defaults definitions = */
/* ======================== */
#define BACKUP_EXTENSION    "ori"

/* ======================== */
/* = Functions prototypes = */
/* ======================== */
int Format (struct options *options);
char *BackupFile(char *fileName, char *ext);
void Usage();
void Help();
void PrintVersion();
void Initialize(struct options *options);

#endif /* _ELFO_H_ */
