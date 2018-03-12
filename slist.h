/*
 *  Sloppy list
 *  Liste dynamique simplement chainée sur le type void *. "Sloppy" car les
 *    éléments dans la liste peuvent être modifiés par slist_apply et
 *    slist_apply_context
 */

#ifndef SLIST_H
#define SLIST_H

#include <stdlib.h>

typedef struct slist slist;

//  slist_empty : crée une liste dynamique simplement chainée sur le type
//    void * initialement vide. Renvoie un pointeur sur l'objet qui gère cette
//    liste en cas de succès et NULL en cas d'échec
extern slist *slist_empty(void);

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type slist * n'est pas l'adresse d'un objet préalablement renvoyé avec
//    succès par slist_empty et non révoqué par slist_dispose depuis. Cette
//    règle ne souffre que d'une seule exception : slist_dispose tolère que la
//    déréférence de son argument ait pour valeur NULL

//  slist_insert_tail : insère x en queue de la liste associée à sl. Renvoie x
//    en cas de succès, NULL en cas d'échec
extern void *slist_insert_tail(slist *sl, void *x);

//  slist_length : renvoie la longueur de la liste associée à sl
extern size_t slist_length(const slist *sl);

//  slist_apply : parcourt séquentiellement la liste sl en exécutant fun sur
//    chacun des éléments. Si, durant le parcours, fun renvoie une valeur non
//    nulle, le parcours prend fin et slist_apply renvoie cette valeur. Sinon,
//    le parcours est mené jusqu'à son terme et slist_apply renvoie zéro
extern int slist_apply(slist *sl, int (*fun)(void *));

//  slist_apply_context : parcourt séquentiellement la liste sl en appelant
//    fun(x, funcontext(context, x)) pour chacun des éléments x. Si, durant le
//    parcours, la valeur de l'appel n'est pas nulle, le parcours prend fin et
//    slist_apply_context renvoie cette valeur. Sinon, le parcours est mené
//    jusqu'à son terme et slist_apply_context renvoie zéro
extern int slist_apply_context(slist *sl,
    int (*fun)(void *arg, void *result_funcontext),
    void *(*funcontext)(void *context, void *arg),
    void *context);

//  slist_dispose : libère les ressources allouées à *ptrsl ainsi qu'à la liste
//    associée et donne à *ptrsl la valeur NULL. Tolère que *ptrsl vaille NULL
extern void slist_dispose(slist **ptrsl);

#endif  // SLIST_H
