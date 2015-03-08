#include <pebble.h>
#include <string.h>

Window *my_window;
TextLayer *text_layer;
ResHandle rh;
char *all_text_buffer;
uint8_t *all_text_buffer_uint;
int n_words = 0;
char** words; // array of words.
char** defns;

static void update_time() {
//   time_t temp = time(NULL);
//   struct tm *tick_time = localtime(&temp);
//   char *text = malloc(sizeof(char) * 8);
//   strncpy(text, all_text_buffer, 8);
  text_layer_set_text(text_layer, (const char*) all_text_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

char** get_words(char* buffer) {
  const char delim[2] = "\n";
  words = malloc(sizeof(char*) * 1);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "starting get_words");

  char* token = strtok(buffer, delim);
  while( token != NULL ) {
    n_words++;
    words = realloc(words, sizeof(char*) * n_words);
    if (words == NULL)
      APP_LOG(APP_LOG_LEVEL_DEBUG, "alloc failed"); //allocation failed
    words[n_words-1] = malloc(sizeof(char)*strlen(token));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "allocated a word");
    strcpy(words[n_words-1], token);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "copied a word");
    token = strtok( NULL, delim );
  }
//   while (buffer) {
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "calling strstr");
//     if (strstr(buffer, delim)) {
//     }
//     buffer = strtok(NULL, delim);
//   }
  return words;
}

char** get_defns(char* buffer) {
  return defns;
}

void handle_init(void) {
  my_window = window_create();
  
  // Load the text from the twoletters.txt file.
  // TODO break up the text, blah
  rh = resource_get_handle(RESOURCE_ID_TWO_LETTERS);
  size_t res_size = resource_size(rh) + 1;
  all_text_buffer_uint = (uint8_t*) malloc(res_size);
  resource_load(rh, all_text_buffer_uint, res_size);
  all_text_buffer = (char*) all_text_buffer_uint;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "calling get_words");  
  // Break the text up into ... an array of words, and an array of defs?
  words = get_words(all_text_buffer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "done with get_words");

  defns = get_defns(all_text_buffer);
  // crashes by here
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorClear);
  text_layer_set_text(text_layer, "yo");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_layer));

  // Show the Window on the watch, with animated=true
  window_stack_push(my_window, true);
  update_time();

}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
