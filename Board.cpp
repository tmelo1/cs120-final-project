#include <assert.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include "Game.h"
#include "Prompts.h"

/**
Tony Melo - tmelo1@jhu.edu (tmelo1)
EN600.120
Assignment - FInal Project
5/5/2017
 */
///////////////
// Board //
///////////////

Board::~Board() {
    // Delete all pointer-based resources
    for (unsigned int i=0; i < m_width * m_height; i++)
        delete m_pieces[i];
    for (size_t i=0; i < m_registeredFactories.size(); i++)
        delete m_registeredFactories[i];
}

// Get the Piece at a specific Position, or nullptr if there is no
// Piece there or if out of bounds.
Piece* Board::getPiece(Position position) const {
    if (validPosition(position))
        return m_pieces[index(position)];
    else {
        Prompts::outOfBounds();
        return nullptr;
    }
}

// Create a piece on the board using the factory.
// Returns true if the piece was successfully placed on the board
bool Board::initPiece(int id, Player owner, Position position) {
    Piece* piece = newPiece(id, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!validPosition(position)) {
        Prompts::outOfBounds();
        return false;
    }
    // Fail if the position is occupied
    if (getPiece(position)) {
        Prompts::blocked();
        return false;
    }
    m_pieces[index(position)] = piece;
    return true;
}

// Add a factory to the Board to enable producing
// a certain type of piece
bool Board::addFactory(AbstractPieceFactory* pGen) {
    // Temporary piece to get the ID
    Piece* p = pGen->newPiece(WHITE);
    int id = p->id();
    delete p;

    PieceGenMap::iterator it = m_registeredFactories.find(id);
    if (it == m_registeredFactories.end()) { // not found
        m_registeredFactories[id] = pGen;
        return true;
    } else {
        std::cout << "Id " << id << " already has a generator\n";
        return false;
    }
}

// Search the factories to find a factory that can translate `id' to
// a Piece, and use it to create the Piece. Returns nullptr if not found.
Piece* Board::newPiece(int id, Player owner) {
    PieceGenMap::iterator it = m_registeredFactories.find(id);
    if (it == m_registeredFactories.end()) { // not found
        std::cout << "Id " << id << " has no generator\n";
        return nullptr;
    } else {
        return it->second->newPiece(owner);
    }
}


void Board::printBoard() {

    for (unsigned int i = 0; i < m_width; i++) {
        for (unsigned int j = 0; j < m_height; j++) {
            Position curr = Position(i, j);
            Piece *p = getPiece(curr);
            if (p) {
                std::cout << p->id() << " ";
            } else {
                std::cout << "+ ";
            }
        }
        std::cout << std::endl;
    }
}




int Board::makeMove(Position start, Position end) {

    Piece *p = getPiece(start);
    if (p->validMove(start, end, *this) < 0) {
        return -1;
    }
    int id = p->id();
    Player pt = playerTurn();
    if (!initPiece(id, pt, end)) {
        return -1;
    }
    delete(p);
    return 1;

}
