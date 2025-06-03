#include <stdlib.h>
#include <time.h>

typedef struct LogEntry {
  size_t id;
  char *message;
  struct tm datetime;
} log_entry_t;


typedef struct Log {
  size_t size;
  size_t capacity;
  char *name;
  log_entry_t *entries;
} log_t;

//not 100% sure what all we'll need yet
log_t  *log_create(char *name);
void log_update();
void log_delete();
log_t *log_load();
void log_unload();


void log_entry_create();
void log_entry_update();
void log_entry_delete();
log_entry_t *log_entry_load();
void log_entry_unload();







