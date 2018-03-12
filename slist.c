#include "slist.h"

typedef struct cslist cslist;

struct cslist {
  void *value;
  cslist *next;
};

struct slist {
  cslist *head;
  cslist *tail;
  size_t length;
};

slist *slist_empty(void) {
  slist *sl = malloc(sizeof *sl);
  if (sl == NULL) {
    return NULL;
  }
  sl -> head = NULL;
  sl -> length = 0;
  return sl;
}

void *slist_insert_tail(slist *sl, void *x) {
  cslist *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p -> value = x;
  p -> next = NULL;
  if (sl -> head == NULL) {
    sl -> head = p;
  } else {
    sl -> tail -> next = p;
  }
  sl -> tail = p;
  sl -> length += 1;
  return x;
}

size_t slist_length(const slist *sl) {
  return sl -> length;
}

int slist_apply(slist *sl, int (*fun)(void *)) {
  cslist *p = sl -> head;
  while (p != NULL) {
    int r = fun(p -> value);
    if (r != 0) {
      return r;
    }
    p = p -> next;
  }
  return 0;
}

int slist_apply_context(slist *sl,
    int (*fun)(void *arg, void *result_funcontext),
    void *(*funcontext)(void *context, void *arg),
    void *context) {
  cslist *p = sl -> head;
  while (p != NULL) {
    int r = fun(p -> value, funcontext(context, p -> value));
    if (r != 0) {
      return r;
    }
    p = p -> next;
  }
  return 0;
}

void slist_dispose(slist **ptrsl) {
  if (*ptrsl != NULL) {
    cslist *p = (*ptrsl) -> head;
    while (p != NULL) {
      cslist *t = p;
      p = p -> next;
      free(t);
    }
    free(*ptrsl);
    *ptrsl = NULL;
  }
}
