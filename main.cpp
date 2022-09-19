#include <clocale>
#include "functionList.h"

int main(int argc, char* argv[]) {
    char *inFileName  = nullptr;
    char *outFileName = nullptr;

    setlocale(LC_ALL, "Russian");
    getFileNames(argc, argv, &inFileName, &outFileName);

    TEXT poemText = {};
    if (checkForErrors(TEXTConsturctor(&poemText, inFileName)))
        return 0;

    FILE *outputFile = fopen(outFileName, "w");

    Lines arrayOfStrings = {};
    if (checkForErrors(getArrayOfStrings(&arrayOfStrings, &poemText)))
        return 0;

//    mergeSort(&arrayOfStrings, 0, arrayOfStrings.numberOfLines, comparatorForMergeSort);
    quickSort(arrayOfStrings.array, sizeof(Line),  0, arrayOfStrings.numberOfLines - 1, comparatorForQsort);
    if (checkForErrors(print(arrayOfStrings, outputFile)))
        return 0;


    qsort(arrayOfStrings.array, arrayOfStrings.numberOfLines, sizeof(Line), revComparator);
    if (checkForErrors(print(arrayOfStrings, outputFile)))
        return 0;

    if (checkForErrors(printOrigin(poemText.buffer, arrayOfStrings.numberOfLines, outputFile)))
        return 0;

    linesDestructor(&arrayOfStrings);
    textDestructor (&poemText      );
    fclose(outputFile);
    return 0;
}
