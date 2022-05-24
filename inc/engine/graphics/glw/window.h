#ifndef __OGL_WINDOW_H__
#define __OGL_WINDOW_H__

#include <string>
#include <functional>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utility/vector2.h"

namespace OGL {
    struct CursorMode {
        enum {
            Normal = GLFW_CURSOR_NORMAL,            // visible and can leave window
            Hidden = GLFW_CURSOR_HIDDEN,            // invisible when over window
            Disable = GLFW_CURSOR_DISABLED          // invisible and locked to window
        };
    };
    class Window {
        GLFWwindow* _window;

        iVec2 _size;

        static std::map<GLFWwindow*, Window*> s_windowMap;

        static void g_character_callback(GLFWwindow* window, unsigned int codepoint);
        static void g_framebuffer_resize_callback(GLFWwindow* window, int width, int height);
        static void g_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void g_mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);
        static void g_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void MouseBtnCallback(int button, int action, int mods);
        void MousePosCallback(double xpos, double ypos);
        void ResizeCallback(int width, int height);
        void CharacterCallback(char c);
        void KeyCallback(int key, int action, int mods);
    public:
        std::function<void(int, int, int)> mouse_btn_callback;
        std::function<void(double, double)> mouse_pos_callback;
        std::function<void(int, int)> resize_callback;
        std::function<void(char)> character_callback;
        std::function<void(int, int, int)> key_callback;

        iVec2 Size() const;

        bool Create(int width, int height, const std::string& title);
        void Close();

        void HandleEvents();
        bool ShouldQuit() const;
        void Quit();



        void SetCharCallback(GLFWcharfun callback);     //?

        void SetCursorMode(int mode);
        void SetRawMouseMotion(bool);
        bool KeyPressed(int) const;
        bool MousePressed(int) const;
        Vec2 MousePos() const;
        void SwapBuffers() const;

        operator GLFWwindow*() { return _window; };
    };
}


#endif