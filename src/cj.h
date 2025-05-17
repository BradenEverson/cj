/**
 * @file cj.h
 *
 * @brief JSON Parsing Single File header and source, containing hashmap, json object and parsing structure definitions
 */

#ifndef CJ_H
#define CJ_H

#define TABLE_SIZE 10
#define STREAM_START_SIZE 10

#define INDEX_GREATER_THAN_LEN -1
#define UNEXPECTED_TOKEN -2

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
    NULL_VAL,
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
 * @return 0 on success
 * @return negative number on failure
 */
int json_parse(const char* json, int len, json_object_t* obj);

/**
 * @brief Frees all memory tied to the JSON Object if it had any heap stored values (sub-objects or strings)
 * Does not free the underlying pointer
 * @param json - JSON object to deinit
 */
void json_deinit(json_object_t* json);

/**
 * @brief - Token Types
 */
typedef enum {
    /**
     * @brief - {
     */
    OPEN_BRACE,
    /**
     * @brief - }
     */
    CLOSE_BRACE,
    /**
     * @brief - [
     */
    OPEN_BRACKET,
    /**
     * @brief - ]
     */
    CLOSE_BRACKET,
    /**
     * @brief - "
     */
    QUOTATION,
    /**
     * @brief - ,
     */
    COMMA,
    /**
     * @brief - :
     */
    COLON,

    /**
     * @brief - true
     */
    TRUE,
    /**
     * @brief - false
     */
    FALSE,
    /**
     * @brief - null
     */
    NULL_TAG,

    /**
     * @brief - a string
     */
    STR,
    /**
     * @brief - a number
     */
    NUM,
} token_tag_t;

/**
 * @brief - A token with it's type and pointer into the underlying buffer
 * @property start - start of the token in the underlying string
 * @property len - length of the token
 * @property tag - type of the token
 */
typedef struct {
    const char* start;
    int len;
    token_tag_t tag;
} token_t;

/**
 * @brief prints the token out
 * @param t - pointer to the token to print
 */
void token_t_print(token_t* t);

/**
 * @brief prints the token's value within a buffer
 * @param t - pointer to the token to print
 */
void token_t_src_print(token_t* t);

/**
 * @brief - A growing stream of tokens
 * @property items - pointer to the tokens
 * @property len - length of the stream
 * @property capacity - total length of the memory allocated
 */
typedef struct {
    token_t* items;
    int len;
    int capacity;
} token_stream_t;

/**
 * @brief initializes a token stream
 * @param s - pointer to the stream to init
 */
void token_stream_t_init(token_stream_t* s);

/**
 * @brief frees a token stream
 * @param s - pointer to the stream to free
 */
void token_stream_t_deinit(token_stream_t* s);

/**
 * @brief Pushes a new token to the stream
 * @param s - pointer to the stream to add to
 * @param add - new token to add
 */
void token_stream_t_push(token_stream_t* s, token_t add);

/**
 * @brief Tokenizes a string and appends all tokens to a stream
 * @param json - JSON string to tokenize
 * @param size - size of the JSON string
 * @param stream - token stream to append to
 */
int tokenize_json(const char* json, int size, token_stream_t* stream);

#endif //CJ_H
