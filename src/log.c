#include <stdlib.h>
#include <time.h>
#include "log.h"
#include <string.h>


log_t *log_create(char *name) {
  
  log_t *log = malloc(sizeof(log_t));
  if(log == NULL) {
    return NULL;
  }
  log->name = malloc(sizeof(char) * strlen(name));
  if(log->name == NULL) {
    free(log);
    return NULL; 
  }

  log->size = 0;
  log->capacity = sizeof(log);
  strcpy(log->name, name);

  return log;  
}

