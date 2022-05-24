#pragma once 

#include <unordered_map>
#include <memory>

class ApplicationState {
public:
    typedef std::shared_ptr<ApplicationState> Ptr_t;

    virtual bool Start() {return true;}; 
    virtual void Stop() {}; 
    virtual void Update(float dt) {};
    virtual void Draw(float alpha) {};
};
