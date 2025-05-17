#include "cj.h"
#include <stddef.h>
#include <stdio.h>
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
            json_deinit(curr->value);
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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_alphabetic(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c  <= 'z') || c == '_';
}

int is_numeric(char c) {
    return c >= '0' && c <= '9';
}

int is_alphanumeric(char c) {
    return is_alphabetic(c) || is_numeric(c);
}

int tokenize_json(const char* json, int size, token_stream_t* stream) {
    token_stream_t_init(stream);
    int idx = 0;

    int flag = 1;
    token_t tok = {0};

    while (idx < size) {
        tok.start = json + idx;
        tok.len = 1;
        flag = 1;

        switch (json[idx]) {
            case '[': 
                tok.tag = OPEN_BRACKET;
                break;

            case ']': 
                tok.tag = CLOSE_BRACKET;
                break;

            case '{': 
                tok.tag = OPEN_BRACE;
                break;

            case '}': 
                tok.tag = CLOSE_BRACE;
                break;

            case ',': 
                tok.tag = COMMA;
                break;

            case ':':
                tok.tag = COLON;
                break;

            case '"':
                tok.tag = QUOTATION;
                break;

            case ' ':
            case '\t':
            case '\n':
                while (is_whitespace(json[idx])) idx++;
                flag = 0;
                break;

            default:
                if (is_alphabetic(json[idx])) {
                    const char* start = json + idx;

                    while (is_alphanumeric(json[idx])) {
                        idx++;
                        tok.len++;
                    }

                    idx--;
                    tok.tag = STR;
                    tok.len--;

                    char buf[tok.len + 1];
                    strncpy(buf, start, tok.len);
                    buf[tok.len] = '\0';

                    if (strcmp(buf, "true") == 0) {
                        tok.tag = TRUE;
                    } else if (strcmp(buf, "false") == 0) {
                        tok.tag = FALSE;
                    } else if (strcmp(buf, "null") == 0) {
                        tok.tag = NULL_TAG;
                    }

                } else if (is_numeric(json[idx])) {
                    while (is_numeric(json[idx]) || json[idx] == '.') {
                        idx++;
                        tok.len++;
                    }
                    tok.tag = NUM;
                    idx--;
                    tok.len--;

                } else {
                    return 1;
                }

                break;
        }

        if (flag) {
            token_stream_t_push(stream, tok);
            idx++;
        }
    }

    return 0;
}

void token_t_print(token_t* t) {
    const char* tag;

    switch (t->tag) {
        case OPEN_BRACE:
            tag = "OPEN_BRACE";
            break;

        case CLOSE_BRACE:
            tag = "CLOSE_BRACE";
            break;

        case OPEN_BRACKET:
            tag = "OPEN_BRACKET";
            break;

        case CLOSE_BRACKET:
            tag = "CLOSE_BRACKET";
            break;

        case QUOTATION:
            tag = "QUOTATION";
            break;

        case COMMA:
            tag = "COMMA";
            break;

        case NUM:
            tag = "NUM";
            break;

        case TRUE:
            tag = "TRUE";
            break;

        case COLON:
            tag = "COLON";
            break;

        case FALSE:
            tag = "FALSE";
            break;

        case NULL_TAG:
            tag = "NULL_TAG";
            break;

        case STR:
            tag = "STR";
            break;

    }

    printf("%s - %d\n", tag, t->len);
}

void token_t_src_print(token_t* t) {
    for (int offset = 0; offset < t->len; offset++) {
        printf("%c", (t->start + offset)[0]);
    }
}

static int parse_value(token_stream_t* s, int* idx, json_object_t* obj);
static int parse_object(token_stream_t* s, int* idx, json_object_t* obj);
static int parse_number(token_stream_t* s, int* idx, json_object_t* obj);
static int parse_boolean(token_stream_t* s, int* idx, json_object_t* obj);
static int parse_null(token_stream_t* s, int* idx, json_object_t* obj);
static int parse_string(token_stream_t* s, int* idx, json_object_t* obj);


