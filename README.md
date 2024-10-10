# C-json-parser
json parsing C library (currently a work in progress) <br>

the type() function takes two arguments: char filepath[], and char element[]<br>
the type() function returns a string that tells you the type of value associated with the element in the json file<br>
<br>
the value() function takes two arguments: char filepath[], and char element[]<br>
the value() function returns the value of the element that is passed<br>
if there are multiple instances of the same element, the value() function will return each value associated with the element passed<br>
<br>
the value() function returns a pointer to dynamic memory, so when finished with the returned value you must free it<br>
the value must be of void type when you free it<br>

ex.)<br>
char* value = value(filepath, element);<br>
// once done<br>
free((void*)value);<br>

