#include "str_buf.h"

struct str_buf {
  char *str;
  size_t length;
  size_t capacity;
  char *filters;
  csize size;
};

#define STR_BUF_MIN 31
#define END_OF_LINE '\0'

str_buf *str_buf_empty(char *filters, csize size) {
  str_buf *s = malloc(sizeof *s);
  if (s == NULL) {
    return NULL;
  }
  s -> str = malloc(STR_BUF_MIN);
  if (s -> str == NULL) {
    free(s);
    return NULL;
  }
  s -> length = 0;
  s -> filters = filters;
  s -> size = size;
  s -> capacity = STR_BUF_MIN;
  *(s -> str) = END_OF_LINE;
  return s;
}
void str_buf_reset(str_buf *s) {
  s -> length = 0;
  *(s -> str) = END_OF_LINE;
}

#define FACTOR_REALLOC 2

int str_buf_append(int c, str_buf *s) {
  if (s -> length == s -> capacity - 1) {
    if (s -> capacity > SIZE_MAX / FACTOR_REALLOC) {
      return EOF;
    }
    char *r = realloc(s -> str, (s -> capacity) * FACTOR_REALLOC);
    if (r == NULL) {
      return EOF;
    }
    s -> str = r;
    s -> capacity *= FACTOR_REALLOC;
  }
  switch(s -> size) {
    case UPPER:
      c = toupper(c);
      break;
    case LOWER:
      c = tolower(c);
      break;
    case NOTHING:
      break;
  }
  if (s -> filters == NULL) {
    *(s -> str + s -> length) = (char)c;
     s -> length += 1;
    *(s -> str + s -> length) = END_OF_LINE;
    return c;
  }
  for (const char *p = s -> filters; *p != END_OF_LINE; p += 1) {
    if ((*p == 'a' && isalpha(c))
    || (*p == 'c' && iscntrl(c))
    || (*p == 'd' && isdigit(c))
    || (*p == 'n' && isalnum(c))
    || (*p == 'p' && ispunct(c))
    || (*p == 's' && isspace(c))) {
      *(s -> str + s -> length) = (char)c;
      s -> length += 1;
      *(s -> str + s -> length) = END_OF_LINE;
      return c;
    }
  }
  return END_OF_LINE;
}

char *str_buf_string(const str_buf *s) {
  return s -> str;
}

char *str_buf_duplicate_string(const str_buf *s) {
  char *a = malloc(s -> length + 1);
  if (a == NULL) {
    return NULL;
  }
  if (strcpy(a, s -> str) == NULL) {
    free(a);
    return NULL;
  }
  return a;
}

int str_buf_is_empty(const str_buf *s) {
  return *(s -> str) == END_OF_LINE;
}

size_t str_buf_capacity(const str_buf *s) {
  return s -> capacity;
}

size_t str_buf_length(const str_buf *s) {
  return s -> length;
}

void str_buf_dispose(str_buf **ptrs) {
  if (*ptrs != NULL) {
    free((*ptrs) -> str);
    free(*ptrs);
    *ptrs = NULL;
  }
}
