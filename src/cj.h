#ifndef CJ_H
#define CJ_H

#define TABLE_SIZE 10

typedef enum {
    NUMBER,
    STRING,
    OBJECT,
    BOOLEAN,
    NIL,
} value_tag_t;

struct json_object_t;

typedef struct json_object_node_t {
    char* key;
    struct json_object_t* value;

    struct json_object_node_t* next;
} json_object_node_t;

typedef struct {
    json_object_node_t* table[TABLE_SIZE];
} json_object_map_t;

int hash(const char* str);

void json_object_map_t_init(json_object_map_t* map);
void json_object_map_t_deinit(json_object_map_t* map);

void json_object_map_t_insert(json_object_map_t* map, const char* key, struct json_object_t* val);

struct json_object_t* json_object_map_t_get(json_object_map_t* map, const char* key);

typedef union {
    double number;
    char* str;
    json_object_map_t* obj;
    int boolean;
} value_type_t;

typedef struct json_object_t {
    value_tag_t tag;
    value_type_t val;
} json_object_t;

void json_parse(const char* json, json_object_t* obj);

#endif //CJ_H
