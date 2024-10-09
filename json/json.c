#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* value(char filepath[], char element[]);
const char* bettervalue(char filepath[], char element[]);


int main() {
    char path[] = "/Users/zacharythornton/Desktop/c/json/data.json";
    char element[20];
    printf("what element would you like the value of\n");

    scanf("%s", element);

    const char* val = bettervalue(path, element); // returns the address of a char with the values in it
    
    if (val != NULL) { // if the address isn't NULL do this
        printf("%s", val); // no need to dereference the string because it is technically an array and is automaticaly derefereced
        free((void*)val); // free the allocated memory used for the value (the memory must point to a void type)
    }
    
    return 0;
}

// retrieves the values associtated with a element
// this function returns a string using dynamicaly allocated memory so be sure to free it when you are done with it
const char* value(char filepath[], char element[]) 
{
    FILE *json = fopen(filepath, "r");

    // checks to see if there is an error opening the file
    if (json == NULL) { 
        return strdup("error"); // copies 'error' into allocated memory
    }

    char *val = (char*)malloc(1024);
    char buffer[256];
    val[0] = '\0';
    size_t remainingSize = 1024;

    while(fgets(buffer, sizeof(buffer), json) != NULL ){
        // checks to see if the element is in the buffer line
        if (strstr(buffer, element) != NULL){
            size_t len = strlen(buffer); // amount of memory in the buffer
            // if the size that remains is less than the amount of memory in the buffer
            if (remainingSize <= len){
                remainingSize += 1024; //put more memory in the remaining size
                val = (char*)realloc(val, remainingSize); // re allocate the amount of memory for the val pointer to the new amount of remaining memory
                // if the value is NULL something bad happend with the memory management
                if (val == NULL){
                    fclose(json);
                    return strdup("memory issue"); // copies 'memory issue' into allocated memory
                }
            }

            char* value = strchr(buffer, ':');
            if (value != NULL){
                value++;
                while (*value == ' ' || *value == '\t'){ // skips over all the spaces
                    value++;
                }

                if (*value == '"'){
                    value++; // value will always have a " a the begining
                }

                char* end = value + strlen(value) - 1;
                while (end > value && (*end == '"' || *end == ',' || *end == '\n' || *end == ' ')) {
                    if (strlen(end) > 1){
                        *end = '\0'; // sets the first value in the address to a end string character
                    }else {
                        *end = '\n'; // sets the first value in the address to a end string character
                    }
                    end--; // sets the end address to the previous addres
                }
            }
            strcat(val, value);
            remainingSize -= len;
        }
    }

    fclose(json);

    if (strcmp(val, "") != 0){
        return val;
    }
    else{
        free(val);
        return strdup("no matches"); //copies 'memory issue' into allocated memory
    }
}

const char* bettervalue(char filepath[], char element[]) 
{
    FILE *json = fopen(filepath, "r");
    if (json == NULL) {
        fclose(json);
        return strdup("no such file or not enough memory\n");
    }

    char buffer[256];
    char correctLine[256];
    char* val = (char*)malloc(256*sizeof(char));
    size_t currentSize = 256*sizeof(char);

    while(fgets(buffer, sizeof(buffer), json)){
        if (strstr(buffer, element) != NULL){
            size_t len = strlen(element);
            if (len >= currentSize){
                currentSize += 256*sizeof(char);
                val = (char*)realloc(val, currentSize);
            }
            if (val == NULL){
                fclose(json);
                return strdup("not enough space\n");
            }
            char *value = strchr(buffer, ':');
            value++;
            while(*value == ' ' || *value == '\t'){
                value++;
            }

            if (*value == '"'){
                value++;
                char* end = value + strlen(value) - 1;
                while(end > value && (*end == ' ' || *end == '"' || *end == ',' || *end == '\n')){
                    if (strlen(end) > 1){
                        *end = '\0';
                    }else{
                        *end = '\n';
                    }
                }
            }else if (*value == '{'){
                fclose(json);
                return strdup("there is a bracket here");
            }else{
                char *end = value + strlen(value) - 1;
                
                /*
                while (*end == ' ' || *end == '\t'){
                    *end = '\0';
                    end--;
                }
                *end = '\n';
                end--;
                */
            }

            strcat(val, value);
            currentSize -= len;
        }
    }
    fclose(json);
    if (strcmp(val, "") != 0){
        return val;
    }else{
        return strdup("no matches");
    }
}