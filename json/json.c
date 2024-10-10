#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* type(char filepath[], char element[]){
    FILE *json = fopen(filepath, "r");
    if (json == NULL){
        fclose(json);
        static char error[40] = "no such file or not enough memory\n";
        return error;
    }
    
    char buffer[256];
    static char type[10] = "";
    while(fgets(buffer, sizeof(buffer), json)){
        if (strstr(buffer, element) != NULL){
            char *characters = strchr(buffer, ':');
            characters++;
            while(*characters == ' ' || *characters == '\t'){
                characters++;
            }
            if (*characters == '"'){
                strcpy(type, "string");
            } else if (*characters == '{'){
                strcpy(type, "object");
            } else {
                strcpy(type, "int");
                
                // this doesn't work right now
                // if the string of characters has a . then it should be a float
                for (int i = 0; i < strlen(characters); i++){
                    if (*characters + i == '.'){
                        strcpy(type, "float");
                    }
                }
                // still returning int even if there is a . in the characters string ??
            }
        }
    }
    fclose(json);
    return type;
}

const char* value(char filepath[], char element[]) 
{
    FILE *json = fopen(filepath, "r");
    if (json == NULL) {
        fclose(json);
        return strdup("no such file or not enough memory\n");
    }

    char buffer[256];
    char* val = (char*)malloc(256*sizeof(char));
    size_t currentSize = 256*sizeof(char);

    int objDetect = 0;
    int bracketCount = 0;
    while(fgets(buffer, sizeof(buffer), json)){
        if (strstr(buffer, element) != NULL && !objDetect){
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
                // the value of the element is a string
                value++;
                char* end = value + strlen(value) - 1;
                while(end > value && (*end == ' ' || *end == '"' || *end == ',' || *end == '\n')){
                    if (strlen(end) > 1){
                        *end = '\0';
                    }else{
                        *end = '\n';
                    }
                    end--;
                }
            }else if (*value == '{'){
                // the value of the element is an object
                objDetect = 1;
                bracketCount++;
            }else{

                // the value of the element is a number
                char *end = value + strlen(value) - 1;
                while (*end == ' ' || *end == '\t' || *end == '\n' || *end == ','){
                    *end = '\0';
                    end--;
                }
                end[1] = '\n';
            }

            strcat(val, value);
            currentSize -= len;
        }else if(objDetect){
            // if there was an object detected add each of it's elements to the value
            // make sure to match each bracket so that it knows when the object has ended
            // an object can have another object in it
            if (strstr(buffer, "{")){
                bracketCount++;
            }
            if (strstr(buffer, "}")){
                bracketCount--;
            }
            // if each open bracket has a closed bracket then the object has been closed
            if (bracketCount == 0){
                objDetect = 0;
            }
            strcat(val, buffer);
        }
    }
    fclose(json);
    if (strcmp(val, "") != 0){
        return val;
    }else{
        return strdup("no matches");
    }
}
