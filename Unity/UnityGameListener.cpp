#include "UnityGameListener.hpp"
#include "MasterChess/ChessPieces/ChessPiece.hpp"

#include <fmt/format.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/empty.pb.h>

#include "MasterChess/ChessPieces/King.hpp"
#include "MasterChess/ChessPieces/Pawn.hpp"

using namespace UnityRpc;

namespace Unity
{

    UnityGameListener::UnityGameListener(std::string_view ip, int port) :
        client([=]
        {
            auto channel = CreateChannel(fmt::format("{}:{}", ip, port), grpc::InsecureChannelCredentials());
            grpc::ClientContext ctx;
            return UnityRPCBoard::NewStub(channel);
        }())
    {

    }

    void UnityGameListener::OnGameStart(MasterChess::Game* game)
    {
        grpc::ClientContext ctx;
        google::protobuf::Empty empty;
        client->Reset(&ctx, empty, &empty);
    }

    void UnityGameListener::OnMovementExecution(MasterChess::IMovement* movement)
    {
        PieceMovement request;
        auto origin = request.mutable_startpos(),
            destination = request.mutable_endpos();
        if (auto m = dynamic_cast<MasterChess::ChessPiece::Movement*>(movement))
        {
            {
                auto [x, y] = m->Origin();
                origin->set_x(x); origin->set_y(y);
            }
            {
                auto [x, y] = m->Destination();
                destination->set_x(x); destination->set_y(y);
            }
            grpc::ClientContext ctx;
            google::protobuf::Empty response;
            bool result;
            if (dynamic_cast<MasterChess::Pawn::EnPassantMovement*>(m))
                result = client->EnPassant(&ctx, request, &response).ok();
            else if (dynamic_cast<MasterChess::King::CastleMovement*>(m))
                result = client->Castle(&ctx, request, &response).ok();
            else result = client->Move(&ctx, request, &response).ok();
            assert(result);
        }
    }

}
