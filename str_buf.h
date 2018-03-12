#ifndef STR_BUF_H
#define STR_BUF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

typedef struct str_buf str_buf;

typedef enum {UPPER, LOWER, NOTHING} csize;

// str_buf_empty: alloue une structure de type str_buf; renvoie un pointeur en
//  cas de succès de l'allocation sinon renvoie NULL.
extern str_buf *str_buf_empty(char *filters, csize size);

// str_buf_reset: réinitialise la longueur d'une structure de type str_buf à 0
//  et sa chaîne de caractère au caractère de fin de ligne.
extern void str_buf_reset(str_buf *s);

//  str_buf_append: ajoute à s le caractère c. Si tout se passe bien, s pointe
//    vers la réallocation et retourne le caractère ajouté. Si le caractère est
//    filtré, la fonction renvoie le caractère de fin de ligne. En cas de
//    dépassement de capacité la chaîne str est réallouée, aucune modification
//    ne se produit et renvoie EOF.
extern int str_buf_append(int c, str_buf *s);

//  str_buf_string: retourne la chaîne de caractère d'une structure
//    de type str_buf.
extern char *str_buf_string(const str_buf *s);

//  str_buf_duplicate_string: retourne une copie de la chaîne de caractère de s
//    en allouant un nouveau espace mémoire, renvoie NULL en cas d'erreur.
extern char *str_buf_duplicate_string(const str_buf *s);

//  str_buf_capacity: retourne la capacité de la chaîne de caractère d'une
//    structure de type str_buf.
extern size_t str_buf_capacity(const str_buf *s);

//  str_buf_length: retourne la longueur de la chaîne de caractère de s sans
//    compter le caractère de fin de ligne.
extern size_t str_buf_length(const str_buf *s);

//  str_buf_is_empty: retourne 1 si la chaine de caractère ne contient que le
//    caractère de fin de ligne sinon renvoie false.
extern int str_buf_is_empty(const str_buf *s);

//  str_buf_dispose: libère la mémoire de la structure pointée par ptrs et la
//    chaîne de caractère de cette même structure.
extern void str_buf_dispose(str_buf **ptrs);

#endif  // STR_BUF_H
