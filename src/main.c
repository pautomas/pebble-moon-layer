#include <pebble.h>

#include "moon-layer.h"

static Window *window;
static TextLayer *date_layer;
static TextLayer *instructions_layer;

static int diff_days = 0;
static int default_hemisphere = MoonLayerHemisphereNorthern;

static MoonLayer *moon_layer;

static void refresh_phase_bitmap() {
  // Get a tm structure
  time_t temp = time(NULL);
  temp += diff_days * 24 * 60 * 60;
  struct tm *tick_time = localtime(&temp);

  moon_layer_set_date(moon_layer, tick_time);

  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b %Y", tick_time);
  text_layer_set_text(date_layer, date_buffer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  default_hemisphere = default_hemisphere == MoonLayerHemisphereNorthern ? MoonLayerHemisphereSouthern : MoonLayerHemisphereNorthern;
  moon_layer_set_hemisphere(moon_layer, default_hemisphere);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  diff_days += 1;
  refresh_phase_bitmap();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  diff_days -= 1;
  refresh_phase_bitmap();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  instructions_layer = text_layer_create(GRect(0, 5, bounds.size.w, 40));
  text_layer_set_text_alignment(instructions_layer, GTextAlignmentCenter);
  text_layer_set_font(instructions_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_background_color(instructions_layer, GColorClear);
  text_layer_set_text(instructions_layer, "Up/Down: Change day\nSelect: Change hemisphere");
  text_layer_set_text_color(instructions_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(instructions_layer));

  date_layer = text_layer_create(GRect(0, 120, bounds.size.w, 20));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(date_layer));

  moon_layer = moon_layer_create(grect_center_point(&bounds));
  layer_add_child(window_layer, moon_layer_get_layer(moon_layer));

  refresh_phase_bitmap();
}

static void window_unload(Window *window) {
  text_layer_destroy(instructions_layer);
  text_layer_destroy(date_layer);
  moon_layer_destroy(moon_layer);
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
