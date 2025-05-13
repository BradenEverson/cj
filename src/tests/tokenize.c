#include "../cj.h"
#include <stdio.h>

int main() {
    token_stream_t t;

    char* src = "{\"foo\": 5.0}";
    tokenize_json(src, 12, &t);

    for (int i = 0; i < t.len; i++) {
        // token_t_print(&t.items[i]);
        token_t_src_print(&t.items[i]);
    }

    printf("\n");

    token_stream_t_deinit(&t);
    return 0;
}
