#include "cj.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// HASHMAP IMPL

/**
 * @brief - Hashes a string
 * @param str - the string to hash
 * @return hash number of the string
 */
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

/**
 * @brief - Initializes a JSON Object HashMap
 * @param map - pointer to the HashMap to initialize
 */
void json_object_map_t_init(json_object_map_t* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
}

/**
 * @brief - Deinitializes and frees all allocated memory for a JSON Object HashMap
 * @param map - pointer to the HashMap to initialize
 */
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

/**
 * @brief - Registers a key value json object pair
 * @param map - pointer to the HashMap to initialize
 * @param key - The name of the object to register
 * @param val - pointer to the json object to register, map will not own the pointer and rather perform a deep clone internally, so you have to free this val itself if you malloc'd it.
 */
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

/**
 * @brief - Checks the HashMap for a key, returning a pointer to its JSON object if it exists
 * @param map - Pointer to the HashMap to initialize
 * @param key - Name of the entry to find
 *
 * @return pointer to the JSON object if it exists
 * @return NULL if key doesn't exist
 */
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

// TOKENIZER IMPL

void token_stream_t_init(token_stream_t* s) {
    s->items = malloc(sizeof(token_t) * STREAM_START_SIZE);
    s->capacity = STREAM_START_SIZE;
    s->len = 0;
}

void token_stream_t_deinit(token_stream_t* s) {
    free(s->items);
    s->capacity=0;
    s->len=0;
}

void token_stream_t_push(token_stream_t* s, token_t add) {
    if (s->capacity == 0) {
        token_stream_t_init(s);
    }

    if (s->len < s->capacity) {
        s->items[s->len++] = add;
        return;
    }

    s->capacity *= 2;
    s->items = realloc(s->items, s->capacity * sizeof(token_t));
    s->items[s->len++] = add;
}

int tokenize_json(const char* json, int size, token_stream_t* stream) {
    token_stream_t_init(stream);
    int idx = 0;

    while (idx < size) {
        idx++;
    }

    return 0;
}
