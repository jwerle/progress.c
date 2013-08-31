
#include <progress.h>

char *
itoa (int n) {
  char *s;
  sprintf(s, "%d", n);
  return s;
}

char *
ftoa (double n) {
  char *s;
  sprintf(s, "%.1f", n);
  return s;
}

char *replace_str(const char *str, const char *old, const char *new) {
  char *ret, *r;
  const char *p, *q;
  size_t oldlen = strlen(old);
  size_t count, retlen, newlen = strlen(new);
  if (oldlen != newlen) {
    for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
      count++;
      /* this is undefined if p - str > PTRDIFF_MAX */
      retlen = p - str + strlen(p) + count * (newlen - oldlen);
  } else {
    retlen = strlen(str);
  }



  if ((ret = malloc(retlen + 1)) == NULL)
    return NULL;

  for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
    /* this is undefined if q - p > PTRDIFF_MAX */
    ptrdiff_t l = q - p;
    memcpy(r, p, l);
    r += l;
    memcpy(r, new, newlen);
    r += newlen;
  }

  strcpy(r, p);

  return ret;
}

progress_t *
progress_new (int total, size_t width) {
  progress_t *progress = malloc(sizeof(progress_t));
  if (!progress) return NULL;
  progress->started = false;
  progress->finished = false;
  progress->value = 0;
  progress->listener_count = 0;
  progress->bar_char = "=";
  progress->bg_bar_char = "-";
  progress->fmt = "";
  progress->total = total;
  progress->width = (width)? (int) width : total;
  return progress;
}

progress_event_t *
progress_event_new (progress_event_type_t type) {
  progress_event_t *event = malloc(sizeof(progress_event_t));
  if (!event) return NULL;
  event->type = type;
  return event;
}

progress_event_listener_t *
progress_event_listener_new (progress_event_t *event, progress_cb_t cb) {
  progress_event_listener_t *listener = malloc(sizeof(progress_event_listener_t));
  if (!listener) return NULL;
  listener->event = event;
  listener->handle = cb;
  return listener;
}

progress_data_t *
progress_data_new (progress_t *holder, int value) {
  progress_data_t *data = malloc(sizeof(progress_data_t));
  data->holder = holder;
  data->value = value;
  data->ts = (int) time(NULL);
  return data;
}

void
progress_event_listener_free (progress_event_listener_t *listener) {
  free(listener->data);
  free(listener);
}

bool
progress_on (progress_t *progress, progress_event_type_t event, progress_cb_t cb) {
  progress_event_listener_t *listener = progress_event_listener_new(progress_event_new(event), cb);
  if (!listener) return false;
  progress->listeners[progress->listener_count++] = *listener;
  return true;
}

bool
progress_emit (progress_t *progress, progress_event_t *event, progress_data_t *data) {
  int count = progress->listener_count;

  for (int i = 0; i < count; ++i) {
    progress_event_listener_t *listener = &progress->listeners[i];
    if (event->type == listener->event->type) {
      listener->data = data;
      listener->handle(data);
    }
  }

  return false;
}

bool
progress_tick (progress_t *progress, int value) {
  if (progress->finished) return false;
  progress->value += value;

  progress->elapsed = difftime(time(NULL), progress->start);
  if (progress->value > progress->total)
    progress->value = progress->total;

  if (!progress->started) {
    progress->started = true;
    progress->start = time(NULL);
    progress_event_t *event = progress_event_new(PROGRESS_EVENT_START);
    progress_data_t *data = progress_data_new(progress, value);
    progress_emit(progress, event, data);
  }

  progress_event_t *event = progress_event_new(PROGRESS_EVENT_PROGRESS);
  progress_data_t *data = progress_data_new(progress, value);
  progress_emit(progress, event, data);

  if (progress->value >= progress->total) {
    progress->finished = true;
    progress->value = progress->total;
    progress_event_t *event = progress_event_new(PROGRESS_EVENT_END);
    progress_data_t *data = progress_data_new(progress, value);
    progress_emit(progress, event, data);
    progress_free(progress);
  }

  return true;
}

void
progress_write (progress_t *progress) {
  int i = 0;
  int width = (int) progress->width;
  int percent = 100 * ((double) progress->value / (double) progress->total);
  int complete = round(width * ((double) progress->value / (double) progress->total));
  int incomplete = width - (complete);
  double elapsed = progress->elapsed;
  char *fmt = malloc(sizeof(char));
  char *bar = malloc((complete + incomplete) * sizeof(char));

  strcpy(fmt, "");
  strcat(fmt, progress->fmt);
  strcpy(bar, "");

  if (complete) {
    for (i = 0; i < complete; ++i) {
      bar[i] = *progress->bar_char;
    }
  }

  if (incomplete) {
    for (; i < complete + incomplete; ++i) {
       bar[i] = *progress->bg_bar_char;
    }
  }

  // get a new reference
  fmt = replace_str(fmt,"`", "");
  fmt = replace_str(fmt, ":bar", bar);
  fmt = replace_str(fmt, ":percent", strcat(itoa(percent), "%"));
  fmt = replace_str(fmt, ":elapsed", strcat(ftoa(elapsed), "s"));

  printf("%c[2K", 27);
  printf("\r%s", fmt);

  fflush(stdout);
  free(bar);
}

void
progress_free (progress_t *progress) {
  free(progress);
}


