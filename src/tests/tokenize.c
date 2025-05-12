#include "../cj.h"

int main() {
    token_stream_t t;

    char* src = "{\"foo\": 5.0}";
    tokenize_json(src, 12, &t);

    token_stream_t_deinit(&t);
    return 0;
}
