/*
   elfo.c
   elfo

   Program to change the EOL Format from unix, mac, dos to dos, unix, mac.

   Created by Julio Carlos Sanchez Ortega (thegeek@thegeekinside.net) on 2007-02-01.
   Copyright 2007, 2008 Tearlach Software. All rights reserved.
 */
// TODO: Se le podra enviar un directorio.
// TODO: Refactory (Que significa esto?)
// TODO: Generar unidades de prueba
// TODO: Dividir la funcionalidad de Format
// TODO: Aplicar estandares de GNU?
// TODO: Usar gettext

#include "elfo.h"
#include "messages.h"

// Save de file info
struct stat s_buf;

/**
 * Main entry point.
 */
int main(int argc, char *argv[])
{
    /* options descriptor */
    static struct option longopts[] =
    {
        { "to", required_argument, NULL, 't' },
        { "file", required_argument, NULL, 'f' },
        { "out-file", optional_argument, NULL, 'o' },
        { "version", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { NULL, 0, NULL, 0 }
    };

    int ch;

    if (argc == 1 || strncmp(argv[1], "-", 1))
    {
        Usage();
    }

    // Variable to storage the options
    struct options options;

    Initialize(&options);

    // Pase the command line
    while ((ch = getopt_long(argc, argv, "t:f:o:vh", longopts, NULL)) != -1)
    {
        switch (ch)
        {
            case 't':
                strcpy(options.format, optarg);
                DPRINTF(("Format: %s\n", options.format))
                break;

            case 'f':
                strcpy(options.sourceFileName, optarg);
                DPRINTF(("Source: %s\n", options.sourceFileName))
                break;

            case 'o':
                strcpy(options.targetFileName, optarg);
                DPRINTF(("Target: %s\n", options.targetFileName))
                break;

            case 'v':
                PrintVersion();
                exit(0);
                break;

            case 'h':
            case '?':
                Help();
                break;

            default:
                Usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (strcmp(options.sourceFileName, "") != 0)
    {
        if (!Format(&options))
        {
            printf("Coudn't format the file %s", options.sourceFileName);
            exit(1);
        }
    }

    return 0;
}

/**
 * Initialize the options Structure
 */
void Initialize(struct options *options)
{
    // Set defaults options
    strcpy(options->format, "dos");
    strcpy(options->sourceFileName, "");
    strcpy(options->targetFileName, "");
}

/**
 * Format
 *
 * Format with the correct EOL.
 */
int Format(struct options *options)
{
    char *sourceFileName;
    char *targetFileName;
    FILE *in, *out;
    int ch,
        rval = FALSE;

    sourceFileName = malloc(FILENAME_MAX * sizeof(char));
    targetFileName = malloc(FILENAME_MAX * sizeof(char));

    if (strcmp(options->format, DOS_FORMAT) != 0 &&
        strcmp(options->format, UNIX_FORMAT) != 0)
    {
        PRINTF(("Invalid file format: %s\n", options->format))
        return FALSE;
    }

    DPRINTF(("Source file: *%s*\n", options->sourceFileName))
    DPRINTF(("Target file: *%s*\n", options->targetFileName))

    if (strcmp(options->targetFileName, "") == 0)
    {
        sourceFileName = BackupFile(options->sourceFileName, NULL);
        if ((in = fopen(options->sourceFileName, W_CNTRL)) == (FILE *)0)
        {
            PRINTF(("Couldn't create the source file %s\n", sourceFileName))
            return FALSE;
        }
        fclose(in);
        strcpy(targetFileName, options->sourceFileName);
    }    else
    {
        strcpy(sourceFileName, options->sourceFileName);
        strcpy(targetFileName, options->targetFileName);
    }

    fprintf(stdout, "Formating %s...\n", sourceFileName);

    if ((in = fopen(sourceFileName, R_CNTRL)) == (FILE *)0)
    {
        PRINTF(("Couldn't open the source file %s\n", sourceFileName))
        return FALSE;
    }

    if ((out = fopen(targetFileName, W_CNTRL)) == (FILE *)0)
    {
        PRINTF(("Couldn't open the target file %s\n", targetFileName))
        fclose(in);
        return FALSE;
    }

    if (strcmp(options->format, DOS_FORMAT) == 0)
    {
        // Convert to DOS file format
        while ((ch = getc(in)) != EOF)
        {
            DPRINTF(("d char: %i\n", ch))
            if (((ch == LINE_FEED) && (putc(CARRIAGE_RETURN, out) == EOF)) ||
                (putc(ch, out) == EOF))
            {
                break;
            }
        }
    }
    else if (strcmp(options->format, UNIX_FORMAT) == 0)
    {
        // Convert to Unix file format
        while ((ch = getc(in)) != EOF)
        {
            DPRINTF(("u char: %i\n", ch))
            if ((ch != CARRIAGE_RETURN && ch != SUBSTITUTE) &&
                (putc(ch, out) == EOF))
            {
                break;
            }
        }
    }

    if (fclose(in) == EOF)
    {
        rval = FALSE;
    }
    if (fclose(out) == EOF)
    {
        rval = FALSE;
    }

    // Free resources
    free(sourceFileName);
    free(targetFileName);

    return TRUE;
}

/**
 * Print the usage screen
 */
void Usage()
{
    printf("Usage:\n");
    printf("elfo [-t format] [-o OutputFile] -f FileToFormat\n");
    printf("\t-t\t--to=[dos|unix]\tFile format in which the file will be converted\n");
    printf("\t-f\t--file=FILE\tFile to format\n");
    printf("\t-o\t--out-file=FILE\tPlace the output into FILE\n");
    printf("\t-v\t--version\tPrint version\n");
    printf("\t-h\t--help\t\tShow this message\n");
}

void PrintVersion()
{
    printf("elfo %i.%i.%i%s by Julio Carlos Sanchez Ortega\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION, PRERELEASE);
    printf("(c) Copyright 2007 Tearlach Software. All Rights Reserved.\n\n");
}

/**
 * Print the help screen
 */
void Help()
{
    PrintVersion();
    Usage();
}

/**
 * Backup de file with the ext parameter extension and return the
 * name of file.
 */
char *BackupFile(char *sourceFileName, char *ext)
{
    char *backupFileName;

    backupFileName = malloc(FILENAME_MAX * sizeof(char));

    if (ext == NULL)
    {
        ext = malloc(4 * sizeof(char));
        strcpy(ext, BACKUP_EXTENSION);
    }

    // Rename the original file to have a .ori extension
    strcpy(backupFileName, sourceFileName);
    strcat(backupFileName, ".");
    strcat(backupFileName, ext);

    // Free resources
    free(ext);

    rename(sourceFileName, backupFileName);

    DPRINTF(("Backup file name: %s\n", backupFileName))

    return backupFileName;
}
