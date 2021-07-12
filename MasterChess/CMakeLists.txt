cmake_minimum_required (VERSION 3.0)

add_library (MasterChess STATIC
    "IConsole.hpp"
    "IBoard.hpp" "IBoard.cpp"
    "IMovement.hpp"
    "IPlayer.hpp"
    "IPiece.hpp"
    "IInput.hpp"
    "IGameListener.hpp"
    
    "ChessPlayer.hpp"
    "Game.hpp" "Game.cpp"
    "ChessGame.hpp" "ChessGame.cpp"

    "ChessPieces/King.hpp" "ChessPieces/King.cpp"
    "ChessPieces/Queen.hpp" "ChessPieces/Queen.cpp"
    "ChessPieces/Rook.hpp" "ChessPieces/Rook.cpp"
    "ChessPieces/Bishop.hpp" "ChessPieces/Bishop.cpp"
    "ChessPieces/Knight.hpp" "ChessPieces/Knight.cpp"
    "ChessPieces/Pawn.hpp" "ChessPieces/Pawn.cpp"
    "ChessPieces/ChessPiece.hpp" "ChessPieces/ChessPiece.cpp"
    "ChessPieces/CannonChessPiece.hpp" "ChessPieces/CannonChessPiece.cpp"
    "ChessPieces/MultiPositionChessPiece.hpp" "ChessPieces/MultiPositionChessPiece.cpp"
    "ChessPieces.hpp"

    "Boards/ChessBoard.hpp" "Boards/ChessBoard.cpp"
    "Listeners/ConsoleChessGameListener.hpp" "Listeners/ConsoleChessGameListener.cpp"

    "Inputs/StockfishInput.hpp" "Inputs/StockfishInput.cpp"
    "Inputs/TwoStepsInput.hpp" "Inputs/TwoStepsInput.cpp"
)

target_link_libraries(MasterChess PUBLIC fmt::fmt)
