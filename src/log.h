#include <stdlib.h>

typedef struct LogList {
  size_t capacity;
  size_t filecount;
  char **log_names;
} log_list_t;

typedef struct LogEntry {
  size_t id;
  char *message;
  char *datetime;
} log_entry_t;

typedef struct LogEntryHandler {
  char *log_name;
  log_entry_t log_entry;
} log_entry_handler_t;

typedef struct Log {
  size_t size;
  size_t capacity;
  char *name;
  log_entry_t **entries;
} log_t;

log_list_t *log_list_get();
void log_list_free(log_list_t *log_list);
log_t *log_create(char *name);
log_t *log_new();
log_t *log_load(const char *name);
void log_name_set(log_t *log, const char *name);
void log_free(log_t *log);
void log_update();
void log_delete();
void log_unload();

log_entry_t *log_entry_new(char *file_entry);
log_entry_t *log_entry_load();
void log_entry_add(log_t *log, log_entry_t *entry, size_t entry_size);
void log_entry_list_get(log_t *log);
void log_entry_unload();
void log_entry_create();
void log_entry_update();
void log_entry_delete();

log_entry_handler_t *log_entry_handler_new(char *log_name, log_entry_t *log_entry);
void log_entry_handler_free(log_entry_handler_t *log_entry_handler);
