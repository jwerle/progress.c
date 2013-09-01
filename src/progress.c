
#include <progress.h>

char *
itoa (int n) {
  char *s = "";
  sprintf(s, "%d", n);
  return s;
}

char *
ftoa (double n) {
  char *s = "";
  sprintf(s, "%.1f", n);
  return s;
}

#if __STDC_VERSION__ >= 199901L
#define C99
char *
strdup (const char *str) {
  char *cpy = NULL;

  if (str) {
    cpy = malloc(strlen(str)+1);
    if (cpy) strcpy(cpy, str);
  }

  return cpy;
}
#endif

char *
replace_str (char *strbuf, char *strold, char *strnew) {
  char *strret, *p = NULL;
  char *posnews, *posold;
  size_t szold = strlen(strold);
  size_t sznew = strlen(strnew);
  size_t n = 1;

  if (!strbuf) return NULL;
  if (!strold || !strnew || !(p = strstr(strbuf, strold)))
     return strdup(strbuf);

  while (n > 0) {
    if (!(p = strstr(p+1, strold))) break;
    else n++;
  }

  strret = (char *) malloc(strlen(strbuf) - (n * szold )+ (n * sznew) + 1);
  p = strstr(strbuf, strold);
  strncpy(strret, strbuf, (p-strbuf));
  strret[p-strbuf] = 0;
  posold = p+szold;
  posnews = strret+(p-strbuf);
  strcpy(posnews, strnew);
  posnews += sznew;

  while (n > 0) {
    if (!(p = strstr(posold, strold)))
      break;
    strncpy(posnews, posold, p-posold);
    posnews[p-posold] = 0;
    posnews += (p-posold);
    strcpy(posnews, strnew);
    posnews += sznew;
    posold = p+szold;
  }

  strcpy(posnews, posold);
  return strret;
}

progress_t *
progress_new (int total, size_t width) {
  progress_t *progress = malloc(sizeof(progress_t));
  if (!progress) return NULL;
  progress->started = false;
  progress->finished = false;
  progress->value = 0;
  progress->elapsed = 0;
  progress->start = 0;
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
  time_t now = time(NULL);

  if (progress->value > progress->total)
    progress->value = progress->total;

  if (!progress->started) {
    progress->started = true;
    progress->start = time(NULL);

    progress_event_t *event = progress_event_new(PROGRESS_EVENT_START);
    progress_data_t *data = progress_data_new(progress, value);
    progress_emit(progress, event, data);
  }

  progress->elapsed = difftime(now, progress->start);
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
  char *fmt = malloc(512 * sizeof(char));
  char *bar = malloc((complete + incomplete) * sizeof(char));
  char *percent_str = malloc(sizeof(char));
  char *elapsed_str = malloc(sizeof(char));

  sprintf(percent_str, "%d%%", percent);
  sprintf(elapsed_str, "%.1fs", elapsed);

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

  fmt = replace_str(fmt, ":bar", bar);
  fmt = replace_str(fmt, ":percent", percent_str);
  fmt = replace_str(fmt, ":elapsed", elapsed_str);

  printf("%c[2K", 27);
  printf("\r%s", fmt);

  fflush(stdout);
  free(bar);
  free(percent_str);
  free(elapsed_str);
  free(fmt);
}

void
progress_free (progress_t *progress) {
  free(progress);
}


