
#ifndef CJ_H
#define CJ_H

#define TABLE_SIZE 10

typedef enum {
    NUMBER,
    STRING,
    OBJECT,
    ARRAY,
    BOOLEAN,
    NULL,
} value_tag_t;

struct value_t;

typedef union {
    double number;
    char* string;
    struct value_t* obj;
    struct value_t* array;
    int boolean;
} value_union_t;

struct value_t {
    value_tag_t tag;
    value_union_t val;
};

typedef struct entry_t {
    char* key;
    struct value_t value;
    struct entry_t* next;
} entry_t;

typedef struct {
    entry_t* table[TABLE_SIZE];
} hashmap_t;

void json_parse(const char* json, hashmap_t* map);

#endif //CJ_H
