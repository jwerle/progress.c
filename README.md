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

### progress_event_new(type);

```c
progress_event_t *
progress_event_new(progress_event_type_t type);
```

Initializes a new `progress_event_t *` pointer with an event type where `type` is of `enum progress_event_types`. 

***example***

```c
progress_event_t * = progress_event_new(PROGRESS_EVENT_PROGRESS);
```

### progress_event_listener_new(type, cb);

```c
progress_event_listener_t *
progress_event_listener_new (progress_event_t *event, progress_cb_t cb);
```

Initializes a new `progress_event_listener_t *` pointer bound to an
event with a given callback `progress_cb_t` function pointer.

***example***

```c
...

progress_event_t *event = progress_event_new(PROGRESS_EVENT_PROGRESS);
progress_event_listener_t *listener = progress_event_listener_new(event, onprogress);

...

void
onprogress (progress_data_t *data) {
  printf("value -> %d\n", data->value);
  printf("ts -> %d\n", data->ts);
}
```

### progress_event_listener_free(listener); 

```c
void
progress_event_listener_free (progress_event_listener_t *listener);
```

Frees a `progress_event_listener_t *` pointer

***example***

```c
progress_event_listener_free(listener);
```

### progress_data_new(holder, value);

Initializes a new `progress_data_t *` pointer

```c
progress_data_t *
progress_data_new (progress_t *holder, int value);
```

***example***

```c
progress_data_t * data = progress_data_new(progress, 32);
```

### progress_on(progress, type, cb);

```c
bool
progress_on (progress_t *progress, progress_event_type_t type, progress_cb_t cb);
```

Binds a `progress_cb_t *` pointer to a `progress_event_t` pointer on a `progress_t` pointer.

***example***

```c
progress_on(progress, PROGRESS_EVENT_PROGRESS, onprogress);
```

### progress_emit(progress, event, data);

Emits an event with data.

```c
bool
progress_emit (progress_t *progress, progress_event_t *event, progress_data_t *data);
```

***example***

```c
progress_event_t *event = progress_event_new(PROGRESS_EVENT_START);
progress_data_t *data = progres_data_new(46);
progress_emit(progress, event, data);
```

### progress_tick(progress, value);

Increments progress by a given value.

```c
bool
progress_tick (progress_t *progress, int value);
```

***example***

```c
progress_tick(progress, 16);
```

### progress_write(progress);

Draws the progress bar to stdout.

```c
void
progress_write (progress_t *progress);
```

***example***
```c
progress_write(progress);
// progress [======------------------------------------------------------] 10% 0.0s
```

### progress_free(progress);

Fress a `progress_t *` poiinter.

```c
void
progress_free (progress_t *progress);
```

***example***

```c
progress_free(progress);
```

## license
MIT
