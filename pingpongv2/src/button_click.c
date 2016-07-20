#include <pebble.h>

typedef enum {firstScreen, secondScreen, mainScreen} AppState;

static Window *window;
static TextLayer *text_layer;
static TextLayer *text_layer2;
AppState state = firstScreen;
static GFont s_font;
static bool gameTo21 = false;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static BitmapLayer *bitmap_layer;
static GBitmap *bitmap_image;
static BitmapLayer *bitmap_layer2;
static GBitmap *bitmap_image2;
static int playerScore;
static int opponentScore;
static TextLayer *scoreTextTop;
static TextLayer *scoreTextBottom;
static TextLayer *serveText;
static bool winByTwo;
static int playerServe;

// void clearScreen(bool player) {
//     if(player) {
//         text_layer_set_text(text_layer, "You Win!");
//         text_layer_set_text(text_layer2, "You Win!");
//         text_layer_set_text(scoreTextTop, "");
//         text_layer_set_text(scoreTextBottom, "");
//         text_layer_set_text(serveText, "");
//     } else
//
// }

void updateScore() {

    if (gameTo21) {
        if(playerScore == opponentScore && playerScore == 20) {
            winByTwo = true;
        }
    } else {
        if(playerScore == opponentScore && playerScore == 10) {
            winByTwo = true;
        }
    }

    if (!winByTwo) {
        if (gameTo21) {
            if (playerScore >= 21 || opponentScore >= 21) {
                if(playerScore > opponentScore) {
                    text_layer_set_text(text_layer, "You Win!");
                    text_layer_set_text(text_layer2, "You Win!");
                } else {
                    text_layer_set_text(text_layer, "Opponent Wins!");
                    text_layer_set_text(text_layer2, "Opponent Wins!");
                }
            }
        } else {
            if (playerScore >= 11 || opponentScore >= 11) {
                if(playerScore > opponentScore) {
                    text_layer_set_text(text_layer, "You Win!");
                    text_layer_set_text(text_layer2, "You Win!");
                } else {
                    text_layer_set_text(text_layer, "Opponent Wins!");
                    text_layer_set_text(text_layer2, "Opponent Wins!");
                }
            }
        }
    } else {
        if (gameTo21) {
            if (playerScore >= 21 || opponentScore >= 21) {
                if(playerScore > opponentScore && (playerScore - opponentScore) > 1) {
                    text_layer_set_text(text_layer, "You Win!");
                    text_layer_set_text(text_layer2, "You Win!");
                } else if (playerScore < opponentScore && (opponentScore - playerScore) > 1){
                    text_layer_set_text(text_layer, "Opponent Wins!");
                    text_layer_set_text(text_layer2, "Opponent Wins!");
                }
            }
        } else {
            if (playerScore >= 11 || opponentScore >= 11) {
                if(playerScore > opponentScore && (playerScore - opponentScore) > 1) {
                    text_layer_set_text(text_layer, "You Win!");
                    text_layer_set_text(text_layer2, "You Win!");
                } else if (playerScore < opponentScore && (opponentScore - playerScore) > 1){
                    text_layer_set_text(text_layer, "Opponent Wins!");
                    text_layer_set_text(text_layer2, "Opponent Wins!");
                }
            }
        }
    }
    // turn our integer of the number of glasses of water into a string which we can display on the screen
    static char buf[] = "00000000000";    /* <-- implicit NUL-terminator at the end here */
    snprintf(buf, sizeof(buf), "%d", playerScore);

    //finally display the number of glasses of water
    text_layer_set_text(scoreTextTop, buf);

    // turn our integer of the number of glasses of water into a string which we can display on the screen
    static char buf2[] = "00000000000";    /* <-- implicit NUL-terminator at the end here */
    snprintf(buf2, sizeof(buf2), "%d", opponentScore);

    //finally display the number of glasses of water
    text_layer_set_text(scoreTextBottom, buf2);

    int total = playerScore + opponentScore;
    if(gameTo21) {
        if(total%5 == 0) {
            if(playerServe) {
                text_layer_set_text(serveText, "Opponent's Serve");
                playerServe = !playerServe;
            } else {
                text_layer_set_text(serveText, "Your Serve");
                playerServe = !playerServe;
            }
        }
    } else {
        if(total%2 == 0) {
            if(playerServe) {
                text_layer_set_text(serveText, "Opponent's Serve");
                playerServe = !playerServe;
            } else {
                text_layer_set_text(serveText, "Your Serve");
                playerServe = !playerServe;
            }
        }
    }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(state == firstScreen) {
        gameTo21 = true;

        text_layer_set_text(text_layer, "Up if opponent serves first");
        text_layer_set_text(text_layer2, "Down if you serve first");

        state = secondScreen;
    } else if(state == secondScreen) {
        playerServe = true;

        text_layer_set_text(text_layer, "");
        text_layer_set_text(text_layer2, "");

        updateScore();

        text_layer_set_background_color(s_textlayer_1, GColorRed);
        text_layer_set_background_color(s_textlayer_2, GColorBlue);

        bitmap_layer_set_bitmap(bitmap_layer, bitmap_image);
        bitmap_layer_set_bitmap(bitmap_layer2, bitmap_image);

        state = mainScreen;
    } else if (state == mainScreen) {
        playerScore++;
        updateScore();
    }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(state == firstScreen) {

        text_layer_set_text(text_layer, "Up if opponent serves first");
        text_layer_set_text(text_layer2, "Down if you serve first");

        state = secondScreen;
    } else if(state == secondScreen) {

        text_layer_set_text(text_layer, "");
        text_layer_set_text(text_layer2, "");

        updateScore();

        text_layer_set_background_color(s_textlayer_1, GColorRed);
        text_layer_set_background_color(s_textlayer_2, GColorBlue);

        bitmap_layer_set_bitmap(bitmap_layer, bitmap_image);
        bitmap_layer_set_bitmap(bitmap_layer2, bitmap_image);


        state = mainScreen;
    } else if (state == mainScreen) {
        opponentScore++;
        updateScore();
    }
}



