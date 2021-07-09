#include "UnityGameListener.hpp"
#include "MasterChess/ChessPieces/ChessPiece.hpp"

#include <fmt/format.h>
#include <grpcpp/create_channel.h>
#include <google/protobuf/empty.pb.h>

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
        std::thread([=]
        {
            PieceMovement mov;
            auto origin = mov.mutable_startpos(),
                destination = mov.mutable_endpos();
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
                assert(client->Move(&ctx, mov, &response).ok());
            }
        }).detach();
    }

}
