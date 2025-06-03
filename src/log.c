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
  log->entries = NULL;

  return log;  
}

void log_free(log_t *log) {
  if(log->name != NULL) { free(log->name); }
 
  if(log->entries != NULL) {
    for(size_t i = 0; i < log->size; i++) {
      free(log->entries[i]); 
    }
  }

  free(log);
}


