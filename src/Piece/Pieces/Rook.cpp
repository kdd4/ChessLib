#include "ChessLib/Piece/Pieces/Rook.hpp"

namespace Chess
{
    namespace Pieces
    {
        Rook::Rook(Position pos, PieceColor color, std::weak_ptr<Board>& board)
            :
            MovablePiece(pos, PieceType::Rook, color, board)
        {}

        Rook::Rook(const Piece& piece, std::weak_ptr<Board> board)
            :
            MovablePiece(piece, board)
        {
            type = PieceType::Rook;
        }

        Rook::Rook(const MovablePiece& right)
            :
            MovablePiece(right)
        {
            type = PieceType::Rook;
        }

        std::shared_ptr<MovablePiece> Rook::clone(std::weak_ptr<Board> board) const
        {
            std::shared_ptr<MovablePiece> copy(new Rook(*this, board));
            return copy;
        }

        void Rook::getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack) const
        {
            if (this->deleted) return;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if ((dx != 0 && dy != 0) || (dx == 0 && dy == 0)) continue;

                    moveDirection(dx, dy, vec);
                }
            }
        }

        bool Rook::move(const Position& pos, std::vector<std::shared_ptr<Move>>& vec) const
        {
            if (!pos.check()) return false;

            std::shared_ptr<Piece> attackedPiece = board.lock()->getPiece(pos);

            if (attackedPiece != nullptr)
            {
                if (attackedPiece->color == color)
                    return false;
            }

            std::shared_ptr<ImplMove> move(new ImplMove(board));

            move->appendAttack(pos);
            move->appendStep(this->pos, pos);

            vec.push_back(move);

            return attackedPiece == nullptr;
        }

        void Rook::moveDirection(int dx, int dy, std::vector<std::shared_ptr<Move>>& vec) const
        {
            for (int i = 1; i < 8; ++i)
            {
                Position movePos(pos.x + dx * i, pos.y + dy * i);

                if (!move(movePos, vec)) break;
            }
        }
    }
}