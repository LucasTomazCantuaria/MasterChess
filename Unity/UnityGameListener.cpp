#include "UnityGameListener.hpp"
#include "MasterChess/ChessGame.hpp"
#include "MasterChess/ChessPieces/ChessPiece.hpp"
#include "MasterChess/ChessPieces/King.hpp"
#include "MasterChess/ChessPieces/Pawn.hpp"

#include <fmt/format.h>
#include <google/protobuf/empty.pb.h>

#include <chrono>

using namespace UnityRpc;

namespace Unity
{
    UnityGameListener::UnityGameListener() : game(nullptr), gameOver(nullptr)
    {

    }

    void UnityGameListener::OnGameStart(MasterChess::Game* game)
    {
        assert(dynamic_cast<MasterChess::ChessGame*>(game));
        this->game = static_cast<MasterChess::ChessGame*>(game);
        lastFen = this->game->Fen();
    }

    void UnityGameListener::OnMovementExecution(MasterChess::IMovement* movement)
    {
        PieceMovement msg;
        if (auto mov = dynamic_cast<MasterChess::ChessPiece::Movement*>(movement))
        {
            auto origin = msg.mutable_origin(),
                destination = msg.mutable_destination();
            msg.set_currentplayer(game->CurrentPlayer() == game->Player(0) ? White : Black);
            {
                auto [x, y] = mov->Origin();
                origin->set_x(x); origin->set_y(y);
            }
            {
                auto [x, y] = mov->Destination();
                destination->set_x(x); destination->set_y(y);
            }
            msg.set_fenstring(lastFen);
            msg.set_isenpassant(dynamic_cast<MasterChess::Pawn::EnPassantMovement*>(mov));
            if (auto promo = dynamic_cast<MasterChess::Pawn::PromotionMovement*>(mov))
            {
                switch (promo->PromotedPiece()->Id())
                {
                case 'q': msg.set_promotiontype(PieceMovement_PieceType_Queen); break;
                case 'r': msg.set_promotiontype(PieceMovement_PieceType_Rook); break;
                case 'b': msg.set_promotiontype(PieceMovement_PieceType_Bishop); break;
                case 'n': msg.set_promotiontype(PieceMovement_PieceType_Knight); break;
                }
            }
            msg.set_iscastling(dynamic_cast<MasterChess::King::CastleMovement*>(mov));
            {
                std::lock_guard l(m);
                for (auto stream : streams)
                    stream->Write(msg);
            }
        }
        lastFen = game->Fen();
    }

    void UnityGameListener::OnMovementUndo(MasterChess::IMovement* movement)
    {
        
    }

    void UnityGameListener::OnGameOver(MasterChess::GameResult* result)
    {
        grpc::ClientContext ctx;
        Result request;
        request.set_status(!result->Winners.empty() ? Result_StatusType_CheckMate : Result_StatusType_Draw);
        for (auto winner : result->Winners)
            request.add_winners(winner == game->Player(0) ? White : Black);
        for (auto loser : result->Losers)
            request.add_losers(loser == game->Player(0) ? White : Black);
        google::protobuf::Empty response;
        //client->EndGame(&ctx, request, &response);
        gameOver = true;
    }

    grpc::Status UnityGameListener::Start(grpc::ServerContext* context, const google::protobuf::Empty* request, grpc::ServerWriter<PieceMovement>* writer)
    {
        using namespace std::chrono_literals;
        {
            std::lock_guard l(m);
            streams.emplace_back(writer);
            PieceMovement msg;
            msg.set_fenstring(game->Fen());
            writer->Write(msg);
        }

        while (!context->IsCancelled() && !gameOver)
            std::this_thread::sleep_for(1ms);

        {
            std::lock_guard l(m);
            auto it = find(streams.begin(), streams.end(), writer);
            streams.erase(it);
        }

        return grpc::Status::OK;
    }
}
