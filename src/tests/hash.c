#include "../cj.h"
#include <stdio.h>

int main () {
    json_object_map_t json_map;
    json_object_map_t_init(&json_map);

    json_object_t val;
    val.tag = NUMBER;
    val.val.number = 5.0;

    json_object_map_t_insert(&json_map, "number", &val);
    json_object_map_t_insert(&json_map, "number", &val);
    json_object_map_t_insert(&json_map, "rumben", &val);

    json_object_t* try = json_object_map_t_get(&json_map, "number");

    printf("%0.1f\n", try->val.number);

    json_object_map_t_deinit(&json_map);
    return 0;
}
