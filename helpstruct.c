#include <stdio.h>
#include "helpstruct.h"

#define TAB() (putc('\t', stdout))
#define NEW_LINE() (putc('\n', stdout))

#define STRINGLEN_ARG_MAX 8
#define DELIMITER '-'
#define DELIMITER_LONG "--"

void helpstruct_print_help(const helpstruct *helparray, size_t nelem,
  const char *preprint, const char *postprint) {
  if (preprint != NULL) {
    fputs(preprint, stdout);
  }
  fputs("Lorsqu'un fichier est passé en argument, affiche les lignes où "\
  "elles se trouvent lorsqu'il y a au moins 2 occurrences de celles ci. "\
  "Lorsque plusieurs fichiers sont passés affiche en argument,les lignes"\
  " en commun avec leur nombre d'occurrence dans chaque fichier.\n", stdout);
  fputs("Liste des commandes:\n", stdout);
  for (size_t k = 0; k < nelem; k += 1) {
    TAB();
    fprintf(stdout, "%c%c ", DELIMITER, helparray[k].op);
    const char *arg = "";
    char sep = ' ';
    if (helparray[k].arity == BINARY) {
      arg = helparray[k].arg;
      sep = '=';
    }
    fprintf(stdout, "%-*s", STRINGLEN_ARG_MAX, arg);
    fputc(' ', stdout);
    fprintf(stdout, "%s%s", DELIMITER_LONG, helparray[k].oplong);
    fprintf(stdout, "%c%-*s", sep, STRINGLEN_ARG_MAX, arg);
    TAB();
    fputs(helparray[k].help, stdout);
    NEW_LINE();
    if (helparray[k].arity == BINARY) {
      TAB();
      fprintf(stdout, "%s disponible(s):", arg);
      NEW_LINE();
      for (size_t j = 0; j < helparray[k].nelem_available; j += 1) {
        TAB();
        fprintf(stdout, "    %-*s%s", STRINGLEN_ARG_MAX, "",
          helparray[k].available[j]);
        NEW_LINE();
      }
    }
  }
  if (postprint != NULL) {
    fputs(postprint, stdout);
  }
}
