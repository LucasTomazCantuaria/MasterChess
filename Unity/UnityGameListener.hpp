#pragma once
#include "MasterChess/IGameListener.hpp"
#include "generated/UnityRpc.grpc.pb.h"

#include <string>
#include <mutex>

namespace MasterChess
{
    struct ChessGame;
}

namespace Unity
{
    struct UnityGameListener final : UnityRpc::UnityRPCBoard::Service, MasterChess::IGameListener
    {
        UnityGameListener();
        void OnGameStart(MasterChess::Game* game) override;
        void OnMovementExecution(MasterChess::IMovement* movement) override;
        void OnMovementUndo(MasterChess::IMovement* movement) override;
        void OnGameOver(MasterChess::GameResult* result) override;
        grpc::Status Start(grpc::ServerContext* context, const google::protobuf::Empty* request, grpc::ServerWriter<UnityRpc::PieceMovement>* writer) override;
    private:
        MasterChess::ChessGame* game;
        std::string lastFen;
        std::vector<grpc::ServerWriter<UnityRpc::PieceMovement>*> streams;
        std::mutex m;
        bool gameOver;
    };
}
