#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

typedef struct line line;

//  line_empty: alloue une structure de type line avec n comme nombre de fichier
//    renvoie un pointeur en cas de succès de l'allocation sinon renvoie NULL.
extern line *line_empty(size_t n);

//  line_add_file: Permet d'ajouter une case mémoire pour un nouveau fichier
//    pour une structure de type line.
extern int line_add_file(line *l);

//  line_add_occr_on_last_file: Ajoute le numéro de la ligne n dans le tableau
//    à la case mémoire correspondant au dernier fichier.
extern int line_add_occr_on_last_file(line *l, long int n);

//  line_print_unique_file: Affiche les différentes occurrences de la ligne,
//    avec s la chaine de caractère de cette ligne. Utilisée lorsqu'il n'y a
//    qu'un seul fichier.
extern int line_print_unique_file(line *l, const char *s);

//  line_print_multiple_file: Affiche le nombre d'occurrence de la ligne l pour
//    chaque fichier si cette même ligne est présente dans tous les fichiers.
//    Utilisée lorsque plusieurs fichiers sont présents.
extern int line_print_multiple_file(line *l, const char *s);

//  line_dispose: libère la mémoire de la structure pointée par ptrl.
extern void line_dispose(line **ptrl);

#endif  // LINE_H
