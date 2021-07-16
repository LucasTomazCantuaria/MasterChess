#include "Arduino/ArduinoChessBoard.hpp"
#include "Arduino/ArduinoExported.hpp"
#include "Arduino/SerialPort.hpp"
#include "MasterChess/ChessGame.hpp"
#include "MasterChess/ChessPieces.hpp"
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/Game.hpp"
#include "MasterChess/Boards/ChessBoard.hpp"
#include "MasterChess/Inputs/StockfishInput.hpp"
#include "MasterChess/Listeners/ConsoleChessGameListener.hpp"
#include "Math/Area.hpp"
#include "Unity/UnityGameListener.hpp"
#include "Windows/Console.hpp"

#include <grpcpp/server_builder.h>
#include <fmt/format.h>

namespace MasterChess
{   
    void CreateArmy(vector<unique_ptr<ChessPiece>>& v, ChessPlayer* player, ChessBoard* board, const Vector2Int& zero, const Vector2Int& right, const Vector2Int& up)
    {
        using std::make_unique;
        v.reserve(v.size() + 16);
        for (int i = 0; i < 8; ++i)
            board->AddPiece(v.emplace_back(make_unique<Pawn>(player, up, Ray{ zero + up * 7, right, 8 })).get(), zero + up + right * i);

        vector<IPiece*> rooks
        {
            v.emplace_back(make_unique<Rook>(player)).get(),
            v.emplace_back(make_unique<Rook>(player)).get(),
        };
        
        board->AddPiece(rooks[0], zero);
        board->AddPiece(rooks[1], zero + right * 7);
        board->AddPiece(v.emplace_back(make_unique<Knight>(player)).get(), zero + right);
        board->AddPiece(v.emplace_back(make_unique<Bishop>(player)).get(), zero + right * 2);
        board->AddPiece(v.emplace_back(make_unique<Queen>(player)).get(), zero + right * 3);
        board->AddPiece(v.emplace_back(make_unique<King>(player, move(rooks))).get(), zero + right * 4);
        board->AddPiece(v.emplace_back(make_unique<Bishop>(player)).get(), zero + right * 5);
        board->AddPiece(v.emplace_back(make_unique<Knight>(player)).get(), zero + right * 6);
    }

    vector<unique_ptr<ChessPiece>> CreateArmy(ChessPlayer* player, ChessBoard* board, const Vector2Int& zero, const Vector2Int& right, const Vector2Int& up)
    {
        vector<unique_ptr<ChessPiece>> v;
        CreateArmy(v, player, board, zero, right, up);
        return v;
    }

}

int main(int argc, char* argv[])
{
    using namespace MasterChess;
    using namespace Windows;
    using namespace Arduino;
    GameResult result;
    try
    {
        auto console = Console(220, 220);
        auto consoleBoard = ConsoleChessGameListener(&console);
        auto fish = StockFishInput();

        //auto serial  = SerialPort("COM3", 2000000);
        //auto arduino = ArduinoExported(&serial);
        //auto arduinoBoard = ArduinoChessBoard(&arduino, &consoleBoard);
        //auto req = ArduinoChessBoard::PlayRequester(&*board, &fish);

        auto board = std::make_unique<ChessBoard>();

        auto white = std::make_unique<ChessPlayer>(&consoleBoard, 1, 0xFF0000),
             black = std::make_unique<ChessPlayer>(&consoleBoard, 2, 0x0000FF);

        auto pieces = CreateArmy(white.get(), &*board, { 0, 0 }, Vector2Int::Right, Vector2Int::Up);
        CreateArmy(pieces, black.get(), &*board, { 0, 7 }, Vector2Int::Right, Vector2Int::Down);

        vector<unique_ptr<ChessPlayer>> players;
        players.emplace_back(move(white));
        players.emplace_back(move(black));

        auto game = ChessGame(move(board), move(players), move(pieces));

        auto unityRpc = Unity::UnityGameListener();
        game.AddListener(&unityRpc);

        game.AddListener(&consoleBoard);

        game.AddListener(&fish);

        auto server = grpc::ServerBuilder()
            .AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials())
            .RegisterService(&unityRpc)
            .BuildAndStart();

        result = game.Play();
        server->Shutdown();
    }
    catch (std::exception& e)
    {
        fmt::print("{}\n", e.what());
        return -1;
    }
    fmt::format("Game end!\n{}\n", result.Message);
}
