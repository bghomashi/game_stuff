#pragma once

#include <bitset>
#include <map>
#include <functional>
#include <unordered_map>
#include "engine/graphics/graphics.h"
#include "engine/utility/vector2.h"

class Input {
public:
    struct Mods {
        enum Key {
            BEGIN       = __LINE__,
            Shift       = GLFW_MOD_SHIFT,
            Ctrl        = GLFW_MOD_CONTROL,
            Alt         = GLFW_MOD_ALT,
            Super       = GLFW_MOD_SUPER,
            CapsLock    = GLFW_MOD_CAPS_LOCK,
            NumLock     = GLFW_MOD_NUM_LOCK,
            NUM = __LINE__ - BEGIN - 1
        };
    };

    struct Binding {
        int code;
        std::bitset<Mods::NUM> mods;

        bool operator<(const Binding& o) const;
    };

    typedef std::function<void(void)> Action;


    static void OnMouseButton(int button, int action, int mods);
    static void OnMouseMove(double x, double y);
    static void OnKey(int key, int action, int mods);

    static void SetKeyDown(int key, std::bitset<Mods::NUM> mods, Action action);
    static void SetKeyDown(int key, Action action);
    static void SetKeyUp(int key, std::bitset<Mods::NUM> mods, Action action);
    static void SetKeyUp(int key, Action action);

    static bool KeyPressed(int key);
    static bool MousePressed(int button);


