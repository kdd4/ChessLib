#pragma once

#include "ChessLib/Piece/MovablePiece.hpp"
#include "ChessLib/Board.hpp"

#include "ChessLib/Piece/Enums.hpp"
#include "ChessLib/Position.hpp"
#include "ChessLib/ImplMove.hpp"

#include <vector>
#include <memory>

namespace Chess
{
    namespace Pieces
    {
        class Queen : public IMovablePiece
        {
        public:
            Queen(Position pos, PieceColor color, std::weak_ptr<Board>& board);
            Queen(const Piece& piece, std::weak_ptr<Board> board);
            Queen(const MovablePiece& right);

            std::shared_ptr<MovablePiece> clone(std::weak_ptr<Board> board) const override final;
            void getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack = false) const override final;

        private:
            bool move(const Position& pos, std::vector<std::shared_ptr<Move>>& vec) const;

            void moveDirection(int dx, int dy, std::vector<std::shared_ptr<Move>>& vec) const;
        };
    }
}
