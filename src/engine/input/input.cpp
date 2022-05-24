#include "engine/input/input.h"

std::unordered_map<int, bool> Input::s_keys;
Vec2 Input::s_mouse;
std::map<Input::Binding, Input::Action> Input::s_down_bindings;
std::map<Input::Binding, Input::Action> Input::s_up_bindings;


void Input::OnMouseButton(int button, int action, int mods) {
    OnKey(button, action, mods);
}
void Input::OnKey(int key, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {             // down
        s_keys[key] = 1;
        auto key_binding = s_down_bindings.find(Binding{key, std::bitset<6>(mods)});
        if (key_binding != s_down_bindings.end())
            key_binding->second();
    }
    if (action == GLFW_RELEASE) {           // up
        s_keys[key] =  0;
        auto key_binding = s_up_bindings.find(Binding{key, std::bitset<6>(mods)});
        if (key_binding != s_up_bindings.end())
            key_binding->second();
    }
}
void Input::OnMouseMove(double x, double y) {
    s_mouse.x = (float)x;
    s_mouse.y = (float)y;
}
void Input::SetKeyDown(int key, Action action) {
    SetKeyDown(key, 0, action);
}
void Input::SetKeyDown(int key, std::bitset<Mods::NUM> mods, Action action) {
    s_down_bindings.insert(std::make_pair(Binding{key, mods}, action));
}
void Input::SetKeyUp(int key, Action action) {
    SetKeyUp(key, 0, action);
}
void Input::SetKeyUp(int key, std::bitset<Mods::NUM> mods, Action action) {
    s_up_bindings.insert(std::make_pair(Binding{key, mods}, action));
}
bool Input::KeyPressed(int key) {
    return s_keys[key];
}
bool Input::MousePressed(int button) {
    return s_keys[button];
}

bool Input::Binding::operator<(const Binding& o) const {
    return ((code << Mods::NUM) + mods.to_ulong()) < 
            ((o.code << Mods::NUM) + o.mods.to_ulong());
}
