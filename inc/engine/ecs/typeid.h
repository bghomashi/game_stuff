#pragma once


struct TypeID {
    // global component counter 
    static unsigned s_count;
public:
    template <typename T>
    static unsigned Get() {
        // the id for the T-component
        static unsigned s_component_id = s_count++;
        return s_component_id;
    }
};