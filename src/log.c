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


char **log_list_get() {
  static const char *LOG_DIR = "logs/";
  struct dirent *de;
  DIR *dr = opendir(LOG_DIR);
  if(dr == NULL) {
    printf("Could not open logs directory\n");
    return NULL;
  }

  char **log_list = calloc(10, sizeof(char *));
  if(log_list == NULL) {
    return NULL;
  }

  printf("Logs file opened..\n");
  size_t filecount = 0;
  while((de = readdir(dr)) != NULL) {
     if( strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) { continue; }
     if(filecount == sizeof(log_list)) {
       log_list = calloc(sizeof(log_list) * 2, sizeof(char *));     
     }

     char *name = malloc(sizeof(char) * strlen(de->d_name) + 1);
     if(name == NULL) {
      printf("Failed to allocate memory for %s file\n", de->d_name);
      return NULL;
     }

     strcpy(name, de->d_name);
     memcpy(&log_list[filecount],  &name, sizeof(char *));
     filecount++;
  }
  closedir(dr);
  printf("Filecount: %ld\n",filecount);
  return log_list;
}

void log_list_free(char **log_list) {
  size_t i = 0;
  while(i < sizeof(log_list)) {
      char *log = log_list[i];
      if(log != NULL) { 
        free(log);
      }
      i++;
    }
  free(log_list);
}


