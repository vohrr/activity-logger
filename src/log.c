#include <stdlib.h>
#include <time.h>
#include "log.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

const char *LOG_DIR = "logs/";
const char *FILE_EXTENSION = ".txt";
const char *ENTRY_DELIMITER = "|~|";
const char *MULTI_LINE_DELIMITER = "|~~|";

FILE *open_log_file(char *log_name) {
  char *filename =  malloc(sizeof(char) * (strlen(LOG_DIR) + strlen(log_name)+1));
  if(filename == NULL) {
    return NULL; 
  }
  strcpy(filename, LOG_DIR);
  strcat(filename, log_name);
  FILE *file = fopen(filename, "a+");
  if(file == NULL) {
    printf("Could not open file: %s\n", log_name);
    return NULL;
  }
  free(filename);
  return file;
}

void save_log_to_file(log_t *log) {
  size_t buffer = strlen(LOG_DIR) + strlen(log->name) + 1;
  char filename[buffer];
  strcpy(filename, LOG_DIR);
  strcat(filename, log->name);
  FILE *same_name_check = fopen(filename, "r");
  if(same_name_check != NULL) {
    fclose(same_name_check);
    printf("A file with this name already exists.\n");
    return;
  }

  FILE *log_file = fopen(filename, "w");
  if(log_file == NULL) {
    printf("Could not create log file.\n");
  }
  fflush(log_file);
  fsync(fileno(log_file));
  fclose(log_file);
}

log_t *log_create(char *name) {
  log_t *log = log_new();
  size_t buffer = strlen(name) + strlen(FILE_EXTENSION) + 1;
  char filename[buffer];
  strcpy(filename, name);
  strcat(filename, FILE_EXTENSION);
  log_name_set(log, filename);
  save_log_to_file(log);  
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
  //this is far too brittle. implement checks and error handling 
  size_t buffer = 4096;
  char ln[buffer];
  while(fgets(ln, sizeof(ln), file) != NULL) {
     size_t message_buffer = 4096;
     char file_entry[message_buffer];
     strcpy(file_entry, ln);
     //handle multilines
     while(strstr(file_entry, MULTI_LINE_DELIMITER) == NULL) {
        //save current line in buffer and append next line
        strcat(file_entry, fgets(ln, sizeof(ln), file)); 
    }
    log_entry_t *log_entry =  log_entry_load(file_entry); 
    log_entry_add(log, log_entry, sizeof(size_t)*(strlen(log_entry->message)+strlen(log_entry->datetime)+1));
  }

  fclose(file);
}

log_list_t *log_list_get(void) {
  struct dirent *de;
  DIR *dr = opendir(LOG_DIR);
  if(dr == NULL) {
    printf("Could not open logs directory\n");
    return NULL;
  }
  rewinddir(dr);
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

log_t *log_new(void) {
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
    log->capacity = entry_size; 
    log->size = 1;
  }
  else {
    log->capacity += entry_size;
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

  log_entry->message = malloc(sizeof(char)*strlen(message)+2);

  size_t buffer = strlen(message) + 10;
  char delimited_message[buffer];
  strcpy(delimited_message, message);
  strcat(delimited_message, MULTI_LINE_DELIMITER);

  strcpy(log_entry->message, delimited_message);
  return log_entry;
}

log_entry_t *log_entry_load(char *file_entry) {
  log_entry_t *entry = malloc(sizeof(log_entry_t));
  if(entry == NULL) {
    return NULL;
  }
  //id
  char *token;
  token = strtok(file_entry, ENTRY_DELIMITER);
  entry->id = strtoul(token, NULL, 10);
  //date
  token = strtok(NULL, ENTRY_DELIMITER);
  entry->datetime = malloc(sizeof(char)*strlen(token)+1);
  strcpy(entry->datetime, token);
  //message
  token = strtok(NULL, MULTI_LINE_DELIMITER);
  entry->message = malloc(sizeof(char)*strlen(token)+1);
  strcpy(entry->message, token);
  return entry;
}

void log_entry_create(log_entry_t *log_entry, char *log_name) {
  FILE *file = open_log_file(log_name);
  fprintf(file, "%ld%s%s%s%s%s\n", log_entry->id,
          ENTRY_DELIMITER, log_entry->datetime, ENTRY_DELIMITER,
          log_entry->message, MULTI_LINE_DELIMITER);
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
