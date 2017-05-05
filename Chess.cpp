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

void ChessGame::run() {
    std::string in;
    std::string file;

    goagain:
    Prompts::menu();
    std::cin >> in;
    if (in.compare("1") == 0) {
        setupBoard();
        Prompts::playerPrompt(playerTurn(), turn());
    } else if (in.compare("2") == 0) {
        Prompts::loadGame();
        std::cin >> file;
        readSaveFile(file);
    } else {
        goto goagain;
    }

    while (true) {
        std::cin >> in;
        parseCommand(in);
    }

}

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
        Prompts::illegalMove();
        return -1;
    }

    return 1;
}

int Rook::validMove(Position start, Position end, const Board& board) const {
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

    int xDiff = end.x - start.y;
    int yDiff = end.y - start.y;

    if (xDiff != 0 && yDiff != 0) {
        Prompts::illegalMove();
        return -1;
    }
    
    if (board.blockedMove(start, end)) {
        Prompts::blocked();
        return -1;
    }
    
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

    int xDiff = end.x - start.x;
    int yDiff = end.y - start.x;
    if (xDiff == 0 || yDiff == 0) {
        Prompts::illegalMove();
        return -1;
    }

    return 1;
}

bool Board::blockedMove(Position start, Position end) const {
    int xDiff = end.x - start.x;
    int yDiff = end.y - start.y;
    if (xDiff == 0) {
        if (yDiff > 0) {
            for (unsigned int j = start.y; j < end.y; j++) {
                Position check = Position(start.x, j);
                Piece *p = getPiece(check);
                if (p) {
                    Prompts::blocked();
                    return -1;
                }
            }
        } else if (yDiff < 0) {
            for (unsigned int j = end.y; j > start.y; j--) {
                Position check = Position(start.x, j);
                Piece *p = getPiece(check);
                if (p) {
                    Prompts::blocked();
                    return -1;
                }
            }
        }
    } else if (yDiff == 0) {
        if (xDiff > 0) {
            for (unsigned int i = start.x; i < end.x; i++) {
                Position check = Position(i, start.y);
                Piece *p = getPiece(check);
                if (p) {
                    Prompts::blocked();
                    return -1;
                }
            }
        } else if (xDiff < 0) {
            for (unsigned int i = end.x; i > start.x; i--) {
                Position check = Position(i, start.y);
                Piece *p = getPiece(check);
                if (p) {
                    Prompts::blocked();
                    return -1;
                }
            }
        }
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

int ChessGame::isValidCommand(std::string command) const {
    switch (command[0]) {
        case 'q':
            this->~ChessGame();
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
    unsigned int s_x;
    unsigned int s_y;
    unsigned int e_x;
    unsigned int e_y;
    Position s;
    Position e;
    std::vector<std::string> tokens;
    while (ss >> item)
        tokens.push_back(item);

    if (tokens[0].compare("board")) {
        toggleDisplay();
        return 1;
    }

    if (tokens[0].compare("save")) {
        Prompts::saveGame();
        std::cin >> item;
        int status = saveGame(item);
        if (status < 0) {
            return -1;
        } else {
            nextTurn();
            return 1;
        }
    }

    if (tokens[0].compare("forfeit")) {
        forfeit();
    }

    if (!isValidCommand(tokens[0]) || !isValidCommand(tokens[1])) {
        Prompts::parseError();
        return -1;
    } else {
        switch(tokens[0][0]) {
            case 'a':
                s_x = A_ENUM;
                break;
            case 'b':
                s_x = B_ENUM;
                break;
            case 'c':
                s_x = C_ENUM;
                break;
            case 'd':
                s_x = D_ENUM;
                break;
            case 'e':
                s_x = E_ENUM;
                break;
            case 'f':
                s_x = F_ENUM;
                break;
            case 'g':
                s_x = G_ENUM;
                break;
            case 'h':
                s_x = H_ENUM;
                break;
        }
        s_y = tokens[0][1] - '0' - 1;
        s = Position(s_x, s_y);

        switch(tokens[1][0]) {
            case 'a':
                e_x = A_ENUM;
                break;
            case 'b':
                e_x = B_ENUM;
                break;
            case 'c':
                e_x = C_ENUM;
                break;
            case 'd':
                e_x = D_ENUM;
                break;
            case 'e':
                e_x = E_ENUM;
                break;
            case 'f':
                e_x = F_ENUM;
                break;
            case 'g':
                e_x = G_ENUM;
                break;
            case 'h':
                e_x = H_ENUM;
                break;
        }

        e_y = tokens[1][1] - '0' - 1;
        e = Position(e_x, e_y);

    }



    return 1;
}

std::string ChessGame::stringToLower(std::string input) const {
    for (unsigned int i = 0; i < input.length(); i++) {
        if (isalpha(input[i])) {
            input[i] = tolower(input[i]);
        }
    }
    return input;
}

int ChessGame::saveGame(std::string fileName) const {
    std::ofstream saveFile(fileName);
    if (saveFile.fail()) {
        Prompts::saveFailure();
        return -1;
    }
    saveFile << "chess" << '\n';
    saveFile << turn() << '\n';
    for (unsigned int i = 0; i < m_width; i++) {
        for (unsigned int j = 0; j < m_height; j++) {
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

void ChessGame::readSaveFile(std::string fileName) const {
    std::cout << fileName << std::endl;
    return;
}

std::string ChessGame::convertPosition(unsigned int row, unsigned int col) const {
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

void ChessGame::forfeit() const {
    Player current = playerTurn();
    int t = turn();
    Prompts::win(current, t);
    endGame();

}

void ChessGame::endGame() const {
    Prompts::gameOver();
    this->~ChessGame();
    exit(1);
}


int main() {
    ChessGame chess;
    chess.run();
}
