#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cassert>

namespace utl {

template <typename T>
class slot_map;

//  handle to object in slot_map
class Handle {
    template <typename T>
    friend class slot_map;

    std::uint16_t index, key;
public:
    bool operator==(const Handle& o) const {
		return index == o.index && key == o.key;
    }
    bool operator!=(const Handle& o) const {
		return !(*this == o);
    }
};

//  system - holds all the components
template <typename T>
class slot_map {
	struct Slot {
		std::uint16_t index, key;
	};
    struct Item {
        T object;
		std::uint16_t slot_index, key;
    };

	std::vector<std::uint16_t> _free_slots;
    std::vector<Item> _items;             // components
    std::vector<Slot> _slots;             // components

	std::uint16_t popFreeList() {
		assert(_free_slots.size() > 0);

		std::uint16_t slot_index = _free_slots.back();
		_free_slots.pop_back();
		return slot_index;
	}
	void pushFreeList(std::uint16_t index) {
		_free_slots.push_back(index);
		_slots[index].index = 0;
		_slots[index].key = 0;
	}
	void VALIDATE() const  {
		static std::ofstream debug("C:/Users/bghom/Dropbox/Development/0_Engine/engine_0_3/debug.txt");

		for (auto a : _slots)
			debug << "SLOTINDEX = " << a.index << " " << a.key << std::endl;

		for (int i = 0; i < _slots.size(); i++) {
			if (_slots[i].key != 0) continue;

			for (int j = 0; j < _slots.size(); j++) {
				if (j == i) continue;
				if (_slots[j].key != 0) continue;
				if (_slots[j].index == _slots[i].index) exit(10);
			}
		}
	}

	unsigned _total_count;		// temp to see total count
    unsigned _count;               // num alloced items
	std::uint16_t _next_key;            // [1..2^16] (don't let == 0)
    unsigned _capacity;            // max size of the array
public: 
    slot_map() = default;
    slot_map(unsigned capacity) {
        Initialize(capacity);
    }
    void Initialize(unsigned capacity) {
        _capacity = capacity;
		_total_count = 0;
        _count = 0;                 // no object
        _next_key = 1;              // next key
        
        _items.resize(capacity);
        _slots.resize(capacity);
		_free_slots.resize(capacity);
		for (unsigned i = 0; i < capacity; i++) {
			_free_slots[i] = capacity - i - 1;
		}
    }
    unsigned Size() const {
        return _count;
    }

    Handle Create() {
        Handle id;
        Create(id);
        return id;
    }
    T* Create(Handle& id) {
		id.index = 0;
		id.key = 0;

		std::uint16_t slot_index = popFreeList();
		_total_count++;

        
        id.index = slot_index;							// index to this slot
		id.key = _next_key;								// record key
		_slots[slot_index].index = _count;              // slots points to the first available item
		_slots[slot_index].key = _next_key;				// record key (for matching with handle)


		//VALIDATE();


        _items[_count].slot_index = slot_index;			// index of slot 
		_items[_count].key = _next_key;					// key that matches slot
        
		_next_key++;

        return &_items[_count++].object;				// return a pointer to the item
    }
    T* Get(Handle id) {
        unsigned slot_index = _slots[id.index].index;
        return &_items[slot_index].object;
    }
    const T* Get(Handle id) const {
		unsigned slot_index = _slots[id.index].index;
        return &_items[slot_index].object;
    }
    Handle Get(const T& o) const {
        Handle id;
        Item* item = (Item*)&o;
		id.index = item->slot_index;
		id.key = item->key;
        return id;
    }
    T* TryGet(Handle id) {
        unsigned slot_index = _slots[id.index].index;

        if (_slots[id.index].key == 0 || id.key != _slots[id.index].key)
            return NULL;

        return &_items[slot_index].object;
    }
    void Remove(Handle id) {
        unsigned item_index = _slots[id.index].index;

        if (id.key == 0 || _slots[id.index].key == 0 || id.key != _slots[id.index].key)
            return;

		pushFreeList(id.index);
        //_items[item_index].key = 0;
		//VALIDATE();// push free slot

        // swap object
        unsigned last_index = _items[_count-1].slot_index;			// slot_index
        unsigned last_key	= _slots[last_index].key;				// 

		_slots[last_index].index = item_index;
		_slots[last_index].key = last_key;

		std::swap(_items[item_index], _items[_count-1]);
        
        _count--;
    }
	// loop till true
	bool ForEachTillTrue(std::function<bool(T&)> func) {
		for (unsigned i = 0; i < _count; i++)
			if (func(_items[i].object)) 
				return true;
		return false;
	}
    // simple for each loop
    void ForEach(std::function<void(T&)> func) {
        for (unsigned i = 0; i < _count; i++)
            func(_items[i].object);
    }
    void ForEach(std::function<void(const T&)> func) const {
        for (unsigned i = 0; i < _count; i++)
            func(_items[i].object);
    }
    // includes index
    void ForEach(std::function<void(unsigned, T&)> func) {
        for (unsigned i = 0; i < _count; i++)
            func(i, _items[i].object);
    }
    // string from index 'n' (to be used in conjunction with above)
    void ForEach(unsigned n, std::function<void(T&)> func) {
        for (unsigned i = n; i < _count; i++)
            func(_items[i].object);
    }
};

}