
#include <progress.h>
#include <assert.h>

void
on_progress_start (progress_data_t *data);

void
on_progress (progress_data_t *data);

void
on_progress_end (progress_data_t *data);

int
main (void) {
  // init with 
  progress_t *progress = progress_new(100, 60);
  progress->fmt = "progress [:bar] :percent :elapsed";

  // set events
  progress_on(progress, PROGRESS_EVENT_START, on_progress_start);
  progress_on(progress, PROGRESS_EVENT_PROGRESS, on_progress);
  progress_on(progress, PROGRESS_EVENT_END, on_progress_end);

  // tick progress
  progress_tick(progress, 10);
  sleep(1);
  progress_tick(progress, 30);
  sleep(1);
  progress_tick(progress, 5);
  sleep(1);
  progress_tick(progress, 10);
  sleep(1);
  progress_tick(progress, 20);
  sleep(1);
  progress_tick(progress, 10);
  sleep(1);
  progress_tick(progress, 5);
  sleep(1);
  progress_tick(progress, 10);
  sleep(1);

}

void
on_progress_start (progress_data_t *data) {
  assert(data);
  puts("\nprogress start");
  progress_write(data->holder);
}

void
on_progress (progress_data_t *data) {
  progress_write(data->holder);
}

void
on_progress_end (progress_data_t *data) {
  puts("\nprogress end");
}

