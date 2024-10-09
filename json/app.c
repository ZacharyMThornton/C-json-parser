#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "json.h"

int main() {
    char path[] = "data.json";
    char element[20];
    printf("what element would you like the value of\n");

    scanf("%s", element);

    const char* val = value(path, element); // returns the address of a char with the values in it
    
    if (val != NULL) { // if the address isn't NULL do this
        printf("%s", val); // no need to dereference the string because it is technically an array and is automaticaly derefereced
        free((void*)val); // free the allocated memory used for the value (the memory must point to a void type)
    }
    
    return 0;
}