static int parse_object(token_stream_t* s, int* idx, json_object_t* obj) {
    obj->tag = OBJECT;
    json_object_map_t* map = malloc(sizeof(json_object_map_t));
    obj->val.obj = map;
    json_object_map_t_init(obj->val.obj);

    // Skip {
    (*idx)++;

    while (*idx < s->len) {
        token_t* t = &s->items[*idx];

        if (t->tag == CLOSE_BRACE) {
            (*idx)++;
            return 0;
        }

        json_object_t key_obj;
        parse_string(s, idx, &key_obj);
        char* key = key_obj.val.str;

        t = &s->items[*idx];
        if (t->tag != COLON) {
            free(key);
            return UNEXPECTED_TOKEN;
        }
        (*idx)++;

        json_object_t val_obj;
        parse_value(s, idx, &val_obj);
        
        json_object_map_t_insert(obj->val.obj, key, &val_obj);
        free(key);
        
        t = &s->items[*idx];
        if (t->tag == COMMA) {
            (*idx)++;
        } else if (t->tag != CLOSE_BRACE) {
            return UNEXPECTED_TOKEN;
        }
    }

    return 0;
}

static int parse_number(token_stream_t* s, int* idx, json_object_t* obj) {
    token_t* t = &s->items[*idx];
    const char* start = t->start;

    char buf[t->len + 1];
    strncpy(buf, start, t->len);
    buf[t->len] = '\0';

    obj->tag = NUMBER;
    obj->val.number = strtof(buf, NULL);

    (*idx)++;
    return 0;
}

static int parse_boolean(token_stream_t* s, int* idx, json_object_t* obj) {
    token_t* t = &s->items[*idx];
    obj->tag = BOOLEAN;

    switch (t->tag) {
        case TRUE:
            obj->val.boolean = 1;
            break;

        case FALSE:
            obj->val.boolean = 0;
            break;
    
        default:
            // Should technically be unreachable
            return UNEXPECTED_TOKEN;
            break;
    }

    (*idx)++;
    return 0;
}

static int parse_null(token_stream_t* s, int* idx, json_object_t* obj) {
    obj->tag = NULL_VAL;
    (*idx)++;
    return 0;
}

static int parse_string(token_stream_t* s, int* idx, json_object_t* obj) {
    // skip the "
    (*idx)++;

    token_t* t = &s->items[*idx];
    if(t->tag != STR) {
        return UNEXPECTED_TOKEN;
    }

    const char* start = t->start;

    // TODO - we need to create a deinit for json_object_t that switches on 
    // the tag and frees strings/nested objects
    char* buf = malloc((t->len + 1) * sizeof(char));
    strncpy(buf, start, t->len);
    buf[t->len] = '\0';

    obj->tag = STRING;
    obj->val.str = buf;

    // skip the " again and move on
    (*idx) += 2;
    return 0;
}

static int parse_value(token_stream_t* s, int* idx, json_object_t* obj) {
    if (*idx >= s->len) return INDEX_GREATER_THAN_LEN;
    token_t* t = &s->items[*idx];
    switch (t->tag) {
        case OPEN_BRACE:
            parse_object(s, idx, obj);
            break;

        case QUOTATION:
            parse_string(s, idx, obj);
            break;

        case NUM:
            parse_number(s, idx, obj);
            break;

        case TRUE:
        case FALSE:
            parse_boolean(s, idx, obj);
            break;

        case NULL_TAG:
            parse_null(s, idx, obj);
            break;

        default:
            return UNEXPECTED_TOKEN;
            break;
    }

    return 0;
}

/**
 * @brief Parses a JSON buffer into a JSON Object
 * @param json - JSON string buffer
 * @param len - length of the JSON string buffer
 * @param obj - pointer to the JSON object to populate
 * @return 0 on success
 * @return negative number on failure
 */
int json_parse(const char* json, int len, json_object_t* obj) {
    token_stream_t s;
    tokenize_json(json, len, &s);

    int idx = 0;

    int return_code = parse_value(&s, &idx, obj);

    token_stream_t_deinit(&s);
    return return_code;
}

void json_deinit(json_object_t* json) {
    switch (json->tag) {
        case STRING:
            free(json->val.str);
            break;

        case OBJECT:
            json_object_map_t_deinit(json->val.obj);
            free(json->val.obj);
            break;

        case NUMBER: 
        case BOOLEAN:
        case NULL_VAL:
        default:
            break;
    }
}
