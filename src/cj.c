#include "cj.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int hash(const char* str) {
    if (str == NULL) {
        return -1;
    }

    int result = 0;
    int idx = 0;

    while (str[idx] != '\0') {
        result += str[idx++];
    }

    return result;
}

void json_object_map_t_init(json_object_map_t* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
}

void json_object_map_t_deinit(json_object_map_t* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        json_object_node_t* curr = map->table[i];

        while (curr != NULL) {
            free(curr->key);
            free(curr->value);

            json_object_node_t* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
}

void json_object_map_t_insert(json_object_map_t* map, const char* key, struct json_object_t* val) {
    int hashed = hash(key);
    int idx = hashed % TABLE_SIZE;

    json_object_node_t* curr = map->table[idx];

    json_object_t* clone = malloc(sizeof(json_object_t));
    *clone = *val;

    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            free(curr->value);

            curr->value = clone;
            return;
        }
        curr = curr->next;
    }

    json_object_node_t* new = malloc(sizeof(json_object_node_t));
    new->next = NULL;
    new->key = strdup(key);
    new->value = clone;

    new->next = map->table[idx];
    map->table[idx] = new;
}

json_object_t* json_object_map_t_get(json_object_map_t* map, const char* key) {
    int hashed = hash(key);
    int idx = hashed % TABLE_SIZE;

    json_object_node_t* curr = map->table[idx];

    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }

        curr = curr->next;
    }

    return NULL;
}
