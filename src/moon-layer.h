#include <pebble.h>

#define MOON_RESOURCE_SIZE 10

typedef enum {
  MoonLayerHemisphereNorthern,
  MoonLayerHemisphereSouthern
} MoonLayerHemisphere;

typedef struct {
  Layer *layer;
} MoonLayer;

MoonLayer* moon_layer_create(GPoint position);
void moon_layer_destroy(MoonLayer *layer);
Layer* moon_layer_get_layer(MoonLayer *layer);
void moon_layer_set_date(MoonLayer *layer, tm *tick_time);
void moon_layer_set_hemisphere(MoonLayer *layer, MoonLayerHemisphere hemisphere);
void moon_layer_set_border_color(MoonLayer *layer, GColor color);
