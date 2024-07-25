#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../include/libc/stdio.h"
#include "../include/libc/stdint.h"
#include "../include/libc/stdbool.h"
#include "../include/idt.h"
#include "../include/io.h"
#include "../include/pic.h"


typedef enum {
    ESCPAE_PRESSED          = 0x01,
    ONE_PRESSED             = 0x02,
    TWO_PRESSED             = 0x03,
    THREE_PRESSED           = 0x04,
    FOUR_PRESSED            = 0x05,
    FIVE_PRESSED            = 0x06,
    SIX_PRESSED             = 0x07,
    SEVEN_PRESSED           = 0x08,
    EIGHT_PRESSED           = 0x09,
    NINE_PRESSED            = 0x0A,
    ZERO_PRESSED            = 0x0B,
    DASH_PRESSED            = 0x0C,
    EQUAL_PRESSED           = 0x0D,
    BACKSPACE_PRESSED       = 0x0E,
    TAB_PRESSED             = 0x0F,
    Q_PRESSED               = 0x10,
    W_PRESSED               = 0x11,
    E_PRESSED               = 0x12,
    R_PRESSED               = 0x13,
    T_PRESSED               = 0x14,
    Y_PRESSED               = 0x15,
    U_PRESSED               = 0x16,
    I_PRESSED               = 0x17,
    O_PRESSED               = 0x18,
    P_PRESSED               = 0x19,
    SQ_BRACKETO_PRESSED     = 0x1A,
    SQ_BRACKETC_PRESSED     = 0x1B,
    ENTER_PRESSED           = 0x1C,
    L_CONTROL_PRESSED       = 0x1D,
    A_PRESSED               = 0x1E,
    S_PRESSED               = 0x1F,
    D_PRESSED               = 0x20,
    F_PRESSED               = 0x21,
    G_PRESSED               = 0x22,
    H_PRESSED               = 0x23,
    J_PRESSED               = 0x24,
    K_PRESSED               = 0x25,
    L_PRESSED               = 0x26,
    SEMICOLON_PRESSED       = 0x27,
    SINGLE_QUOTE_PRESSED    = 0x28,
    BACK_TICK_PRESSED       = 0x29,
    L_SHIFT_PRESSED         = 0x2A,
    BACK_SLASH_PRESSED      = 0x2B,
    Z_PRESSED               = 0x2C,
    X_PRESSED               = 0x2D,
    C_PRESSED               = 0x2E,
    V_PRESSED               = 0x2F,
    B_PRESSED               = 0x30,
    N_PRESSED               = 0x31,
    M_PRESSED               = 0x32,
    COMMA_PRESSED           = 0x33,
    DOT_PRESSED             = 0x34,
    FORWARD_SLASH_PRESSED   = 0x35,
    R_SHIFT_PRESSED         = 0x36,
    KP_STAR_PRESSED         = 0x37,
    L_ALFT_PRESSED          = 0x38,
    SPACE_PRESSED           = 0x39,
    CAPS_LOCK_PRESSED       = 0x3A,
    F1_PRESSED              = 0x3B,
    F2_PRESSED              = 0x3C,
    F3_PRESSED              = 0x3D,
    F4_PRESSED              = 0x3E,
    F5_PRESSED              = 0x3F,
    F6_PRESSED              = 0x40,
    F7_PRESSED              = 0x41,
    F8_PRESSED              = 0x42,
    F9_PRESSED              = 0x43,
    F10_PRESSED             = 0x44,
    F11_PRESSED             = 0x57,
    F12_PRESSED             = 0x58,
    NUM_LOCK_PRESSED        = 0x45,
    SCROLL_LOCK_PRESSED     = 0x46,
    KP_7_PRESSED            = 0x47,
    KP_8_PRESSED            = 0x48,
    KP_9_PRESSED            = 0x49,
    KP_MINUS_PRESSED        = 0x4A,
    KP_4_PRESSED            = 0x4B,
    KP_5_PRESSED            = 0x4C,
    KP_6_PRESSED            = 0x4D,
    KP_PLUS_PRESSED         = 0x4E,
    KP_1_PRESSED            = 0x4F,
    KP_2_PRESSED            = 0x50,
    KP_3_PRESSED            = 0x51,
    KP_0_PRESSED            = 0x52,
    KP_DOT_PRESSED          = 0x53,
    // same keys but released
    ESCPAE_RELEASED         = 0x81,
    ONE_RELEASED            = 0x82,
    TWO_RELEASED            = 0x83,
    THREE_RELEASED          = 0x84,
    FOUR_RELEASED           = 0x85,
    FIVE_RELEASED           = 0x86,
    SIX_RELEASED            = 0x87,
    SEVEN_RELEASED          = 0x88,
    EIGHT_RELEASED          = 0x89,
    NINE_RELEASED           = 0x8A,
    ZERO_RELEASED           = 0x8B,
    DASH_RELEASED           = 0x8C,
    EQUAL_RELEASED          = 0x8D,
    BACKSPACE_RELEASED      = 0x8E,
    TAB_RELEASED            = 0x8F,
    Q_RELEASED              = 0x90,
    W_RELEASED              = 0x91,
    E_RELEASED              = 0x92,
    R_RELEASED              = 0x93,
    T_RELEASED              = 0x94,
    Y_RELEASED              = 0x95,
    U_RELEASED              = 0x96,
    I_RELEASED              = 0x97,
    O_RELEASED              = 0x98,
    P_RELEASED              = 0x99,
    SQ_BRACKETO_RELEASED    = 0x9A,
    SQ_BRACKETC_RELEASED    = 0x9B,
    ENTER_RELEASED          = 0x9C,
    L_CONTROL_RELEASED      = 0x9D,
    A_RELEASED              = 0x9E,
    S_RELEASED              = 0x9F,
    D_RELEASED              = 0xA0,
    F_RELEASED              = 0xA1,
    G_RELEASED              = 0xA2,
    H_RELEASED              = 0xA3,
    J_RELEASED              = 0xA4,
    K_RELEASED              = 0xA5,
    L_RELEASED              = 0xA6,
    SEMICOLON_RELEASED      = 0xA7,
    SINGLE_QUOTE_RELEASED   = 0xA8,
    BACK_TICK_RELEASED      = 0xA9,
    L_SHIFT_RELEASED        = 0xAA,
    BACK_SLASH_RELEASED     = 0xAB,
    Z_RELEASED              = 0xAC,
    X_RELEASED              = 0xAD,
    C_RELEASED              = 0xAE,
    V_RELEASED              = 0xAF,
    B_RELEASED              = 0xB0,
    N_RELEASED              = 0xB1,
    M_RELEASED              = 0xB2,
    COMMA_RELEASED          = 0xB3,
    DOT_RELEASED            = 0xB4,
    FORWARD_SLASH_RELEASED  = 0xB5,
    R_SHIFT_RELEASED        = 0xB6,
    KP_STAR_RELEASED        = 0xB7,
    L_ALFT_RELEASED         = 0xB8,
    SPACE_RELEASED          = 0xB9,
    CAPS_LOCK_RELEASED      = 0xBA,
    F1_RELEASED             = 0xBB,
    F2_RELEASED             = 0xBC,
    F3_RELEASED             = 0xBD,
    F4_RELEASED             = 0xBE,
    F5_RELEASED             = 0xBF,
    F6_RELEASED             = 0xC0,
    F7_RELEASED             = 0xC1,
    F8_RELEASED             = 0xC2,
    F9_RELEASED             = 0xC3,
    F10_RELEASED            = 0xC4,
    F11_RELEASED            = 0xD7,
    F12_RELEASED            = 0xD8,
    NUM_LOCK_RELEASED       = 0xC5,
    SCROLL_LOCK_RELEASED    = 0xC6,
    KP_7_RELEASED           = 0xC7,
    KP_8_RELEASED           = 0xC8,
    KP_9_RELEASED           = 0xC9,
    KP_MINUS_RELEASED       = 0xCA,
    KP_4_RELEASED           = 0xCB,
    KP_5_RELEASED           = 0xCC,
    KP_6_RELEASED           = 0xCD,
    KP_PLUS_RELEASED        = 0xCE,
    KP_1_RELEASED           = 0xCF,
    KP_2_RELEASED           = 0xD0,
    KP_3_RELEASED           = 0xD1,
    KP_0_RELEASED           = 0xD2,
    KP_DOT_RELEASED         = 0xD3,
    KP_ENTER_PRESSED        = 0xE01C,
    KP_ENTER_RELEASED       = 0xE09C
} PS2_KBD_CODES;
extern bool wait_key;
extern char last_pressed_key;
extern char keycache[256];
char *get_input();
__attribute__((interrupt)) void kbd_handler(interrupt_frame *frame);

// TODO: Keyboard driver
#endif