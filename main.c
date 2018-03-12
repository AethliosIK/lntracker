#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpstruct.h"
#include "hashtbl.h"
#include "str_buf.h"
#include "slist.h"
#include "line.h"

typedef enum {
  OP_TRANSFORM, OP_FILTER, OP_HELP
} operator;

helpstruct helparray[] = {
  [OP_TRANSFORM]= {
    .op = 'c',
    .arity = BINARY,
    .arg = "TYPE",
    .oplong = "case",
    .help = "utilise TYPE pour mettre en majuscule ou en"\
      " minuscule chaque ligne(s) du/des fichier(s).",
    .nelem_available = 2, .available = {
      "upper: mettre en majuscule", "lower: mettre en minuscule"
    }
  },
  [OP_FILTER]   = {
    .op = 'f',
    .arity = BINARY,
    .arg = "MOTIF",
    .oplong = "filter",
    .help = "utilise MOTIF pour filtrer chaque ligne(s) des "\
      "fichier(s). MOTIF est une suite de caractère non vide.",
    .nelem_available = 6,
    .available = {
      "a: ajoute le caractère si la fonction isalpha renvoie 1",
      "c: ajoute le caractère si la fonction iscntrl renvoie 1",
      "d: ajoute le caractère si la fonction isdigit renvoie 1",
      "n: ajoute le caractère si la fonction isalnum renvoie 1",
      "p: ajoute le caractère si la fonction ispunct renvoie 1",
      "s: ajoute le caractère si la fonction isspace renvoie 1"
    }
  },
  [OP_HELP]     = {
    .op = 'h',
    .arity = UNARY,
    .oplong = "help",
    .help = "affiche ce menu et ferme le programme."
  },
};

//***************** Prototypes : ***********************************************

char *strstr_r(const char *haystack, const char *needle);
size_t hash_fun(const void *value);
int lfree(line *l);
int sfree(char *s);
int htbl_unique_print(const char *s, line *l);
int htbl_multiple_print(const char *s, line *l);

//******************************************************************************

#define MSG_OPEN  "\033[1m"
#define MSG_CLOSE "\033[0m"

#define ON_VALUE_GOTO(expr, value, label)     \
    if ((expr) == (value)) {                  \
      goto label;                             \
    }
#define NOT_ON_VALUE_GOTO(expr, value, label) \
    if ((expr) != (value)) {                  \
      goto label;                             \
    }

