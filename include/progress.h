
#ifndef _PROGRESS_H_
#define _PROGRESS_H_ 1

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PROGRESS_MAX_LISTENER_POOL 32

struct progress_data;

typedef enum {
    PROGRESS_EVENT_START
  , PROGRESS_EVENT_PROGRESS
  , PROGRESS_EVENT_END
} progress_event_type_t;


typedef struct progress_event {
  progress_event_type_t type;
} progress_event_t;

typedef void (* progress_cb_t) (struct progress_data *data);

typedef struct progress_event_listener {
  progress_cb_t handle;
  progress_event_t *event;
  struct progress_data *data;
} progress_event_listener_t;

typedef struct progress {
  int value;
  int total;
  int listener_count;
  double elapsed;
  size_t width;
  time_t start;
  bool started;
  bool finished;
  char *bar_char;
  char *bg_bar_char;
  char *fmt;
  progress_event_listener_t listeners[PROGRESS_MAX_LISTENER_POOL];
} progress_t;


typedef struct progress_data {
  progress_t *holder;
  int value;
  int ts;
} progress_data_t;

progress_t *
progress_new (int total, size_t width);

progress_event_t *
progress_event_new(progress_event_type_t type);

progress_event_listener_t *
progress_event_listener_new (progress_event_t *event, progress_cb_t cb);

void
progress_event_listener_free (progress_event_listener_t *listener);

progress_data_t *
progress_data_new (progress_t *holder, int value);

bool
progress_on (progress_t *progress, progress_event_type_t event, progress_cb_t cb);

bool
progress_emit (progress_t *progress, progress_event_t *event, progress_data_t *data);

bool
progress_tick (progress_t *progress, int value);

void
progress_write (progress_t *progress);

void
progress_free (progress_t *progress);

#endif
