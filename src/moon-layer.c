#include <pebble.h>
#include <math.h>

#include "moon-layer.h"

#define MAX_MOON_AGE 30

typedef struct {
  int age;
  MoonLayerHemisphere hemisphere;
  GBitmap *moon_bitmap;
  GBitmap *current_phase_bitmap;
} MoonLayerData;

double modulo(double a, double n) {
  return a - n * ((int) (a / n));
}

/**
 * Based on John Conway 'do it in your head' algorithm
 *
 * See: http://www.faqs.org/faqs/astronomy/faq/part3/section-15.html
 * http://www.ben-daglish.net/moon.shtml
 *
 * @param  year  year to calculate
 * @param  month month to calculate
 * @param  day   day to calculate
 * @return       the age of the moon as an in between 0 and 29
 */
double get_moon_age(int year, int month, int day) {
  double r = year % 100;
  r = modulo(r, 19);
  if (r > 9) { r -= 19; }
  r = modulo(r * 11, 30) + month + day;
  if (month < 3) { r += 2; }
  r -= (year < 2000) ? 4 : 8.3;
  r = modulo(floor(r + 0.5), 30);
  return (r < 0) ? r + 30 : r;
}

static void update_proc(Layer *this, GContext *ctx) {
  MoonLayerData* data = layer_get_data(this);
  // Destroy any preexistent sub bitmap
  gbitmap_destroy(data->current_phase_bitmap);

  int y_offset = 0;
  switch (data->hemisphere) {
    case MoonLayerHemisphereNorthern:
      y_offset = data->age;
      break;
    case MoonLayerHemisphereSouthern:
      y_offset = (MAX_MOON_AGE - data->age) % MAX_MOON_AGE;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Hemisphere must be set");
      return;
  }

  GRect rect = GRect(0, y_offset * MOON_RESOURCE_SIZE, MOON_RESOURCE_SIZE, MOON_RESOURCE_SIZE);

  // Create a new sub bitmap and draw it
  data->current_phase_bitmap = gbitmap_create_as_sub_bitmap(data->moon_bitmap, rect);
  graphics_draw_bitmap_in_rect(ctx, data->current_phase_bitmap, layer_get_bounds(this));
}

void moon_layer_set_date(MoonLayer *this, tm *tick_time) {
  MoonLayerData* data = layer_get_data(this->layer);

  int year = 1900 + tick_time->tm_year;
  int month = tick_time->tm_mon + 1;
  int day = tick_time->tm_mday;

  data->age = (int) get_moon_age(year, month, day);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Date: %d/%d/%d - Age: %d", year, month, day, data->age);

  layer_mark_dirty(this->layer);
}

void moon_layer_set_hemisphere(MoonLayer *this, MoonLayerHemisphere hemisphere) {
  MoonLayerData* data = layer_get_data(this->layer);
  data->hemisphere = hemisphere;
  layer_mark_dirty(this->layer);
}

Layer* moon_layer_get_layer(MoonLayer *this) {
  return this->layer;
}

void moon_layer_destroy(MoonLayer *this) {
  layer_destroy(this->layer);
  free(this);
}

MoonLayer* moon_layer_create(GPoint position) {
  MoonLayer* this = (MoonLayer*)malloc(sizeof(MoonLayer));
  this->layer = layer_create_with_data(GRect(position.x - MOON_RESOURCE_SIZE/2, position.y - MOON_RESOURCE_SIZE/2, MOON_RESOURCE_SIZE, MOON_RESOURCE_SIZE), sizeof(MoonLayerData));
  layer_set_update_proc(this->layer, update_proc);

  MoonLayerData *data = (MoonLayerData *)layer_get_data(this->layer);
  data->age = 0;
  data->hemisphere = MoonLayerHemisphereNorthern;
  data->moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_MOON);

  return this;
}
