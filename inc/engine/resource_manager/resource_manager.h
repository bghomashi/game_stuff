#pragma once

#include <unordered_map>
#include <string>


#ifdef __linux__
// const std::string g_font_directory = "/usr/share/fonts/TTF/";
#elif WIN32
// const std::string g_font_directory = "C:/Windows/Fonts/";
#endif

const std::string g_font_directory = "res/fonts/";

template <typename T>
class ResourceManager {
    static std::unordered_map<std::string, T> s_resources;
public:
    template <typename... Args>
    static bool Load(const std::string& name, Args ...args) {
        if (s_resources.find(name) != s_resources.end()) return true;
        return s_resources[name].Load(args...);
    }
    static void Unload(const std::string& name) {
        s_resources[name].Destroy();
        s_resources.erase(name);
    }
    static T& Get(const std::string& name) {
        return s_resources[name];
    }
};

template <typename T>
std::unordered_map<std::string, T> ResourceManager<T>::s_resources;