#ifndef GAME_H
#define GAME_H
#include <vector>
#include <map>


// The list of players
enum Player {
    WHITE = 0,
    BLACK = 1
};


// We need to pre-declare these to allow for cross-dependencies
class Piece;
class AbstractPieceFactory;

// A struct to store the position
// [DO NOT MODIFY]
struct Position {
    unsigned int x, y;
    Position(unsigned int xx = 0 , unsigned int yy = 0) : x(xx) , y(yy) { }
};

// The board for the game
class Board {
    // The type of a piece factory map. Maps from int describing a
    // Piece to the factory class producing the Piece.
    typedef std::map<int, AbstractPieceFactory*> PieceGenMap;
public:
    // Construct a board with the specified dimensions
    Board(unsigned int w, unsigned int h, int turn = 1) :
        m_width(w), m_height(h), m_turn(turn), m_pieces(w * h, nullptr) {}

    // Virtual destructor is necessary for a class with virtual methods
    virtual ~Board();

    // Returns the width of the board
    // [DO NOT MODIFY]
    unsigned int width() const { return m_width; }

    // Returns the height of the board
    // [DO NOT MODIFY]
    unsigned int height() const { return m_height; }

    // Create a piece on the board using the factory.
    // Returns true if the piece was successfully placed on the board
    // [DO NOT MODIFY]
    bool initPiece(int id, Player owner, Position p);
 
    // Returns a pointer to the piece at the specified position,
    // if the position is valid and occupied, nullptr otherwise.
    // [DO NOT MODIFY]
    Piece* getPiece(Position p) const;

    // Returns the player whose turn it is
    // [DO NOT MODIFY]
    Player playerTurn() const { return static_cast<Player>(!(m_turn % 2)); }

    // Return the current turn
    // [DO NOT MODIFY]
    int turn() const { return m_turn; }

    // Returns true if the position is within bounds
    // [DO NOT MODIFY]
    bool validPosition(Position p) const {
        return p.x < m_width && p.y < m_height;
    }

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    // [Do not modify the type of this method]
    virtual int makeMove(Position start, Position end);
        // This method may handle the parts of moving pieces that
        // generalize over different board games
       
    

    void printBoard();
    // The main gameplay loop. Ideally, you should be able to implement
    // all of the gameplay loop logic here in the Board class rather than
    // overriding this method in the specialized Game-specific class
    virtual void run() { printBoard();   }


    // Returns "true" if the game is over
    virtual bool gameOver() const = 0 ;

protected:
    // All the factories registered with this Board
    PieceGenMap m_registeredFactories;

    unsigned int m_width , m_height;

    // Current game turn
    int m_turn;

    // Vector containing all the Pieces of the board
    std::vector<Piece*> m_pieces;

    // Get the 1d index from the 2d position
    // [DO NOT MODIFY]
    unsigned int index(Position position) const {
        return position.y * m_width + position.x;
    }
    
    // Functionality for creating a new piece (called by initPiece)
    // [DO NOT MODIFY]
    Piece* newPiece(int id, Player owner);
   
    // Functionality for adding piece factories (called by constructor)
    // [DO NOT MODIFY]
    bool addFactory(AbstractPieceFactory*);
};

// A (virtual) class responsible for creating new instances of a
// particular type of piece (factory pattern)
// [DO NOT MODIFY]
class AbstractPieceFactory {
public:
    // Create a piece with the specified owner
    virtual Piece* newPiece(Player owner) const = 0;
    virtual ~AbstractPieceFactory() {}
};

// A templated class generating Pieces
// [DO NOT MODIFY]
template <class T>
class PieceFactory : public AbstractPieceFactory {
public:
    PieceFactory(int id) : m_id(id) {}
    Piece* newPiece(Player owner) const override {
        return new T(owner, m_id);
    }
protected:
    int m_id;
};

// An individual piece
class Piece {
public:
    // No public constructor: only use factories to build this
    // class's subclasses
    
    virtual ~Piece() {}

    // Returns the owner of the piece
    // [DO NOT MODIFY]
    Player owner() const { return m_owner; }

    // Returns the id of the piece
    // [DO NOT MODIFY]
    int id() const { return m_id; }

    // Returns an integer representing move validity
    // >= 0 = valid, < 0 = invalid
    // [Do not modify the type of this method]
    virtual int validMove(Position start, Position end, const Board& board) const {
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

protected:
    Player m_owner;
    int m_id;

    // Constructs a piece with a specified owner
    // Note that this is deliberately made protected. Use the factory only!
    // [DO NOT MODIFY]
    Piece(Player owner, int id) : m_owner(owner) , m_id(id) {}
};

#endif // GAME_H
