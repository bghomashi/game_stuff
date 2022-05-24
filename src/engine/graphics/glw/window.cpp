#include "glw/window.h"

#include <iostream>

char* char_utf32_to_utf8(char32_t utf32, const char* buffer);

namespace OGL {

    std::map<GLFWwindow*, Window*> Window::s_windowMap;


    void Window::g_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        // possible check against map if windows
        OGL::Window* myWindow = Window::s_windowMap[window];
        myWindow->KeyCallback(key, action, mods);
    }
    void Window::g_character_callback(GLFWwindow* window, unsigned int codepoint) {
        // possible check against map if windows
        OGL::Window* myWindow = Window::s_windowMap[window];
        char buffer[5];
        char_utf32_to_utf8(codepoint, buffer);
        myWindow->CharacterCallback(buffer[0]);
    }
    void Window::g_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
        // possible check against map if windows
        OGL::Window* myWindow = Window::s_windowMap[window];
        myWindow->ResizeCallback(width, height);
    }
    void Window::g_mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
        // possible check against map if windows
        OGL::Window* myWindow = Window::s_windowMap[window];
        myWindow->MousePosCallback(xpos, ypos);
    }
    void Window::g_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        // possible check against map if windows
        OGL::Window* myWindow = Window::s_windowMap[window];
        myWindow->MouseBtnCallback(button, action, mods);
    }



    void Window::ResizeCallback(int width, int height) {
        _size.x = width;
        _size.y = height;
        if (resize_callback) {
            resize_callback(width, height);
        }

    }
    void Window::CharacterCallback(char c) {
        if (character_callback) {
            character_callback(c);
        }
    }
    void Window::KeyCallback(int key, int action, int mods) {
        if (key_callback) {
            key_callback(key, action, mods);
        }
    }
    void Window::MouseBtnCallback(int button, int action, int mods) {
        if (mouse_btn_callback) {
            mouse_btn_callback(button, action, mods);
        }
    }
    void Window::MousePosCallback(double xpos, double ypos) {
        if (mouse_pos_callback) {
            mouse_pos_callback(xpos, _size.y - ypos);
        }
    }
    bool Window::Create(int width, int height, const std::string& title) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }    
        
        glfwSetKeyCallback(_window, g_key_callback);
        glfwSetCharCallback(_window, g_character_callback);
        glfwSetFramebufferSizeCallback(_window, g_framebuffer_resize_callback); 
        glfwSetCursorPosCallback(_window, g_mouse_pos_callback); 
        glfwSetMouseButtonCallback(_window, g_mouse_button_callback);

        glViewport(0, 0, width, height);
        _size = {width, height};
        s_windowMap.insert(std::make_pair(_window, this));
        return true;
    }

    void Window::Close() {
        s_windowMap.erase(_window);
        glfwDestroyWindow(_window);
        glfwTerminate();
        _window = NULL;
    }

    bool Window::ShouldQuit() const {
        return glfwWindowShouldClose(_window);
    }
    void Window::Quit() {
        glfwSetWindowShouldClose(_window, true);
    }
    void Window::SetCursorMode(int mode) {
        glfwSetInputMode(_window, GLFW_CURSOR, mode);
    }
    void Window::SetRawMouseMotion(bool mode) {
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, mode);
    }

    Vec2 Window::MousePos() const {
        double xpos, ypos;
        glfwGetCursorPos(_window, &xpos, &ypos);
        return Vec2{ (float)xpos, (float)(_size.y - ypos) };
    }

    bool Window::KeyPressed(int key) const {
        return glfwGetKey(_window, key) == GLFW_PRESS;
    }
    bool Window::MousePressed(int button) const {
        return glfwGetMouseButton(_window, button) == GLFW_PRESS;
    }
    void Window::SetCharCallback(GLFWcharfun callback) {
        glfwSetCharCallback(_window, callback);
    }
    void Window::SwapBuffers() const {
        glfwSwapBuffers(_window);
    }
    iVec2 Window::Size() const {
        return _size;
    }
    void Window::HandleEvents() {
        glfwPollEvents();
    }
}



char* char_utf32_to_utf8(char32_t utf32, const char* buffer)
// Encodes the UTF-32 encoded char into a UTF-8 string. 
// Stores the result in the buffer and returns the position 
// of the end of the buffer
// (unchecked access, be sure to provide a buffer that is big enough)
{
    char* end = const_cast<char*>(buffer);
    if(utf32 < 0x7F) *(end++) = static_cast<unsigned>(utf32);
    else if(utf32 < 0x7FF) {
        *(end++) = 0b1100'0000 + static_cast<unsigned>(utf32 >> 6);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else if(utf32 < 0x10000){
        *(end++) = 0b1110'0000 + static_cast<unsigned>(utf32 >> 12);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    } else if(utf32 < 0x110000) {
        *(end++) = 0b1111'0000 + static_cast<unsigned>(utf32 >> 18);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 12) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    *end = '\0';
    return end;
}
