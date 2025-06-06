#include <stdlib.h>
#include <time.h>
#include "log.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>

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


log_list_t *log_list_get() {
  static const char *LOG_DIR = "logs/";
  struct dirent *de;
  DIR *dr = opendir(LOG_DIR);
  if(dr == NULL) {
    printf("Could not open logs directory\n");
    return NULL;
  }
  size_t capacity = 10;
  char **log_list = calloc(capacity, sizeof(char *));
  if(log_list == NULL) {
    return NULL;
  }

  printf("Logs file opened..\n");
  size_t filecount = 0;
  while((de = readdir(dr)) != NULL) {
     if( strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) { continue; }
     if(filecount == capacity) {
       capacity = capacity*2;
       log_list = (char **)realloc(log_list, sizeof(char *)*capacity);
     }

     char *name = malloc(sizeof(char) * strlen(de->d_name) + 1);
     if(name == NULL) {
      return NULL;
     }

     strcpy(name, de->d_name);
     memcpy(&log_list[filecount],  &name, sizeof(char *));
     filecount++;
  }
  closedir(dr);

  log_list_t *log_files = malloc(sizeof(log_list_t));
  if(log_files == NULL) {
    return NULL;
  }
  log_files->capacity = capacity;
  log_files->filecount = filecount;
  log_files->logs = log_list;

  printf("Filecount: %ld\n",filecount);

  return log_files;
}


void log_list_free(log_list_t *log_list) {
  size_t i = 0;
  while(i < log_list->filecount) {
      char *log = log_list->logs[i];
      if(log != NULL) {
        free(log);
      }
      i++;
    }
  free(log_list->logs);
  free(log_list);
}


