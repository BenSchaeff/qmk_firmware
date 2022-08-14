/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum userspace_layers {
    QWERTY,
	FNLAYER,
	POPTAB,
};

//custom keycodes
#define MOD_MASK_LALT (MOD_BIT(KC_LALT)) // Make ALT layer for encoder use
#define MOD_MASK_LCTRL (MOD_BIT(KC_LCTL)) // Make CTRL layer for encoder use
#define MOD_MASK_LSFT (MOD_BIT(KC_LSFT)) // Make LSHIFT layer for encoder use
#define SWAP_L SGUI(KC_LEFT) // Swap application to left display
#define SWAP_R SGUI(KC_RGHT) // Swap application to right display
#define MINI LGUI(KC_DOWN) // Shrink window
#define MAXI LGUI(KC_UP) // Maximize window
#define CLOSEAPPLICATION LALT(KC_F4) // Kill application
#define CLOSETAB LCTL(KC_W) // Kill current tab
#define POPOUT LCTL(LSFT(KC_K))


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.

    [QWERTY] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_HOME,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_END,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_DELETE,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(FNLAYER),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [FNLAYER] = LAYOUT(
        CLOSEAPPLICATION, CLOSETAB,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MEDIA_PLAY_PAUSE,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, SWAP_L ,  MAXI  , SWAP_R , _______, _______, _______, _______, _______, _______, _______, _______, _______, RESET  ,          _______,
        _______, _______,  MINI  , _______, POPOUT,  _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______,                            _______,                            RGB_TOG, KC_TRNS, _______, _______, _______, _______
    ),

};

//this makes shift+backspace do delete
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Store the current modifier state in the variable for later reference
    mod_state = get_mods();
    switch (keycode) {

    case KC_BSPC:
        {
        // Initialize a boolean variable that keeps track
        // of the delete key status: registered or not?
        static bool delkey_registered;
        if (record->event.pressed) {
            // Detect the activation of either shift keys
            if (mod_state & MOD_MASK_SHIFT) {
                // First temporarily canceling both shifts so that
                // shift isn't applied to the KC_DEL keycode
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                // Update the boolean variable to reflect the status of KC_DEL
                delkey_registered = true;
                // Reapplying modifier state so that the held shift key(s)
                // still work even after having tapped the Backspace/Delete key.
                set_mods(mod_state);
                return false;
            }
        } else { // on release of KC_BSPC
            // In case KC_DEL is still being sent even after the release of KC_BSPC
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        // Let QMK process the KC_BSPC keycode as usual outside of shift
        return true;
    }
    //CTRL + I is up
    case KC_I:
      if (mod_state & MOD_MASK_CTRL){
        if(record->event.pressed) {
            del_mods(MOD_MASK_CTRL);
            register_code(KC_UP);
            set_mods(mod_state);
        } else {
            unregister_code(KC_UP);
        }
      } else {
        return true;
      }
      return false;
    //CTRL + J is left
    case KC_J:
      if (mod_state & MOD_MASK_CTRL){
        if(record->event.pressed) {
            del_mods(MOD_MASK_CTRL);
            register_code(KC_LEFT);
            set_mods(mod_state);
        } else {
            unregister_code(KC_LEFT);
        }
      } else {
        return true;
      }
      return false;
    //CTRL + K is down
    case KC_K:
      if (mod_state & MOD_MASK_CTRL){
        if(record->event.pressed) {
            del_mods(MOD_MASK_CTRL);
            register_code(KC_DOWN);
            set_mods(mod_state);
        } else {
            unregister_code(KC_DOWN);
        }
      } else {
        return true;
      }
      return false;
    //CTRL + L is right
    case KC_L:
      if (mod_state & MOD_MASK_CTRL){
        if(record->event.pressed) {
            del_mods(MOD_MASK_CTRL);
            register_code(KC_RGHT);
            set_mods(mod_state);
        } else {
            unregister_code(KC_RGHT);
        }
      } else {
        return true;
      }
      return false;
    default:
      return true;
    }

    return true;
};

