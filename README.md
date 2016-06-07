# Pebble Moon Layer

A Layer for Pebble apps that renders a small (![Phases](phases.gif) 10x10) icon representing the moon phase for a given date and hemisphere.

## Usage

1. Install the library by running `pebble package install pebble-moon-layer --save`.
2. Include `moon-layer.h` in your code.

    ```c
    #include <pebble-moon-layer/moon-layer.h>
    ```
5. Create a new ``MoonLayer``. The `GPoint` parameter represents the position of the center-point of the image.

    ```c
    MoonLayer* moon_layer = moon_layer_create(GPoint(20, 20));
    ```

6. Set the date for the layer.

    ```c
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    moon_layer_set_date(moon_layer, tick_time);
    ```

## Other configurations

People in different hemispheres see the moon in a slightly different way, so it's possible to configure the hemisphere where the moon is being viewed from. Possible options are `MoonLayerHemisphereNorthern` and `MoonLayerHemisphereSouthern` (default value is `MoonLayerHemisphereNorthern`).

  ```c
  moon_layer_set_hemisphere(moon_layer, MoonLayerHemisphereSouthern);
  ```

It's also possible to display a 1px border around the icon to improve visibility. Any `GColor` value is accepted. Use `GColorClear` for no border (default values are `GColorWhite` on BW devices and `GColorClear` on Color devices).

  ```c
  moon_layer_set_border_color(moon_layer, GColorBlack);
  ```

![Phases](phases.gif)