    // renaming GLFW_KEY_CODES
    enum KeyCodes {
        BEGIN = __LINE__,
        MouseButton1 = GLFW_MOUSE_BUTTON_1,
        MouseButton2 = GLFW_MOUSE_BUTTON_2,
        MouseButton3 = GLFW_MOUSE_BUTTON_3,
        MouseButton4 = GLFW_MOUSE_BUTTON_4,
        MouseButton5 = GLFW_MOUSE_BUTTON_5,
        MouseButton6 = GLFW_MOUSE_BUTTON_6,
        MouseButton7 = GLFW_MOUSE_BUTTON_7,
        MouseButton8 = GLFW_MOUSE_BUTTON_8,
        MouseLeft    = GLFW_MOUSE_BUTTON_LEFT,
        MouseRight   = GLFW_MOUSE_BUTTON_RIGHT,
        Space       = GLFW_KEY_SPACE,
        Apostrophe  = GLFW_KEY_APOSTROPHE,
        Comma       = GLFW_KEY_COMMA,
        Minus       = GLFW_KEY_MINUS,
        Period      = GLFW_KEY_PERIOD,
        Slash       = GLFW_KEY_SLASH,
        Zero        = GLFW_KEY_0,
        One         = GLFW_KEY_1,
        Two         = GLFW_KEY_2,
        Three       = GLFW_KEY_3,
        Four        = GLFW_KEY_4,
        Five        = GLFW_KEY_5,
        Six         = GLFW_KEY_6,
        Seven       = GLFW_KEY_7,
        Eight       = GLFW_KEY_8,
        Nine        = GLFW_KEY_9,
        SemiColon   = GLFW_KEY_SEMICOLON,
        Equal       = GLFW_KEY_EQUAL,
        A           = GLFW_KEY_A,
        B           = GLFW_KEY_B,
        C           = GLFW_KEY_C,
        D           = GLFW_KEY_D,
        E           = GLFW_KEY_E,
        F           = GLFW_KEY_F,
        G           = GLFW_KEY_G,
        H           = GLFW_KEY_H,
        I           = GLFW_KEY_I,
        J           = GLFW_KEY_J,
        K           = GLFW_KEY_K,
        L           = GLFW_KEY_L,
        M           = GLFW_KEY_M,
        N           = GLFW_KEY_N,
        O           = GLFW_KEY_O,
        P           = GLFW_KEY_P,
        Q           = GLFW_KEY_Q,
        R           = GLFW_KEY_R,
        S           = GLFW_KEY_S,
        T           = GLFW_KEY_T,
        U           = GLFW_KEY_U,
        V           = GLFW_KEY_V,
        W           = GLFW_KEY_W,
        X           = GLFW_KEY_X,
        Y           = GLFW_KEY_Y,
        Z           = GLFW_KEY_Z,
        LeftBracket = GLFW_KEY_LEFT_BRACKET,
        BackSlask   = GLFW_KEY_BACKSLASH,
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,
        World1      = GLFW_KEY_WORLD_1,
        World2      = GLFW_KEY_WORLD_2,
        Escape      = GLFW_KEY_ESCAPE,
        Enter       = GLFW_KEY_ENTER,
        Tab         = GLFW_KEY_TAB,
        BackSpace   = GLFW_KEY_BACKSPACE,
        Insert      = GLFW_KEY_INSERT,
        Delete      = GLFW_KEY_DELETE,
        Right       = GLFW_KEY_RIGHT,
        Left        = GLFW_KEY_LEFT,
        Down        = GLFW_KEY_DOWN,
        Up          = GLFW_KEY_UP,
        PageUp      = GLFW_KEY_PAGE_UP,
        PageDown    = GLFW_KEY_PAGE_DOWN,
        Home        = GLFW_KEY_HOME,
        End         = GLFW_KEY_END,
        CapsLock    = GLFW_KEY_CAPS_LOCK,
        ScrollLock  = GLFW_KEY_SCROLL_LOCK,
        NumLock     = GLFW_KEY_NUM_LOCK,
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
        Pause       = GLFW_KEY_PAUSE,
        F1          = GLFW_KEY_F1,
        F2          = GLFW_KEY_F2,
        F3          = GLFW_KEY_F3,
        F4          = GLFW_KEY_F4,
        F5          = GLFW_KEY_F5,
        F6          = GLFW_KEY_F6,
        F7          = GLFW_KEY_F7,
        F8          = GLFW_KEY_F8,
        F9          = GLFW_KEY_F9,
        F10         = GLFW_KEY_F10,
        F11         = GLFW_KEY_F11,
        F12         = GLFW_KEY_F12,
        F13         = GLFW_KEY_F13,
        F14         = GLFW_KEY_F14,
        F15         = GLFW_KEY_F15,
        F16         = GLFW_KEY_F16,
        F17         = GLFW_KEY_F17,
        F18         = GLFW_KEY_F18,
        F19         = GLFW_KEY_F19,
        F20         = GLFW_KEY_F20,
        F21         = GLFW_KEY_F21,
        F22         = GLFW_KEY_F22,
        F23         = GLFW_KEY_F23,
        F24         = GLFW_KEY_F24,
        F25         = GLFW_KEY_F25,
        KP_Zero     = GLFW_KEY_KP_0,
        KP_One      = GLFW_KEY_KP_1,
        KP_Two      = GLFW_KEY_KP_2,
        KP_Three    = GLFW_KEY_KP_3,
        KP_Four     = GLFW_KEY_KP_4,
        KP_Five     = GLFW_KEY_KP_5,
        KP_Six      = GLFW_KEY_KP_6,
        KP_Seven    = GLFW_KEY_KP_7,
        KP_Eight    = GLFW_KEY_KP_8,
        KP_Nine     = GLFW_KEY_KP_9,
        Decimal     = GLFW_KEY_KP_DECIMAL,
        Divide      = GLFW_KEY_KP_DIVIDE,
        Multiply    = GLFW_KEY_KP_MULTIPLY,
        Subtract    = GLFW_KEY_KP_SUBTRACT,
        Add         = GLFW_KEY_KP_ADD,
        KP_Enter    = GLFW_KEY_KP_ENTER,
        KP_Equal    = GLFW_KEY_KP_EQUAL,
        LeftShift   = GLFW_KEY_LEFT_SHIFT,
        LeftCtrl    = GLFW_KEY_LEFT_CONTROL,
        LeftAlt     = GLFW_KEY_LEFT_ALT,
        LeftSuper   = GLFW_KEY_LEFT_SUPER,
        RightShift  = GLFW_KEY_RIGHT_SHIFT,
        RightCtrl   = GLFW_KEY_RIGHT_CONTROL,
        RightAlt    = GLFW_KEY_RIGHT_ALT,
        RightSuper  = GLFW_KEY_RIGHT_SUPER,
        Menu        = GLFW_KEY_MENU,
        NUM = __LINE__ - BEGIN - 1
    };
private:
    static std::unordered_map<int, bool> s_keys;
    static Vec2 s_mouse;
    static std::map<Binding, Action> s_down_bindings;
    static std::map<Binding, Action> s_up_bindings;
};