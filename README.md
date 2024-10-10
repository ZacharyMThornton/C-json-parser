# C-json-parser
json parsing C library (currently a work in progress)

the type() function takes two arguments: char filepath[], and char element[]
the type() function returns a string that tells you the type of value associated with the element in the json file

the value() function takes two arguments: char filepath[], and char element[]
the value() function returns the value of the element that is passed
if there are multiple instances of the same element, the value() function will return each value associated with the element passed

the value() function returns a pointer to dynamic memory, so when finished with the returned value you must free it
the value must be of void type when you free it

ex.)
char* value = value(filepath, element);
// once done
free((void*)value);

