# Pebble Moon Layer

A Layer for Pebble apps that renders a small (![Phases](phases.gif) 10x10) icon representing the moon phase for a given date and hemisphere.

## Usage

1. Add `src/moon-layer.h` and `src/moon-layer.c` to your `src` folder.
2. Copy `resources/phases.png` to your `resources` folder.
3. Include the file in your `appinfo.json` resources list.

    ```json
    {
        "type": "bitmap",
        "name": "MOON",
        "file": "phases.png"
    }
    ```
4. Include `moon-layer.h` in your code.

    ```c
    #include "moon-layer.h"
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

7. Optionally set the hemisphere. Possible options are `MoonLayerHemisphereNorthern` or `MoonLayerHemisphereSouthern` (default value is `MoonLayerHemisphereNorthern`).

    ```c
    moon_layer_set_hemisphere(moon_layer, MoonLayerHemisphereSouthern);
    ```

![Phases](phases.gif)
