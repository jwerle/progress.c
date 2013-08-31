progress.h
-----

## api/example

```c
progress_t *progress = progress_new(total, width);
progress_on(PROGRESS_EVENT_START, on_progress_start);
progress_on(PROGRESS_EVENT_PROGRESS, on_progress);
progress_on(PROGRESS_EVENT_END, on_progress_end);
progress_tick(progress, count);

...

void
on_progress (progress_data_t *data) {
  progress_write(data->holder); // progress [======----------] 20% 0.5x
}
```

## license

MIT
