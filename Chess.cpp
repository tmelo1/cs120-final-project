#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>

int Piece::validMove(Position start, Position end, const Board& board) const {
        // This particular method may include generic logic to check
        // for a valid move. 
    if (!board.validPosition(start) || !board.validPosition(end)) {
        return -1;
    }
    if (start.x == end.x && start.y == end.y) {
        return -2;
    }
    if (!board.getPiece(start)) {
        return -3;
    }
    if (board.getPiece(start)->owner() != board.turn()) {
        return -4;
    }
    if (board.getPiece(end)->owner() == board.turn()) {
        return -5;
    }

    return 1;
}

// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end) {
    // Possibly implement chess-specific move logic here
    //
    // We call Board's makeMove to handle any general move logic
    // Feel free to use this or change it as you see fit
    int retCode = Board::makeMove(start, end);
    if (retCode < 0) {
        return -1;
    }
    
    nextTurn();
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

int ChessGame::isValidCommand(std::string command) {
    switch (command[0]) {
        case 'q':
            exit(1);
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
            break;
        default:
            return 0;
    }

    switch (command[1] - '0') {
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
            return 1;
        default:
            return 0;
    }
}

int ChessGame::parseCommand(std::string command) {
    command = stringToLower(command);
    std::stringstream ss(command);
    std::string item;
    unsigned int x;
    unsigned int y;
    std::vector<std::string> tokens;
    while (ss >> item)
        tokens.push_back(item);

    if (tokens[0].compare("board")) {
        toggleDisplay();
    }

    if (tokens[0].compare("save")) {
        Prompts::saveGame();
        std::cin >> item;
        saveGame(item);
    }

    if (!isValidCommand(tokens[0]) || !isValidCommand(tokens[1])) {
        Prompts::parseError();
        return 0;
    }

    return 1;
}

std::string ChessGame::stringToLower(std::string input) {
    for (unsigned int i = 0; i < input.length(); i++) {
        if (isalpha(input[i])) {
            input[i] = tolower(input[i]);
        }
    }
    return input;
}

int ChessGame::saveGame(std::string fileName) {
    std::ofstream saveFile(fileName);
    if (saveFile.fail()) {
        return -1;
    }
    saveFile << "chess" << '\n';
    saveFile << turn() << '\n';
    for (unsigned int i = 0; i < m_width; i++) {
        for (unsigned int j = 0; m_height; j++) {
            Position current = Position(i, j);
            Piece *p = getPiece(current);
            if (!p) {
                continue;
            } else {
                saveFile << p->owner() << " " << convertPosition(i, j) << " " << p->id() << '\n';
            }
        }
    }
    saveFile.close();
    return 1;

}

std::string ChessGame::convertPosition(unsigned int row, unsigned int col) {
    std::string rows = std::to_string(row + 1);
    std::string pos("");
    switch(col) {
        case 0:
            pos = pos + "a";
            break;
        case 1:
            pos = pos + "b";
            break;
        case 2:
            pos = pos + "c";
            break;
        case 3:
            pos = pos + "d";
            break;
        case 4:
            pos = pos + "e";
            break;
        case 5:
            pos = pos + "f";
            break;
        case 6:
            pos = pos + "g";
            break;
        case 7:
            pos = pos + "h";
            break;            
    }

    pos = pos + rows;
    return pos;
}

int main() {
    ChessGame chess;
    chess.setupBoard();
    chess.run();
}
