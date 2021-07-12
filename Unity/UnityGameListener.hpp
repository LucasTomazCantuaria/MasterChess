#pragma once
#include "MasterChess/IGameListener.hpp"
#include "generated/UnityRpc.grpc.pb.h"

#include <string_view>

namespace Unity
{
    struct UnityGameListener final : MasterChess::IGameListener
    {
        explicit UnityGameListener(std::string_view ip, int port);
        void OnGameStart(MasterChess::Game* game) override;
        void OnMovementExecution(MasterChess::IMovement* movement) override;
    private:
        std::unique_ptr<UnityRpc::UnityRPCBoard::Stub> client;
    };
}
