#include <stdlib.h>
#include <time.h>
#include "log.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>

log_t *log_create(char *name) {

  log_t *log = log_new();
  log_name_set(log, name);
  log->size = 0;
  log->capacity = sizeof(log);
  log->entries = NULL;
  return log;
}

void log_entry_list_get(log_t *log) {

}

log_t *log_load(char *name) {
  // need to find file by name then parse all the entries into the entries log
  log_t *log = log_new(); //ensure file with name exists, return if not  
  log_name_set(log, name);
  log_entry_list_get(log);
  return log;
}

log_list_t *log_list_get() {
  static const char *LOG_DIR = "logs/";
  struct dirent *de;
  DIR *dr = opendir(LOG_DIR);
  if(dr == NULL) {
    printf("Could not open logs directory\n");
    return NULL;
  }
  size_t capacity = 10;
  char **log_names = calloc(capacity, sizeof(char *));
  if(log_names == NULL) {
    return NULL;
  }

  printf("Logs file opened..\n");
  size_t filecount = 0;
  while((de = readdir(dr)) != NULL) {
     if( strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) { continue; }

     if(filecount == capacity) {
       capacity = capacity*2;
       log_names = (char **)realloc(log_names, sizeof(char *)*capacity);
     }
     
     char *name = malloc(sizeof(char) * strlen(de->d_name) + 1);
     if(name == NULL) {
      return NULL;
     }

     strcpy(name, de->d_name);
     memcpy(&log_names[filecount],  &name, sizeof(char *));
     filecount++;
  }
  closedir(dr);

  log_list_t *log_files = malloc(sizeof(log_list_t));
  if(log_files == NULL) {
    return NULL;
  }
  log_files->capacity = capacity;
  log_files->filecount = filecount;
  log_files->log_names = log_names;

  printf("Filecount: %ld\n",filecount);

  return log_files;
}


void log_list_free(log_list_t *log_list) {
  size_t i = 0;
  while(i < log_list->filecount) {
      char *log = log_list->log_names[i];
      if(log != NULL) {
        free(log);
      }
      i++;
    }
  free(log_list->log_names);
  free(log_list);
}

log_t *log_new() {
  
  log_t *log = malloc(sizeof(log_t));
  if(log == NULL) {
    return NULL; 
  }
  return log;
}

void log_name_set(log_t *log, char *name) {
   if(log == NULL || name == NULL) { return; }
  //TODO - check for existing memory allocation, realloc if necessary - will need for update
  log->name = malloc(sizeof(char)* strlen(name) +1);
  if(log->name == NULL) {
    return;
  }
  strcpy(log->name, name);
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
