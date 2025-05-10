#include "../cj.h"

int main() {
    token_stream_t t;
    token_stream_t_init(&t);

    char* src = "{\"foo\": 5.0}";

    token_t tok;
    tok.len = 1;
    tok.start = &src[0];
    tok.tag = OPEN_BRACE;

    token_stream_t_push(&t, tok);

    tok.len = 3;
    tok.start = &src[2];
    tok.tag = STR;

    token_stream_t_push(&t, tok);

    tok.len = 1;
    tok.start = &src[6];
    tok.tag = COLON;

    token_stream_t_push(&t, tok);

    tok.len = 3;
    tok.start = &src[8];
    tok.tag = NUM;

    token_stream_t_push(&t, tok);

    tok.len = 1;
    tok.start = &src[11];
    tok.tag = CLOSE_BRACE;

    token_stream_t_push(&t, tok);

    token_stream_t_deinit(&t);
    return 0;
}
