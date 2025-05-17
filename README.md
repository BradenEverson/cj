# Header Only JSON Parser

Parses a JSON string from either a file or string buffer into a recursive JSON Object data structure. Currently supports the entire JSON spec aside from arrays, which I am currently working on :)

Sample Usage:

```c
#include "../json.h"
#include <stdio.h>
#include <string.h>

int main () {
    const char* json = "{\"person\":{\"name\": \"Teller\", \"age\":7}, \"is_awesome\":true}";
    json_object_t obj;

    json_parse(json, strlen(json), &obj);

    json_object_map_t* person = json_object_map_t_get(obj.val.obj, "person")->val.obj;

    printf("Name: %s\n", json_object_map_t_get(person, "name")->val.str);
    printf("Age: %0.f\n", json_object_map_t_get(person, "age")->val.number);

    printf("Are they awesome? %s\n", json_object_map_t_get(obj.val.obj, "is_awesome")->val.boolean ? "yes" : "no");

    json_deinit(&obj);
}

```
