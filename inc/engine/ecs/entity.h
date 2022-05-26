#pragma once

#include <vector>
#include <iostream>
#include "engine/utility/slot_map.h"
#include "engine/ecs/typeid.h"

#define MAX_ENTITIES 100
#define MAX_COMPONENTS 64


typedef utl::Handle EntityID;
typedef utl::Handle ComponentID;
typedef unsigned ComponentTypeID;

template <typename T> ComponentTypeID id_init();

// Components
// each component maintains a *packed* list of all the active components this this type (T)
template <typename T>
class Component {
    static utl::slot_map<T> s_components;
public:
    EntityID parent;                                                // each component is owned by an entity

    // Basic API
    static ComponentTypeID GetID() {
        static ComponentTypeID s_id = id_init<T>();
        return s_id;
    }
    static ComponentID Create() {
        return s_components.Create();
    }
    static T* Create(ComponentID& id) {
        return s_components.Create(id);
    }
    static void Remove(ComponentID id) {
        s_components.Remove(id);
    }
    static T* Get(ComponentID id) {
        return s_components.Get(id);
    }
    // simple for each loop
    static void ForEach(std::function<void(T&)> func) {
        s_components.ForEach(func);
    }
    // includes index
    static void ForEach(std::function<void(unsigned, T&)> func)  {
        s_components.ForEach(func);
    }
    // quit when func() returns true
	static bool ForEachTillTrue(std::function<bool(T&)> func) {
		return s_components.ForEachTillTrue(func);
	}
};

struct Entity {
    EntityID id;
    std::vector<std::pair<ComponentTypeID, ComponentID>> component_ids;
};


class EntityManager {
    template<typename T> friend unsigned id_init();

    static utl::slot_map<Entity> s_entities;                        // all the entities
    static std::vector<void (*)(ComponentID)> s_remove_components;  // point to functions to delete a component - needed to we do *not* need to know the type T to destory the component
public:
    // Create/Destroy entity
    static EntityID Create();
    static void Destroy(EntityID id);
    static bool Exists(EntityID id);

    // find component in entity
    static bool Find(Entity& entity, ComponentTypeID type_id, ComponentID& handle);
    static bool Find(Entity& entity, ComponentTypeID type_id);

    // Component API
    // Add/Remove/Get components from entity
    template<typename T, typename ... Args>
    static ComponentID Add(EntityID id, Args ... args) {
        auto entity = s_entities.Get(id);
        ComponentID handle;

        if (entity != NULL && Find(*entity, Component<T>::GetID(), handle)) // if the entity already has this component
            // Remove<T>(id);                           // remove it completely
            T::Remove(handle);

        T* c = Component<T>::Create(handle);
        *c = T{id, args...};
        entity->component_ids.push_back(std::make_pair<>(Component<T>::GetID(), handle));
        return handle;
    }
    
    // assumes component-T exists in entity
    template<typename T>
    static void Remove(EntityID id) {
        auto entity = s_entities.Get(id);
        ComponentID handle;

        // find and remove handle from entity
        for (int i = 0; i < entity->component_ids.size(); i++) {
            auto& cid = entity->component_ids[i];
            if (cid.first == Component<T>::GetID()) {
                handle = cid.second;
                entity->component_ids.erase(entity->component_ids.begin() + i);
                break;
            }
        }
        // remove from system
        Component<T>::Remove(handle);
    }

    // assumes component-T exists in entity
    template<typename T>
    static T* Get(EntityID id) {
        ComponentID handle;
        if (s_entities.Get(id) == NULL || !Find(*s_entities.Get(id), Component<T>::GetID(), handle))
            return NULL;
        return Component<T>::Get(handle);
    }
    template<typename T>
    static bool Has(EntityID id) {
        auto entity = s_entities.Get(id);
        if (entity == NULL)
            return false;
        return Find(*entity, Component<T>::GetID());
    }

};

template <typename T>
utl::slot_map<T> Component<T>::s_components(MAX_ENTITIES);

template <typename T>
ComponentTypeID id_init() {
    ComponentTypeID id = (ComponentTypeID)TypeID::Get<T>();
    EntityManager::s_remove_components[id] = &Component<T>::Remove;
    return id;
}

