#ifndef HELPSTRUCT_H
#define HELPSTRUCT_H

#include <stdlib.h>

#define NELEM_AVAILABLE_MAX 10

typedef struct {
  char op;
  enum { UNARY, BINARY } arity;
  char *arg;
  char *oplong;
  char *help;
  size_t nelem_available;
  char *available[NELEM_AVAILABLE_MAX];
} helpstruct;

//  helpstruct_print_help : fonction d'aide, écris dans la sortie standard
//    les différentes commandes disponibles du programme.
extern void helpstruct_print_help(const helpstruct *helparray, size_t nelem,
  const char *preprint, const char *postprint);

#endif //HELPSTRUCT_H
