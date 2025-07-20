#include <stdlib.h>
#include <time.h>
#include "log.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>

FILE *open_log_file(char *log_name) {
  char *filename =  malloc(sizeof(char) * (strlen("logs/") + strlen(log_name)+1));
  if(filename == NULL) {
    return NULL; 
  }
  strcpy(filename, "logs/");
  strcat(filename, log_name);
  FILE *file = fopen(filename, "a+");
  if(file == NULL) {
    printf("Could not open file: %s\n", log_name);
    return NULL;
  }
  free(filename);
  return file;
}

log_t *log_create(char *name) {
  log_t *log = log_new();
  log_name_set(log, name);
  return log;
}

log_t *log_load(const char *name) {
  log_t *log = log_new(); 
  log_name_set(log, name);
  log_entry_list_get(log);
  return log;
}

void log_entry_list_get(log_t *log) {
  if(log == NULL || log->name == NULL) {
    return;
  }
  FILE *file = open_log_file(log->name);

  size_t buffer = 4096;
  char ln[buffer];
  while(fgets(ln, sizeof(ln), file) != NULL) {
    log_entry_t *entry =  log_entry_load(ln); 
    log_entry_add(log, entry, sizeof(size_t)*(strlen(entry->message)+strlen(entry->datetime)+1));
  }

  fclose(file);
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
  log->size = 0;
  log->capacity = 0; 
  log->entries = NULL;
  return log;
}

void log_name_set(log_t *log, const char *name) {
  if(log == NULL || name == NULL) { return; }
  log->name = malloc(sizeof(char) * strlen(name) + 1);
  if(log->name == NULL) {
    return;
  }
  strcpy(log->name, name);
}

void log_entry_add(log_t *log, log_entry_t *entry, size_t entry_size) {
  if (log->entries == NULL) {
    log->entries = calloc(1, entry_size);
    memcpy(&log->entries[0],  &entry, sizeof(log_entry_t));
    log->capacity = sizeof(entry);
    log->size = 1;
  }
  else {
    log->capacity += sizeof(entry);
    log->entries = realloc(log->entries, sizeof(size_t)*log->capacity);
    memcpy(&log->entries[log->size], &entry, sizeof(log_entry_t));
    log->size += 1;
  }
}

log_entry_t *log_entry_new(size_t id, char *message) {
  log_entry_t *log_entry = malloc(sizeof(log_entry_t));
  if(log_entry == NULL) {
    return NULL;
  }
  log_entry->id = id; 

  time_t now = time(NULL);
  char *datetime = ctime(&now);
  datetime[strcspn(datetime, "\n")] = '\0';
  log_entry->datetime = malloc(sizeof(char)*strlen(datetime)+1);
  strcpy(log_entry->datetime, datetime);

  log_entry->message = malloc(sizeof(char)*strlen(message)+1);
  strcpy(log_entry->message, message);
  
  return log_entry;
}

log_entry_t *log_entry_load(char *file_entry) {
  log_entry_t *entry = malloc(sizeof(log_entry_t));
  if(entry == NULL) {
    return NULL;
  }
  const char *delim = "|";
  //id
  char *token;
  token = strtok(file_entry, delim);
  entry->id = strtoul(token, NULL, 10);
  //date
  token = strtok(NULL, delim);
  entry->datetime = malloc(sizeof(char)*strlen(token)+1);
  strcpy(entry->datetime, token);
  //message
  token = strtok(NULL, delim);
  entry->message = malloc(sizeof(char)*strlen(token)+1);
  strcpy(entry->message, token);
  return entry;
}

void log_entry_create(log_entry_t *log_entry, char *log_name) {
  FILE *file = open_log_file(log_name);
  fprintf(file, "%ld|%s|%s\n", log_entry->id, log_entry->datetime, log_entry->message);
  fclose(file);
}

void log_entry_update(log_entry_t *log_entry, char *log_name, char *message) {
  //TODO
  //  FILE *file = open_log_file(log_name);
  //  find the entry we are looking for and update the message
  //fclose(file);
}

log_entry_handler_t *log_entry_handler_new(char *log_name, log_entry_t *log_entry) {
  log_entry_handler_t *log_entry_handler = malloc(sizeof(log_entry_handler_t));
  log_entry_handler->log_name = malloc(sizeof(char) * strlen(log_name) + 1);

  if(log_entry_handler->log_name == NULL) {
    printf("Unable to allocate log entry handler name");
    return NULL;
  }
  strcpy(log_entry_handler->log_name, log_name); 

  if(log_entry != NULL) {
    log_entry_handler->log_entry = malloc(sizeof(log_entry_t));
    if(log_entry_handler->log_entry == NULL) {
      printf("Unable to allocate log entry in handler");
      free(log_entry_handler->log_name);
      free(log_entry_handler);
      return NULL;
    }
    memcpy(log_entry_handler->log_entry,  log_entry, sizeof(log_entry_t));
  }
  
  return log_entry_handler;
}

void log_entry_free(log_entry_t *log_entry) {
  free(log_entry->message);
  free(log_entry->datetime);
  free(log_entry);
}

void log_free(log_t *log) {
  if(log->name != NULL) { free(log->name); }
  if(log->entries != NULL) {
    for(size_t i = 0; i < log->size; i++) {
      log_entry_free(log->entries[i]);
    }
  }
  free(log);
}

void log_entry_handler_free(log_entry_handler_t *log_entry_handler) {
  free(log_entry_handler->log_name);
  if(log_entry_handler->log_entry != NULL) { free(log_entry_handler->log_entry); }
  free(log_entry_handler);
}
