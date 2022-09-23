#include <clocale>
#include "functionList.h"

int main(int argc, char* argv[]) {
    char *inFileName  = nullptr;
    char *outFileName = nullptr;

//    setlocale(LC_ALL, "Russian");
    getFileNames(argc, argv, &inFileName, &outFileName);

    Text poemText = {};
    if (TEXTConstructor(&poemText, inFileName))
        return 1;

    FILE *outputFile = fopen(outFileName, "w");

    Lines arrayOfStrings = {};
    if (getArrayOfStrings(&arrayOfStrings, &poemText))
        return 1;

    quickSort(arrayOfStrings.array, sizeof(Line),  0, arrayOfStrings.numberOfLines - 1, comparatorForQsort);
    if (print(arrayOfStrings, outputFile))
        return 1;


    qsort(arrayOfStrings.array, arrayOfStrings.numberOfLines, sizeof(Line), revComparator);
    if (print(arrayOfStrings, outputFile))
        return 1;

    qsort(arrayOfStrings.array, arrayOfStrings.numberOfLines, sizeof(Line), originComparator);
    if (print(arrayOfStrings, outputFile))
        return 1;

    linesDestructor(&arrayOfStrings);
    textDestructor (&poemText      );
    fclose(outputFile);
    logClose();
    return 0;
}
