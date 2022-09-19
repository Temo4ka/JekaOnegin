#include "functionList.h"
#include <sys/types.h>
#include <sys/stat.h>

const char *OUTPUTFILENAME = "SortedStrings.txt";
const char *INPUTFILENAME  =          "poem.txt";

Error error = {};

#define logPrint() { \
    FILE *logFile = fopen("errors.txt", "w"); \
    fprintf(logFile, "%s.\nFile: %s,\nLine: %d,\nFunction: %s\n", error.errorMessage, error.fileName, error.line - 1, error.functionName); \
    fclose(logFile);     \
}

void makeError(char const *fileName, char const *functionName, unsigned lineNumber);


int TEXTConsturctor(TEXT *txt, const char *fileName) {
    if (txt == nullptr || fileName == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileIN;
    }

    txt -> file     = fopen(fileName, "r");
    txt -> fileName = fileName;

    if (txt -> file == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileIN;
    }

    txt -> fileSize = getFileSize(txt -> fileName) ;
    if (txt -> fileSize == FileIN)
        return FileIN;

    txt -> buffer = (char *) calloc(txt -> fileSize + 1, sizeof(char));
    size_t gotSymbols = fread(txt -> buffer, sizeof(char), txt -> fileSize, txt -> file);
    txt -> buffer[gotSymbols] = '\0';

    if (!feof(txt -> file)) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileIN;
    }

    return OK;
}

int getArrayOfStrings(Lines *arrayOfStrings, TEXT *poemText) {
    if (poemText == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileIN;
    }
    if (poemText->buffer == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return BUFFER;
    }

    size_t numberOfStrings = 0;

    for (size_t index = 0; poemText -> buffer[index] != '\0'; ++index)
        if (poemText -> buffer[index] == '\n')
            ++numberOfStrings;
    linesConstructor(arrayOfStrings, numberOfStrings);

    if (arrayOfStrings -> array == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return DINAMICMEMORY;
    }

    size_t stringIndex = 0;
    char *firstBufferPointer  = poemText -> buffer;
    char *secondBufferPointer = poemText -> buffer;
    while (*secondBufferPointer != '\0') {
        if (*secondBufferPointer == '\n') {
            lineConstructor(&(arrayOfStrings -> array[stringIndex]), firstBufferPointer, secondBufferPointer - firstBufferPointer);

            if (arrayOfStrings -> array[stringIndex].line == nullptr) {
                makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
                return DINAMICMEMORY;
            }

            *secondBufferPointer = '\0';
            ++secondBufferPointer;
            ++stringIndex;
            firstBufferPointer = secondBufferPointer;
        }
        ++secondBufferPointer;
    }

    return OK;
}

void textDestructor(TEXT *txt) {
    assert(txt != nullptr && "Error in textDestructor!!!\n");

    fclose(txt ->   file);
    free  (txt -> buffer);
}

long getFileSize(const char *fileName) {
    assert(fileName != nullptr && "getFileSize Error!!\n");

    /*fseek(fileName , 0 , SEEK_END);
    long totalLength = ftell(fileName);
    rewind(fileName);*/

    struct stat buf = {};
    if (stat(fileName, &buf)) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileIN;
    }

    return buf.st_size;
}

int print(Lines arrayOfStrings, FILE *outputFile) {
    if (outputFile == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileOUT;
    }

    for (size_t currentString = 0; currentString < arrayOfStrings.numberOfLines; ++currentString) {
        fputs(arrayOfStrings.array[currentString].line, outputFile);
        fprintf(outputFile, "\n");
    }
    fputs("---------------------------------------------------\n", outputFile);

    return OK;
}

int printOrigin(char *buffer, size_t numberOfStrings, FILE *outputFile) {
    if (outputFile == nullptr) {
        makeError(__FILE__, __PRETTY_FUNCTION__, __LINE__);
        return FileOUT;
    }

    size_t currentString = 0;
    while (currentString < numberOfStrings) {
        if (*buffer == '\0') {
            fputc('\n', outputFile);
            currentString++;
        } else
            fputc(*buffer, outputFile);
        buffer++;
    }

    return OK;
}

int checkForErrors(int er) {
    switch (er) {
        case FileIN:
            error.errorMessage = (char*) "An Error in reading file occured";
            logPrint()
            return 1;

        case FileOUT:
            error.errorMessage = (char*) "An Error in writing file occured";
            logPrint()
            return 1;

        case BUFFER:
            error.errorMessage = (char*) "An Error with buffer occured";
            logPrint()
            return 1;

        case DINAMICMEMORY:
            error.errorMessage = (char*) "An Error with calloc occured";
            logPrint()
            return 1;

        default:
            return 0;
    }
}

void getFileNames(int args, char **argv, char **inFileName, char **outFileName) {
    switch (args) {
        case 3:
            *inFileName  = argv[1];
            *outFileName = argv[2];
            break;
        case 2:
            *inFileName  = argv[1];
            *outFileName = (char *) OUTPUTFILENAME;
            break;
        default:
            *inFileName  = (char *)  INPUTFILENAME;
            *outFileName = (char *) OUTPUTFILENAME;
            break;
    }
}

void makeError(char const *fileName, char const *functionName, unsigned lineNumber) {
    error.fileName     =     fileName;
    error.functionName = functionName;
    error.line         =   lineNumber;
}