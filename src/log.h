#include <stdlib.h>
#include <time.h>

typedef struct Log {
  size_t size;
  size_t capacity;
  char *name;
  log_entry_t *entries;
} log_t;

typedef struct LogEntry {
  size_t id;
  char *message;
  tm datetime;
} log_entry_t;

//not 100% sure what all we'll need yet
void log_create();
void log_update();
void log_delete();
void log_load();
void log_unload();


void log_entry_create();
void log_entry_update();
void log_entry_delete();
void log_entry_load();
void log_entry_unload();







