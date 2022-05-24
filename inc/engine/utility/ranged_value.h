#pragma once



template <typename T>
class RangedValue {
public:
    T min, max, cur;
    T Add(T val) {
        T temp = cur + val;
        if (temp > max) {
            cur = max;
            return temp - max;
        } 
        if (temp < min) {
            cur = min;
            return min - temp;
        }
        cur = temp;
        return 0;
    }
    T Sub(T val) {
        return -Add(-val);
    }
    const T& Get() const {
        return cur;
    }
    void Set(T val) {
        cur = val;
    }
    void SetRange(T min, T max) {
        min = min;
        max = max;
    } 
    bool operator> (const T& o) const {
        return cur > o;
    }
    bool operator>= (const T& o) const {
        return cur >= o;
    }
    bool operator< (const T& o) const {
        return cur < o;
    }
    bool operator<= (const T& o) const {
        return cur <= o;
    }
    bool operator== (const T& o) const {
        return cur == o;
    }
};