bool encoder_update_user(uint8_t index, bool clockwise)
{
    //ctrl
    if (get_mods() & MOD_MASK_LALT) {
        if (clockwise) {
            tap_code(KC_TAB);
        }
        else {
            tap_code16(S(KC_TAB));
        }
	}
    //alt
    else if (get_mods() & MOD_MASK_LCTRL) {
        if (clockwise) {
            tap_code(KC_TAB);
        }
        else {
            tap_code16(S(KC_TAB));
        }
    }
    //shift
    else if (get_mods() & MOD_MASK_LSFT){
        //we do this a weird way because we don't want shift to acutally be held when using rotary encoder
        if (clockwise) {
            del_mods(MOD_MASK_LSFT);
            tap_code(KC_RGHT);
            set_mods(MOD_MASK_LSFT);
        }
        else {
            del_mods(MOD_MASK_LSFT);
            tap_code(KC_LEFT);
            set_mods(MOD_MASK_LSFT);
        }
    }
    //on function layer
    else if(IS_LAYER_ON(FNLAYER)) {
        if(clockwise){
            tap_code(KC_MEDIA_NEXT_TRACK);
        }
        else {
            tap_code(KC_MEDIA_PREV_TRACK);
        }
    }
    //nothing pressed
    else {
        if (clockwise){
        tap_code(KC_VOLU);
        }
        else{
        tap_code(KC_VOLD);
        }
    }
    return false;
}

  void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch(get_highest_layer(layer_state))
    {  // special handling per layer
            case 0:  //layer one
                if (host_keyboard_led_state().caps_lock)
                    rgb_matrix_set_color_all(255,0,0);
                else
                    //set all to default
                 for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++)
                    {
                        RGB_MATRIX_INDICATOR_SET_COLOR(i,32,32,32);
                    }
                    //light orange keys

                    RGB_MATRIX_INDICATOR_SET_COLOR(0, 255, 153, 51) //esc
                    RGB_MATRIX_INDICATOR_SET_COLOR(1, 255, 153, 51) //`
                    RGB_MATRIX_INDICATOR_SET_COLOR(7, 255, 153, 51)//1
                    RGB_MATRIX_INDICATOR_SET_COLOR(2, 255, 153, 51) //tab
                    ///darker orange keys
                    RGB_MATRIX_INDICATOR_SET_COLOR(8, 255, 128, 0)//q
                    RGB_MATRIX_INDICATOR_SET_COLOR(14, 255, 128, 0)//w
                    RGB_MATRIX_INDICATOR_SET_COLOR(13, 255, 128, 0)//2
                    RGB_MATRIX_INDICATOR_SET_COLOR(19, 255, 128, 0)//3
                    RGB_MATRIX_INDICATOR_SET_COLOR(9, 255, 128, 0)//a
                    RGB_MATRIX_INDICATOR_SET_COLOR(3, 255, 128, 0)//capslock
                    RGB_MATRIX_INDICATOR_SET_COLOR(4, 255, 128, 0)//lshift
                    RGB_MATRIX_INDICATOR_SET_COLOR(5, 255, 128, 0)//lctrl

                    RGB_MATRIX_INDICATOR_SET_COLOR(28, 255, 51, 51) //f5
                    RGB_MATRIX_INDICATOR_SET_COLOR(34, 255, 51, 51) //f6
                    RGB_MATRIX_INDICATOR_SET_COLOR(39, 255, 51, 51) //f7
                    RGB_MATRIX_INDICATOR_SET_COLOR(44, 255, 51, 51) //f8
                    RGB_MATRIX_INDICATOR_SET_COLOR(96, 255, 51, 51) //enter
                    RGB_MATRIX_INDICATOR_SET_COLOR(79, 255, 51, 51) //rightarrow
                    RGB_MATRIX_INDICATOR_SET_COLOR(95, 255, 51, 51) //leftarrow
                    RGB_MATRIX_INDICATOR_SET_COLOR(97, 255, 51, 51) //dwnarrow
                    RGB_MATRIX_INDICATOR_SET_COLOR(94, 255, 51, 51) //uparrow
                    //red keys
                    RGB_MATRIX_INDICATOR_SET_COLOR(11, 255, 51, 51)//win
                    RGB_MATRIX_INDICATOR_SET_COLOR(10, 255, 51, 51)//z
                    RGB_MATRIX_INDICATOR_SET_COLOR(15, 255, 51, 51)//s
                    RGB_MATRIX_INDICATOR_SET_COLOR(20, 255, 51, 51)//e
                    RGB_MATRIX_INDICATOR_SET_COLOR(24, 255, 51, 51)//4
                    //magenta
                    RGB_MATRIX_INDICATOR_SET_COLOR(17, 204, 0, 102)//lalt
                    RGB_MATRIX_INDICATOR_SET_COLOR(16, 204, 0, 102)//x
                    RGB_MATRIX_INDICATOR_SET_COLOR(21, 204, 0, 102)//d
                    RGB_MATRIX_INDICATOR_SET_COLOR(25, 204, 0, 102)//r
                    RGB_MATRIX_INDICATOR_SET_COLOR(29, 204, 0, 102)//5
                    //purple
                    RGB_MATRIX_INDICATOR_SET_COLOR(22, 153, 0, 153)//c
                    RGB_MATRIX_INDICATOR_SET_COLOR(26, 153, 0, 153)//f
                    RGB_MATRIX_INDICATOR_SET_COLOR(30, 153, 0, 153)//t
                    RGB_MATRIX_INDICATOR_SET_COLOR(35, 153, 0, 153)//6
                    //darker blue
                    RGB_MATRIX_INDICATOR_SET_COLOR(27, 76, 0, 153)//v
                    RGB_MATRIX_INDICATOR_SET_COLOR(31, 76, 0, 153)//g
                    RGB_MATRIX_INDICATOR_SET_COLOR(36, 76, 0, 153)//y
                    RGB_MATRIX_INDICATOR_SET_COLOR(40, 76, 0, 153)//7




            break;

            case 1:

                for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++)
                {
                    RGB_MATRIX_INDICATOR_SET_COLOR(i,0,0,0);
                }
                RGB_MATRIX_INDICATOR_SET_COLOR(0, 255, 153, 51) //esc
                RGB_MATRIX_INDICATOR_SET_COLOR(6, 64, 64, 64) //f1
                RGB_MATRIX_INDICATOR_SET_COLOR(8, 255, 128, 0)//q //q
                RGB_MATRIX_INDICATOR_SET_COLOR(14, 255, 128, 0) //w
                RGB_MATRIX_INDICATOR_SET_COLOR(20, 255, 51, 51) //e
                RGB_MATRIX_INDICATOR_SET_COLOR(15, 255, 51, 51)//s
                RGB_MATRIX_INDICATOR_SET_COLOR(26, 153, 0, 153)//f
                RGB_MATRIX_INDICATOR_SET_COLOR(49, 64, 64, 64) //R_Alt
            break;

            default:
                break;
            break;
}

}
