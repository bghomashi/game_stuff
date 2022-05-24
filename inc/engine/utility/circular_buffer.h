#pragma once

#include <vector>

template <typename T>
class CircularBuffer {
    std::vector<T> _data;
    int _loc;
public:
    CircularBuffer() : _loc(-1) {}
    CircularBuffer(int size) : _data(size, 0), _loc(0) {}
    T Avg() const {
        T sum = 0;
        for (auto& d : _data)
            sum += d;
        sum /= _data.size();
        return sum;
    }
    void push(T v) {
        _data[_loc++] = v;
        _loc %= _data.size();
    }
    void resize(int size) {
        _data.resize(size, 0);
    }

};