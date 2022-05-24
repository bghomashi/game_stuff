#pragma once

#include "engine/ecs/entity.h"
#include "engine/network/network.h"

struct NetworkComponent : public Component<NetworkComponent> {
    Net::ClientID client_id;
};

