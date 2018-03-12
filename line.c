#include "line.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

//*************************** occrs : ******************************************

typedef struct occrs occrs;

struct occrs {
  long int *array;
  size_t capacity;
  size_t length;
};

#define MIN_OCCRS_ARRAY 11

static occrs *occrs_empty() {
  occrs *o = malloc(sizeof *o);
  if (o == NULL) {
    return NULL;
  }
  o -> array = malloc((sizeof *(o -> array)) * MIN_OCCRS_ARRAY);
  if (o -> array == NULL) {
    free(o);
    return NULL;
  }
  for (size_t k = 0; k < MIN_OCCRS_ARRAY; k += 1) {
    o -> array[k] = 0;
  }
  o -> capacity = MIN_OCCRS_ARRAY;
  o -> length = 0;
  return o;
}

#define OCCRS_REALLOC_FACTOR 2

static int add_occr(occrs *o, long int n) {
  if (o -> length == o -> capacity) {
    if (o -> capacity > SIZE_MAX / 2){
      return FUN_FAILURE;
    }
    long int *a = realloc(o -> array,
      (sizeof *(o -> array)) * (o -> capacity) * OCCRS_REALLOC_FACTOR);
    if (a == NULL) {
      return FUN_FAILURE;
    }
    o -> array = a;
    o -> capacity *= OCCRS_REALLOC_FACTOR;
    for (size_t k = o -> length; k < o -> capacity; k += 1) {
      o -> array[k] = 0;
    }
  }
  o -> array[o -> length] = n;
  o -> length += 1;
  return FUN_SUCCESS;
}

static void occrs_dispose(occrs **ptro) {
  occrs *o = *ptro;
  free(o -> array);
  free(o);
}

//******************************************************************************

struct line {
  occrs **list_file;
  size_t n;
};

line *line_empty(size_t n) {
  line *l = malloc(sizeof *l);
  if (l == NULL) {
    return NULL;
  }
  l -> n = 0;
  l -> list_file = NULL;
  for (size_t k = 0; k < n; k += 1) {
    if (line_add_file(l) != FUN_SUCCESS) {
      return NULL;
    }
  }
  return l;
}

int line_add_file(line *l) {
  occrs **array = NULL;
  if (l -> list_file == NULL) {
    l -> list_file = malloc(sizeof *(l -> list_file));
    if (l -> list_file == NULL) {
      return FUN_FAILURE;
    }
  } else {
    if (l -> n == SIZE_MAX){
      return FUN_FAILURE;
    }
    array = realloc(l -> list_file,
      (sizeof*(l -> list_file)) * (l -> n + 1));
    if (array == NULL) {
      return FUN_FAILURE;
    }
    l -> list_file = array;
  }
  occrs *o = occrs_empty();
  if (o == NULL) {
    free(array);
    return FUN_FAILURE;
  }
  l -> list_file[l -> n] = o;
  l -> n += 1;
  return FUN_SUCCESS;
}

int line_add_occr_on_last_file(line *l, long int n) {
  if (l -> list_file[l -> n - 1] == NULL) {
    return FUN_FAILURE;
  }
  return add_occr(l -> list_file[l -> n - 1], n);
}

void line_dispose(line **ptrl) {
  if (*ptrl != NULL) {
    for (size_t k = 0; k < (*ptrl) -> n; k += 1) {
      occrs_dispose(&((*ptrl) -> list_file[k]));
    }
    free((*ptrl) -> list_file);
    free(*ptrl);
    *ptrl = NULL;
  }
}

//*********************** Fonctions d'affichage : ******************************

static int is_present_in_all_file(line *l) {
  for (size_t k = 0; k < l -> n; k += 1) {
    occrs *o = l -> list_file[k];
    if (o == NULL || o -> length == 0) {
      return 0;
    }
  }
  return 1;
}

static void n_occrs_print(line *l, size_t n) {
  occrs *o = l -> list_file[n];
  printf("%zu", o -> length);
}

static size_t line_print(line *l, size_t n) {
  occrs *o = l -> list_file[n];
  if (o -> length > 1) {
    for(size_t k = 0; k < o -> length; k += 1) {
      if (k < o -> length - 1 ) {
        printf("%zu,", o -> array[k]);
      } else {
        printf("%zu", o -> array[k]);
      }
    }
  }
  return o -> length;
}

int line_print_unique_file(line *l, const char *s) {
  size_t length = line_print(l, 0);
  return (length > 1) ? (printf("\t%s\n", s) < 0) : FUN_SUCCESS;
}

int line_print_multiple_file(line *l, const char *s) {
  if (is_present_in_all_file(l)) {
    for (size_t k = 0; k < l -> n; k += 1) {
      n_occrs_print(l, k);
      printf("\t");
    }
    return (printf("%s\n", s) < 0);
  }
  return FUN_SUCCESS;
}

//******************************************************************************