int main(int argc, char *argv[]) {
  for (int k = 1; k < argc; ++k) {
    if (strcmp(argv[k], "--help") == 0 || strcmp(argv[k], "-h") == 0) {
      printf("Uilisation: %s [OPTION]... [FICHIER]...\n", argv[0]);
      helpstruct_print_help(helparray, sizeof helparray / sizeof *helparray,
        MSG_OPEN, MSG_CLOSE);
      return EXIT_SUCCESS;
    }
  }
  if (argc < 2) {
    fprintf(stderr, "Mauvais nombre d'argument.\n");
    fprintf(stderr, "Essayez '%s %s'.\n", argv[0], "--help");
    return EXIT_FAILURE;
  }
  hashtable *htbl = NULL;
  slist *hl = NULL;
  slist *ll = NULL;
  str_buf *s = NULL;
  char *a = NULL;
  char *filters = NULL;
  enum {UPPER, LOWER, NOTHING} transform = NOTHING;
  FILE *f = NULL;
  size_t n = 1;
  ON_VALUE_GOTO(htbl = hashtable_empty(hash_fun,
    (int (*)(const void *, const void *)) strcmp), NULL, error);
  ON_VALUE_GOTO(hl = slist_empty(), NULL, error);
  ON_VALUE_GOTO(ll = slist_empty(), NULL, error);

//************************** Commandes : ***************************************
  #define PRE_DELIMITER_LONG "--"
  #define PRE_DELIMITER "- "
  #define POST_DELIMITER "="

  for (int k = 1; k < argc; ++k) {
    char o[] = PRE_DELIMITER;
    char *r = NULL;
    //Transform
    o[1] = helparray[OP_TRANSFORM].op;
    char ol[strlen(PRE_DELIMITER_LONG) +
      strlen(helparray[OP_TRANSFORM].oplong) +
      strlen(POST_DELIMITER) + 1];
    sprintf(ol, "%s%s%s", PRE_DELIMITER_LONG,
      helparray[OP_TRANSFORM].oplong, POST_DELIMITER);
    if (strcmp(argv[k], o) == 0 || (r = strstr_r(argv[k], ol)) != NULL) {
      char *t = r == NULL ? argv[k += 1] : r;
      if (strcmp(t, "upper") == 0) {
        transform = UPPER;
      }else if(strcmp(t, "lower") == 0) {
        transform = LOWER;
      }
      continue;
    }
    //Filter
    o[1] = helparray[OP_FILTER].op;
    char ol2[strlen(PRE_DELIMITER_LONG) +
      strlen(helparray[OP_FILTER].oplong) +
      strlen(POST_DELIMITER) + 1];
    sprintf(ol2, "%s%s%s", PRE_DELIMITER_LONG,
      helparray[OP_FILTER].oplong, POST_DELIMITER);
    if(strcmp(argv[k], o) == 0 || (r = strstr_r(argv[k], ol2)) != NULL) {
      filters = r == NULL ? argv[k += 1] : r;
      continue;
    }
//******************************************************************************
    if (s == NULL){
      ON_VALUE_GOTO(s = str_buf_empty(filters, transform), NULL, error);
    }
    ON_VALUE_GOTO(f = fopen(argv[k], "r"), NULL, error_f);
    NOT_ON_VALUE_GOTO(slist_apply(ll, (int (*)(void *)) line_add_file),
      0, error);
    int c;
    long int nl = 1;
    while ((c = fgetc(f)) != EOF) {
      if (c != '\n') {
        ON_VALUE_GOTO(str_buf_append(c, s), EOF, error);
      } else if (!(str_buf_is_empty(s))) {
        line *l = (line *)(hashtable_value(htbl, str_buf_string(s)));
        if (l != NULL) {
          NOT_ON_VALUE_GOTO(line_add_occr_on_last_file(l, nl), 0, error);
        } else {
          ON_VALUE_GOTO(l = line_empty(n), NULL, error);
          a = str_buf_duplicate_string(s);
          ON_VALUE_GOTO(a, NULL, error);
          NOT_ON_VALUE_GOTO(line_add_occr_on_last_file(l, nl), 0, error);
          ON_VALUE_GOTO(hashtable_add(htbl, a, l), NULL, error);
          ON_VALUE_GOTO(slist_insert_tail(hl, a), NULL, error);
          ON_VALUE_GOTO(slist_insert_tail(ll, l), NULL, error);
          a = NULL;
        }
        ON_VALUE_GOTO(nl, LONG_MAX, error);
        nl += 1;
        str_buf_reset(s);
      }
    }
    fclose(f);
    n += 1;
    printf("%s\t", argv[k]);
  }
  printf("\n");

  int (*fun)(void *, void *);
  fun = n > 2 ? (int (*)(void *, void *)) htbl_multiple_print :
                     (int (*)(void *, void *)) htbl_unique_print;
  NOT_ON_VALUE_GOTO(slist_apply_context(hl, fun,
    (void *(*)(void *, void *)) hashtable_value, htbl), 0, error);
  printf("\n");
  int r = EXIT_SUCCESS;
  goto dispose;

//*************************  Erreurs : *****************************************

  error_f:
    fprintf(stderr, "Erreur avec le fichier.\n");
    if (f != NULL) {
      fclose(f);
    }
    r = EXIT_FAILURE;
    goto dispose;

  error:
    fprintf(stderr, "Erreur lors d'une allocation.\n");
    r = EXIT_FAILURE;

//************************** Dispose : *****************************************

  dispose:
    free(a);
    str_buf_dispose(&s);
    hashtable_dispose(&htbl);
    if (ll != NULL) {
      slist_apply(ll, (int (*)(void *)) lfree);
    }
    slist_dispose(&ll);
    if (hl != NULL) {
      slist_apply(hl, (int (*)(void *)) sfree);
    }
    slist_dispose(&hl);
  return r;
}

//******************** Fonctions auxilliaires : ********************************

int lfree(line *l) {
  line_dispose(&l);
  return 0;
}

int sfree(char *s) {
  free(s);
  return 0;
}

int htbl_unique_print(const char *s, line *l) {
  return line_print_unique_file(l, s);
}

int htbl_multiple_print(const char *s, line *l) {
  return line_print_multiple_file(l, s);
}

char *strstr_r(const char *haystack, const char *needle) {
  char *r = strstr(haystack, needle);
  return r == NULL ? NULL : r + strlen(needle);
}

size_t hash_fun(const void *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}


