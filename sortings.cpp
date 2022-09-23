#include "functionList.h"
#include <ctype.h>

//const char *WHITELIST = "ABCDEFGHIJKLMNOPQRSUVWXYZabcdefghijklnopqrstuvwxyz";

static size_t partition(void *arrayOfStrings, size_t sizeOfElement, size_t leftParam, size_t rightParam, int (*cmp)(const void *aParam, const void *bParam));

void quickSort(void *arrayOfStrings, size_t sizeOfElement, size_t left, size_t right, int (*cmp)(const void *aParam, const void *bParam)) {
    if (right <= left )
        return;

    assert(arrayOfStrings != nullptr && "An Error in quickSort!");
    assert(cmp            != nullptr && "An Error in quickSort!");

    size_t mid = partition(arrayOfStrings, sizeOfElement, left, right, cmp);

    quickSort(arrayOfStrings, sizeOfElement,    left,    mid, cmp);
    quickSort(arrayOfStrings, sizeOfElement, mid + 1,  right, cmp);
}

size_t partition(void *arrayOfStrings, size_t sizeOfElement, size_t leftParam, size_t rightParam, int (*cmp)(const void *aParam, const void *bParam)) {
    assert(arrayOfStrings != nullptr && "An Error in Partition!\n");
    assert(cmp != nullptr && "An Error in Partition!\n");

    size_t mid = rand() % (rightParam - leftParam) + leftParam;
    void *controlElement = ((char*) arrayOfStrings) + mid * sizeOfElement;
    void *left = ((char*) arrayOfStrings) + leftParam * sizeOfElement;
    void *right = ((char*) arrayOfStrings) + rightParam * sizeOfElement;


    while (leftParam <= rightParam) {
        while (cmp(left, controlElement) == Less) {
            left = (char *) left + sizeOfElement;
            leftParam++;
        }
        while (cmp(controlElement, right) == Less) {
            right = (char*) right - sizeOfElement;
            rightParam--;
        }

        if (leftParam >= rightParam)
            break;

        swap(left, right, sizeOfElement);

        right = (char*) right - sizeOfElement;
        left  = (char*)  left + sizeOfElement;
        rightParam--;
         leftParam++;
    }

    return rightParam;
}

void mergeSort(Lines *arrayOfStrings, size_t left, size_t right, int (*cmp)(const void *aParam, const void *bParam)) {
    if (right - left == 1)
        return;

    size_t mid = (right + left) / 2;
    mergeSort(arrayOfStrings, left, mid, cmp);
    mergeSort(arrayOfStrings, mid, right, cmp);

    size_t firstIndex    = left;
    size_t secondIndex   = mid;
    size_t passedSymbols = 0;
    Lines resultOfSorting = {};
    linesConstructor(&resultOfSorting, right - left);

    while (passedSymbols < right - left) {
        if (firstIndex == mid) {
            resultOfSorting.array[passedSymbols] = arrayOfStrings->array[secondIndex];
            ++secondIndex;
        } else if (secondIndex == right) {
            resultOfSorting.array[passedSymbols] = arrayOfStrings->array[firstIndex];
            ++firstIndex;
        } else if (cmp(&arrayOfStrings->array[firstIndex], &arrayOfStrings->array[secondIndex]) == Less) {
            resultOfSorting.array[passedSymbols] = arrayOfStrings->array[firstIndex];
            ++firstIndex;
        } else {
            resultOfSorting.array[passedSymbols] = arrayOfStrings -> array[secondIndex];
            ++secondIndex;
        }
        ++passedSymbols;
    }
    linesDestructor(&resultOfSorting);

    for (size_t resultIndex = 0, arrayIndex = left; resultIndex < right - left; ++resultIndex, ++arrayIndex)
        arrayOfStrings->array[arrayIndex] = resultOfSorting.array[resultIndex];
}

int comparatorForQsort(const void *aParam, const void *bParam) {
    assert(aParam != nullptr && bParam != nullptr && "An Error in comparatorForQsort!!!");

    const Line *a = (const Line*) aParam;
    const Line *b = (const Line*) bParam;

    if (a->line == nullptr)
        return Less;
    if (b->line == nullptr)
        return Bigger;

    size_t index1 = 0;
    size_t index2 = 0;
    while (a->line[index1] != '\0' || b->line[index2] != '\0') {

        while (a->line[index1] != '\0' && !isalpha(a->line[index1]))
            ++index1;
        while (a->line[index2] != '\0' && !isalpha(b->line[index2]))
            ++index2;

        if (a->line[index1] < b->line[index2])
            return Less;
        if (b->line[index2] < a->line[index1])
            return Bigger;

        ++index1;
        ++index2;
    }

    return Equal;
}

int originComparator(const void *aParam, const void *bParam) {
    assert(aParam != nullptr && bParam != nullptr && "An Error in originComparator!!!");

    const Line *a = (const Line*) aParam;
    const Line *b = (const Line*) bParam;

    return a -> line - b -> line;
}

int revComparator(const void *aParam, const void *bParam) {
    assert(aParam != nullptr && bParam != nullptr && "An Error in revComparator!!!");

    const Line *a = (const Line*) aParam;
    const Line *b = (const Line*) bParam;

    if (a->line == nullptr)
        return Less;
    if (b->line == nullptr)
        return Bigger;

    int index1 = (int) a -> length - 1;
    int index2 = (int) b -> length - 1;
    while (index1 > -1 || index2 > -1) {

        while (index1 > -1 && ispunct(a->line[index1]))
            --index1;
        while (index2 > -1 && ispunct(b->line[index2]))
            --index2;

        if (index1 == -1)
            return Less;
        if (index2 == -1)
            return Bigger;

        if (a->line[index1] < b->line[index2])
            return Less;
        if (b->line[index2] < a->line[index1])
            return Bigger;

        --index1;
        --index2;
    }

    return Equal;
}
