
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

typedef struct {
    // hashmap of char* -> json_object_t
} json_object_map_t;

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
