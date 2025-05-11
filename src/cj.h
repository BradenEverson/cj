/**
 * @file cj.h
 *
 * @brief JSON Parsing Single File header and source, containing hashmap, json object and parsing structure definitions
 */


#ifndef CJ_H
#define CJ_H

#define TABLE_SIZE 10
#define STREAM_START_SIZE 10

/**
 * @brief - Types a JSON value can be
 */
typedef enum {
    /**
     * @brief - A floating point number
     */
    NUMBER,
    /**
     * @brief - String value
     */
    STRING,
    /**
     * @brief - A nested JSON object
     */
    OBJECT,
    /**
     * @brief - A boolean
     */
    BOOLEAN,
    /**
     * @brief - NULL values
     */
    NIL,
} value_tag_t;

struct json_object_t;

/**
 * @brief - A linked list structure containing JSON objects at every node, used internally for the hashmap implementation
 * @property key - The name of the field
 * @property value - owned pointer to the value (gets freed at `deinit` time)
 * @property next - Pointer to the next node in this LL
 */
typedef struct json_object_node_t {
    char* key;
    struct json_object_t* value;

    struct json_object_node_t* next;
} json_object_node_t;

/**
 * @brief - A hashtable of Linked List Json Object Nodes
 * @property table - hash table of linked lists
 */
typedef struct {
    json_object_node_t* table[TABLE_SIZE];
} json_object_map_t;

/**
 * @brief - Hashes a string
 * @param str - the string to hash
 * @return hash number of the string
 */
int hash(const char* str);

/**
 * @brief - Initializes a JSON Object HashMap
 * @param map - pointer to the HashMap to initialize
 */
void json_object_map_t_init(json_object_map_t* map);

/**
 * @brief - Deinitializes and frees all allocated memory for a JSON Object HashMap
 * @param map - pointer to the HashMap to initialize
 */
void json_object_map_t_deinit(json_object_map_t* map);

/**
 * @brief - Registers a key value json object pair
 * @param map - pointer to the HashMap to initialize
 * @param key - The name of the object to register
 * @param val - pointer to the json object to register, map will not own the pointer and rather perform a deep clone internally, so you have to free this val itself if you malloc'd it.
 */
void json_object_map_t_insert(json_object_map_t* map, const char* key, struct json_object_t* val);

/**
 * @brief - Checks the HashMap for a key, returning a pointer to its JSON object if it exists
 * @param map - Pointer to the HashMap to initialize
 * @param key - Name of the entry to find
 *
 * @return pointer to the JSON object if it exists
 * @return NULL if key doesn't exist
 */
struct json_object_t* json_object_map_t_get(json_object_map_t* map, const char* key);

/**
 * @brief - Union of all different JSON object leaf types
 * @property number - floating point number
 * @property str - string
 * @property obj - Recursive object map pointer
 * @property boolean - bool
 */
typedef union {
    double number;
    char* str;
    json_object_map_t* obj;
    int boolean;
} value_type_t;

/**
 * @brief - A tagged union JSON object
 * @property tag - JSON object type (either a 'leaf' value or a recursive JSON object map)
 * @property val - Union object value
 */
typedef struct json_object_t {
    value_tag_t tag;
    value_type_t val;
} json_object_t;

/**
 * @brief Parses a JSON buffer into a JSON Object
 * @param json - JSON string buffer
 * @param len - length of the JSON string buffer
 * @param obj - pointer to the JSON object to populate
 */
void json_parse(const char* json, int len, json_object_t* obj);

typedef enum {
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    STR,
    NUM,
    TRUE,
    FALSE,
    NULL_TAG,
    COMMA,
    COLON,
} token_tag_t;

typedef struct {
    const char* start;
    int len;
    token_tag_t tag;
} token_t;

typedef struct {
    token_t* items;
    int len;
    int capacity;
} token_stream_t;

void token_stream_t_init(token_stream_t* s);
void token_stream_t_deinit(token_stream_t* s);
void token_stream_t_push(token_stream_t* s, token_t add);

int tokenize_json(const char* json, int size, token_stream_t* stream);

#endif //CJ_H