static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    /* COLOR LAYERs */
    // s_textlayer_1
    s_textlayer_1 = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h/2));

    text_layer_set_text(s_textlayer_1, "");
    layer_add_child(window_layer, text_layer_get_layer(s_textlayer_1));

    // s_textlayer_2
    s_textlayer_2 = text_layer_create(GRect(0, bounds.size.h/2, bounds.size.w, bounds.size.h/2));

    text_layer_set_text(s_textlayer_2, "");
    layer_add_child(window_layer, text_layer_get_layer(s_textlayer_2));
    /* COLOR LAYERS */

    s_font = fonts_load_custom_font(
        resource_get_handle(RESOURCE_ID_CODE_20));

        text_layer = text_layer_create(GRect(0, 15, bounds.size.w, 80));
        text_layer_set_text(text_layer, "Up for a \ngame to 21");
        text_layer_set_background_color(text_layer, GColorClear);
        text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(text_layer));
        text_layer_set_font(text_layer, s_font);

        text_layer2 = text_layer_create(GRect(0, 105, bounds.size.w, 80));
        text_layer_set_text(text_layer2, "Down for a game to 11");
        text_layer_set_background_color(text_layer2, GColorClear);
        text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(text_layer2));
        text_layer_set_font(text_layer2, s_font);

        bitmap_image = gbitmap_create_with_resource(RESOURCE_ID_CIRC);
        bitmap_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h/2));
        bitmap_layer_set_alignment(bitmap_layer, GAlignCenter);
        bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpSet);
        //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bitmap_layer));

        bitmap_image2 = gbitmap_create_with_resource(RESOURCE_ID_CIRC);
        bitmap_layer2 = bitmap_layer_create(GRect(0, bounds.size.h/2, bounds.size.w, bounds.size.h/2));
        bitmap_layer_set_alignment(bitmap_layer2, GAlignCenter);
        bitmap_layer_set_compositing_mode(bitmap_layer2, GCompOpSet);
        //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bitmap_layer2));

        scoreTextTop = text_layer_create(GRect(-1, 15, bounds.size.w, 80));
        text_layer_set_text(scoreTextTop, "");
        text_layer_set_background_color(scoreTextTop, GColorClear);
        text_layer_set_text_alignment(scoreTextTop, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(scoreTextTop));
        text_layer_set_font(scoreTextTop, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS ));

        scoreTextBottom = text_layer_create(GRect(-1, 98, bounds.size.w, 80));
        text_layer_set_text(scoreTextBottom, "");
        text_layer_set_background_color(scoreTextBottom, GColorClear);
        text_layer_set_text_alignment(scoreTextBottom, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(scoreTextBottom));
        text_layer_set_font(scoreTextBottom, s_font);
        text_layer_set_font(scoreTextBottom, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS ));

        serveText = text_layer_create(GRect(0, bounds.size.h/2, bounds.size.w, 20));
        text_layer_set_text(serveText, "");
        text_layer_set_background_color(serveText, GColorClear);
        text_layer_set_text_alignment(serveText, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(serveText));

    }

    static void window_unload(Window *window) {
        text_layer_destroy(text_layer);
    }

    static void init(void) {
        window = window_create();
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
