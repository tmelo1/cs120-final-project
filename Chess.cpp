#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"

// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end) {
    // Possibly implement chess-specific move logic here
    //
    // We call Board's makeMove to handle any general move logic
    // Feel free to use this or change it as you see fit
    int retCode = Board::makeMove(start, end);

    
    return retCode;
}

// Setup the chess board with its initial pieces
void ChessGame::setupBoard() {
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        initPiece(PAWN_ENUM, WHITE, Position(i, 1));
        initPiece(pieces[i], WHITE, Position(i, 0));
        initPiece(pieces[i], BLACK, Position(i, 7));
        initPiece(PAWN_ENUM, BLACK, Position(i, 6));
    }
}


int Pawn::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case -1:
            Prompts::outOfBounds();
            return -1;
        case -2:
            Prompts::illegalMove();
            return -1;
        case -3:
            Prompts::noPiece();
            return -1;
        case -4:
            Prompts::noPiece();
            return -1;
        case -5:
            Prompts::blocked();
            return -1;
        case 1:
            break;
    }

    if (start.x <= end.x) {
        return -1;
    }

    return 1;
}

int Rook::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case 1:
            break;
        default:
            return -1;
    }

    int xDiff = end.x - start.y;
    int yDiff = end.y - start.y;
    if (xDiff != 0) {
        if (yDiff != 0) {
            Prompts::illegalMove();
            return -1;
        }
    }

    /**
    if (xDiff == 0) {
        if (yDiff > 0) {
            for (unsigned int j = start.y; j <= end.y; j++) {
                Position check = Position(start.x, j);
                Piece *p = board.getPiece(check);
                if (p) {
                    Prompts::blocked();
                    return -1;
                }
            }
         }
    }
    */
    return 1;
}

int Knight::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case -1:
            Prompts::outOfBounds();
            return -1;
        case -2:
            Prompts::illegalMove();
            return -1;
        case -3:
            Prompts::noPiece();
            return -1;
        case -4:
            Prompts::noPiece();
            return -1;
        case -5:
            Prompts::blocked();
            return -1;
        case 1:
            break;
    }

    return 1;
}

int Bishop::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case -1:
            Prompts::outOfBounds();
            return -1;
        case -2:
            Prompts::illegalMove();
            return -1;
        case -3:
            Prompts::noPiece();
            return -1;
        case -4:
            Prompts::noPiece();
            return -1;
        case -5:
            Prompts::blocked();
            return -1;
        case 1:
            break;
    }

    return 1;
}


int Queen::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case -1:
            Prompts::outOfBounds();
            return -1;
        case -2:
            Prompts::illegalMove();
            return -1;
        case -3:
            Prompts::noPiece();
            return -1;
        case -4:
            Prompts::noPiece();
            return -1;
        case -5:
            Prompts::blocked();
            return -1;
        case 1:
            break;
    }

    return 1;
}

int King::validMove(Position start, Position end, const Board& board) const {
    switch (Piece::validMove(start, end, board)) {
        case -1:
            Prompts::outOfBounds();
            return -1;
        case -2:
            Prompts::illegalMove();
            return -1;
        case -3:
            Prompts::noPiece();
            return -1;
        case -4:
            Prompts::noPiece();
            return -1;
        case -5:
            Prompts::blocked();
            return -1;
        case 1:
            break;
    }

    return 1;
}





int main() {
    ChessGame chess;
    chess.setupBoard();
    chess.run();
}
