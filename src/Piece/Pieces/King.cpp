#include "ChessLib/Piece/Pieces/King.hpp"

namespace Chess
{
    namespace Pieces
    {
        King::King(Position pos, PieceColor color, std::weak_ptr<Board>& board)
            :
            MovablePiece(pos, PieceType::King, color, board)
        {}

        King::King(const Piece& piece, std::weak_ptr<Board> board)
            :
            MovablePiece(piece, board)
        {
            type = PieceType::King;
        }

        King::King(const MovablePiece& right)
            :
            MovablePiece(right)
        {
            type = PieceType::King;
        }

        std::shared_ptr<MovablePiece> King::clone(std::weak_ptr<Board> board) const
        {
            std::shared_ptr<MovablePiece> copy(new King(*this, board));
            return copy;
        }

        void King::getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack) const
        {
            if (this->deleted) return;

            std::shared_ptr<ImplMove> move;
            std::shared_ptr<Piece> attacked_place;
            Position move_step;

            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    move_step = {pos.x + dx, pos.y + dy };

                    if ((dx == 0 && dy == 0) || !move_step.check()) continue;

                    attacked_place = board.lock()->getPiece(move_step);
                    if (attacked_place != nullptr)
                    {
                        if (attacked_place->color == color)
                            continue;
                    }
                    
                    move = std::shared_ptr<ImplMove>(new ImplMove(board));

                    move->appendStep(pos, move_step);
                    move->appendAttack(move_step);

                    vec.push_back(move);
                }
            }

            if (this->getMoveCount() != 0) return;

            if (!onlyAttack)
            {
                QueensideCastling(vec);
                KingsideCastling(vec);
            }
        }

        void King::QueensideCastling(std::vector<std::shared_ptr<Move>>& vec) const
        {
            if (this->getMoveCount() != 0) return;

            std::shared_ptr<Piece> figureRook = board.lock()->getPiece(Position(0, pos.y));
            if (figureRook == nullptr) return;
            if (figureRook->type != PieceType::Rook ||\
                figureRook->color != color ||\
                figureRook->getMoveCount() != 0) return;

            for (int x = pos.x - 1; x > 0; --x)
            {
                if (board.lock()->getPiece(Position(x, pos.y)) != nullptr)
                    return;
            }
            
            std::vector<Position> checkingPosition = { Position(pos.x - 1, pos.y), Position(pos.x - 2, pos.y) };
            PieceColor enemyColor = (color == PieceColor::White) ? PieceColor::Black : PieceColor::White;

            std::vector<std::shared_ptr<Move>> enemyAttackMoves;
            board.lock()->getMoves(enemyAttackMoves, enemyColor, true);

            for (std::shared_ptr<Move>& enemyMove : enemyAttackMoves)
            {
                for (const Position& attacked_pos : enemyMove->getAttackedPositions())
                {
                    for (const Position& checkPos : checkingPosition)
                    {
                        if (checkPos == attacked_pos)
                            return;
                    }
                }
            }

            std::shared_ptr<ImplMove> move(new ImplMove(board));

            move->appendStep(pos, Position(pos.x - 2, pos.y));
            move->appendStep(figureRook->pos, Position(pos.x - 1, pos.y));

            vec.push_back(move);
        }

        void King::KingsideCastling(std::vector<std::shared_ptr<Move>>& vec) const
        {
            if (this->getMoveCount() != 0) return;

            std::shared_ptr<Piece> figureRook = board.lock()->getPiece(Position(7, pos.y));
            if(figureRook == nullptr) return;
            if (figureRook->type != PieceType::Rook || \

                figureRook->color != color || \
                figureRook->getMoveCount() != 0) return;

            for (int x = pos.x + 1; x < 7; ++x)
            {
                if (board.lock()->getPiece(Position(x, pos.y)) != nullptr)
                    return;
            }

            std::vector<Position> checkingPosition = { Position(pos.x + 1, pos.y), Position(pos.x + 2, pos.y) };
            PieceColor enemyColor = (color == PieceColor::White) ? PieceColor::Black : PieceColor::White;

            std::vector<std::shared_ptr<Move>> enemyAttackMoves;
            board.lock()->getMoves(enemyAttackMoves, enemyColor, true);

            for (std::shared_ptr<Move>& enemyMove : enemyAttackMoves)
            {
                for (const Position& attacked_pos : enemyMove->getAttackedPositions())
                {
                    for (const Position& checkPos : checkingPosition)
                    {
                        if (checkPos == attacked_pos)
                            return;
                    }
                }
            }

            std::shared_ptr<ImplMove> move(new ImplMove(board));

            move->appendStep(pos, Position(pos.x + 2, pos.y));
            move->appendStep(figureRook->pos, Position(pos.x + 1, pos.y));

            vec.push_back(move);
        }
    }
}