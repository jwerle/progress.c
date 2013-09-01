progress.h
-----

Simple progress bar display for the terminal inspired by [node-progress](https://github.com/visionmedia/node-progress)

## usage

```c
#include <progress.h>
```

## example

```c

#include <progress.h>

void
on_progress (progress_data_t *data);

int
main (void) {
  progress_t *progress = progress_new(100, 60);
  progress->fmt = "progress [:bar] :percent :elapsed";
   
  // listen for progress
  progress_on(progress, PROGRESS_EVENT_PROGRESS, on_progress);
  
  // tick progress
  progress_tick(progress, 10);
}

void
on_progress (progress_data_t *data) {
  progress_write(data->holder);
}

```

**output**

```
progress [======------------------------------------------------------] 10% 0.0s
```

## api

### progress_new(total, width);

```c
progress_t *
progress_new (int total, size_t width);
```

Initializes a new `progress_t *` pointer with a total and width.

***example***

```c
progress_t *progress = progress_new(100, 20);
```

### progress_event_new(event, cb);

```c
progress_event_t *
progress_event_new(progress_event_type_t type);
```

Initializes a new `progress_event_t *` pointer with an event type where `type` is of `enum progress_event_types`. 

***example***

```c
progress_event_t * = progress_event_new(PROGRESS_EVENT_PROGRESS);
```

```c
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
```

## license
MIT
