// spiders.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Pit::display going.  That gives you
// more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <type_traits>
#include <cassert>
#define CHECKTYPE(c, f, r, a)  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
       "FAILED: You changed the type of " #c "::" #f);  \
    [[gnu::unused]] auto xxx##c##_##f = static_cast<r(c::*)a>(&c::f)

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;      // max number of rows in the pit
const int MAXCOLS = 40;      // max number of columns in the pit
const int MAXSPIDERS = 170;  // max number of spiders allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Pit;  // This is needed to let the compiler know that Pit is a
            // type name, since it's mentioned in the Spider declaration.

class Spider
{
public:
    // Constructor
    Spider(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;

    // Mutators
    void move();

private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};

class Player
{
public:
    // Constructor
    Player(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

    // Mutators
    void   stand();
    void   move(int dir);
    void   setDead();

private:
    Pit* m_pit;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     spiderCount() const;
    int     numberOfSpidersAt(int r, int c) const;
    void    display() const;

    // Mutators
    bool   addSpider(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSpider(int r, int c);
    bool   moveSpiders();

private:
    int      m_rows;
    int      m_cols;
    Player* m_player;
    Spider* m_spiders[MAXSPIDERS];
    int      m_nSpiders;

    // Helper functions
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nSpiders);
    ~Game();

    // Mutators
    void play();

private:
    Pit* m_pit;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Spider implementation
///////////////////////////////////////////////////////////////////////////

Spider::Spider(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** A spider must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "***** Spider created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
}

int Spider::row() const
{
    return m_row;
}

int Spider::col() const
{
    // DONE: TRIVIAL:  Return the column the spider is at.
    return m_col;  // this is wrong -- replace it
}

void Spider::move()
{
    // Attempt to move in a random direction; if it can't move, don't move
    switch (randInt(0, NUMDIRS - 1))
    {
    case UP:
        if (row() != 1) //Move spider up except when on the first row
            m_row--;  // DONE:  Move the spider up one row if possible.
        break;
    case DOWN:
        if (row() != m_pit->rows()) //Move spider down except when at the last row
            m_row++;
        break;
    case LEFT:
        if (col() != 1) //Move spider left except when at the first column
            m_col--;
        break;
    case RIGHT:
        if (col() != m_pit->cols()) //move spider right except when at the last column
            m_col++;
        break;  // DONE:  Implement the other movements.
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** The player must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    // DONE: TRIVIAL:  Return the row the player is at.
    return m_row;  // this is wrong -- replace it
}

int Player::col() const
{
    // DONE: TRIVIAL:  Return the column the player is at.
    return m_col;  // this is wrong -- replace it
}

int Player::age() const
{
    // DONE: TRIVIAL:  Return the player's age.
    return m_age;  // this is wrong -- replace it
}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;

    switch (dir)
    {
    case UP:
        if (!(row() == 1))   //when player is not at first row
        {
            if (m_pit->numberOfSpidersAt(row() - 1, col()) == 0)  //if grid pos one row above is empty, move up
            {
                m_row--;
            }
            else if (m_pit->numberOfSpidersAt(1, col()) > 0 && row() == 2) //if spider at row 1 and player at row 2
            {
                m_pit->destroyOneSpider(1, col()); 
                m_row--;
            }
            else if (m_pit->numberOfSpidersAt(row() - 1, col()) > 0 &&
                m_pit->numberOfSpidersAt(row() - 2, col()) == 0) //if number of spiders one row above is more than zero, and two rows above is vacant.
            {
                m_pit->destroyOneSpider(row() - 1, col());
                m_row -= 2;
            }
            else if (m_pit->numberOfSpidersAt(row() - 1, col()) > 0 &&
                m_pit->numberOfSpidersAt(row() - 2, col()) > 0) //if number of spiders one row above is more than zero, and two rows above also has spider(s).
            {
                m_pit->destroyOneSpider(row() - 1, col());
                setDead();
                m_row -= 2;
            }
        }
        break;
    case DOWN:
        if (!(row() == m_pit->rows())) 
        {
            if (m_pit->numberOfSpidersAt(row() + 1, col()) == 0)

            {
                m_row++;
            }
            else if (m_pit->numberOfSpidersAt(m_pit->rows(), col()) > 0 && row() == m_pit->rows() - 1)

            {
                m_pit->destroyOneSpider(m_pit-> rows(), col()); 
                m_row++;
            }
            else if (m_pit->numberOfSpidersAt(row() + 1, col()) > 0 &&
                m_pit->numberOfSpidersAt(row() + 2, col()) == 0)

            {
                m_pit->destroyOneSpider(row() + 1, col());
                m_row += 2;
            }
            else if (m_pit->numberOfSpidersAt(row() + 1, col()) > 0 &&
                m_pit->numberOfSpidersAt(row() + 2, col()) > 0) 
            {
                m_pit->destroyOneSpider(row() + 1, col());
                setDead();
                m_row += 2;
            }
        }

        break;
    case LEFT:          
        if (!(col() == 1))
        {
            if (m_pit->numberOfSpidersAt(row(), col() - 1) == 0)

            {
                m_col--;
            }
            else if (m_pit->numberOfSpidersAt(row(), 1) > 0 && col() == 2)
            {
                m_pit->destroyOneSpider(row(), 1);
                m_col--;
            }
            else if (m_pit->numberOfSpidersAt(row(), col() - 1) > 0 &&
                m_pit->numberOfSpidersAt(row(), col() - 2) == 0)

            {
                m_pit->destroyOneSpider(row(), col() - 1);
                m_col -= 2;
            }
            else if (m_pit->numberOfSpidersAt(row(), col() - 1) > 0 &&
                m_pit->numberOfSpidersAt(row(), col() - 2) > 0) 
            {
                m_pit->destroyOneSpider(row(), col() - 1);
                setDead();
                m_col -= 2;
            }
        }
        break;
    case RIGHT:                 
        if (!(col() == m_pit->cols()))
        {
            if (m_pit->numberOfSpidersAt(row(), col() + 1) == 0)
            {
                m_col++;
            }
            else if (m_pit->numberOfSpidersAt(row(), m_pit->cols()) > 0 && col() == m_pit->cols() - 1) 
            {
                m_pit->destroyOneSpider(row(), m_pit->cols());
                m_col++;
            }
            else if (m_pit->numberOfSpidersAt(row(), col() + 1) > 0 &&
                m_pit->numberOfSpidersAt(row(), col() + 2) == 0)

            {
                m_pit->destroyOneSpider(row(), col() + 1);
                m_col += 2;
            }
            else if (m_pit->numberOfSpidersAt(row(), col() + 1) > 0 &&
                m_pit->numberOfSpidersAt(row(), col() + 2) > 0)
            {
                m_pit->destroyOneSpider(row(), col() + 1);
                setDead();
                m_col += 2;
            }
        }
        break;
    }
    
}

bool Player::isDead() const
{
    // DONE: TRIVIAL:  Return whether the player is dead.
    return m_dead;  // this is wrong -- replace it
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementation
///////////////////////////////////////////////////////////////////////////

Pit::Pit(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Pit created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nSpiders = 0;
}

Pit::~Pit()
{
    // DONE:  Delete the player and all remaining dynamically allocated spiders.

    for (int i = 0; i < m_nSpiders; i++) {
        delete m_spiders[i];
    }
    delete m_player;
}

int Pit::rows() const
{
    // DONE: TRIVIAL:  Return the number of rows in the pit.
    return m_rows;  // this is wrong -- replace it
}

int Pit::cols() const
{
    // DONE: TRIVIAL:  Return the number of columns in the pit.
    return m_cols;  // this is wrong -- replace it
}

Player* Pit::player() const
{
    return m_player;
}

int Pit::spiderCount() const
{
    return m_nSpiders;
}

int Pit::numberOfSpidersAt(int r, int c) const
{
    // DONE:  Return the number of spiders at row r, column c.
    int numSpider = 0;

    for (int i = 0; i < m_nSpiders; i++) { //loop through all the spiders 
        if (m_spiders[i] != nullptr) {
            if (m_spiders[i]->row() == r && m_spiders[i]->col() == c) //if there is a spider at the position then increment numSpider
                numSpider++;
        }
    }

    return numSpider;  // this is wrong -- replace it
}

void Pit::display() const
{
    // Position (row,col) in the pit coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each spider's position
// DONE:  If one spider is at some grid point, set the char to 'S'.
//        If it's 2 though 8, set it to '2' through '8'. 
//        For 9 or more, set it to '9'.
    for (int i = 1; i < rows() + 1; i++) {
        for (int j = 1; j < cols() + 1; j++) {
            if (numberOfSpidersAt(i, j) == 1)
                grid[i - 1][j - 1] = 'S';
            else if (numberOfSpidersAt(i, j) > 1 && numberOfSpidersAt(i, j) < 9)
                grid[i - 1][j - 1] = '0' + numberOfSpidersAt(i, j);
            else if (numberOfSpidersAt(i, j) >= 9)
                grid[i - 1][j - 1] = '9';
            cout << "Number of Spiders at i, j" << numberOfSpidersAt(i, j) << endl;
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (m_player->isDead())
            gridChar = '*';
        else
            gridChar = '@';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, spider, and player info
    cout << "There are " << spiderCount() << " spiders remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Pit::addSpider(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a spider on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    // If there are MAXSPIDERS existing spiders, return false.
    // Otherwise, dynamically allocate a new spider at coordinates (r,c).
    // Save the pointer to newly allocated spider and return true.
    if (m_nSpiders >= MAXSPIDERS)
        return false;

    m_spiders[m_nSpiders] = new Spider(this, r, c);
    m_nSpiders++;
    // DONE:  Implement this.
    return true;  // This is wrong -- replace it.
}

bool Pit::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a spider
    if (numberOfSpidersAt(r, c) > 0)
        return false;

    // Dynamically allocate a new Player and add it to the pit
    m_player = new Player(this, r, c);
    return true;
}

bool Pit::destroyOneSpider(int r, int c)
{
    // DONE:  Destroy one spider at row r, column c.  Return true if a
    //        spider was destroyed, or false if there were no spiders there.
    int num_Spiders = 0;
    if (numberOfSpidersAt(r, c) > 0) {
        for (int i = 0; i < m_nSpiders; i++) { //loop through number of spiders
            if (m_spiders[i] != nullptr) {
                if (m_spiders[i]->row() == r && m_spiders[i]->col() == c) { //find spider at the position
                    num_Spiders = i; //set num_spiders to that position
                    break;
                }
            }

        }
        delete m_spiders[num_Spiders]; //delete the number of snakes at that position

        for (int k = num_Spiders; k < m_nSpiders; k++) {
            m_spiders[k] = m_spiders[k + 1]; //shift the array
        }
        --m_nSpiders;
        return true;
    }
    return false;  // this is wrong -- replace it
}

bool Pit::moveSpiders()
{
    for (int k = 0; k < m_nSpiders; k++)
    {
        m_spiders[k]->move();

        if ((m_player->row() == m_spiders[k]->row()) && (m_player->col() == m_spiders[k]->col()))
            m_player->setDead();
    }
    // DONE:  Have the k-th spider in the pit make one move.
    //        If that move results in that spider being in the same
    //        position as the player, the player dies.
// return true if the player is still alive, false otherwise
    return !m_player->isDead();
}

bool Pit::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSpiders)
{
    if (nSpiders < 0)
    {
        cout << "***** Cannot create Game with negative number of spiders!" << endl;
        exit(1);
    }
    if (nSpiders > MAXSPIDERS)
    {
        cout << "***** Trying to create Game with " << nSpiders
            << " spiders; only " << MAXSPIDERS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nSpiders > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the spiders!" << endl;
        exit(1);
    }

    // Create pit
    m_pit = new Pit(rows, cols);

    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_pit->addPlayer(rPlayer, cPlayer);

    // Populate with spiders
    while (nSpiders > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a spider where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        m_pit->addSpider(r, c);
        nSpiders--;
    }
}

Game::~Game()
{
    delete m_pit;
}

void Game::play()
{
    m_pit->display();
    Player* player = m_pit->player();
    if (player == nullptr)
        return;

    while (!m_pit->player()->isDead() && m_pit->spiderCount() > 0)
    {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin, action);
        if (action.size() == 0)
            player->stand();
        else
        {
            switch (action[0])
            {
            default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
            case 'q':
                return;
            case 'u':
            case 'd':
            case 'l':
            case 'r':
                player->move(decodeDirection(action[0]));
                break;
            }
        }
        if (!player->isDead())
            m_pit->moveSpiders();
        m_pit->display();
    }
    if (m_pit->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

int decodeDirection(char dir)
{
    switch (dir)
    {
    case 'u':  return UP;
    case 'd':  return DOWN;
    case 'l':  return LEFT;
    case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
    switch (dir)
    {
    case UP:     rowDelta = -1; colDelta = 0; break;
    case DOWN:   rowDelta = 1; colDelta = 0; break;
    case LEFT:   rowDelta = 0; colDelta = -1; break;
    case RIGHT:  rowDelta = 0; colDelta = 1; break;
    default:     return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////
void thisFunctionWillNeverBeCalled()
{
    // If the student deleted or changed the interfaces to the public
    // functions, this won't compile.  (This uses magic beyond the scope
    // of CS 31.)

    Spider(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Spider, row, int, () const);
    CHECKTYPE(Spider, col, int, () const);
    CHECKTYPE(Spider, move, void, ());

    Player(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, age, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, stand, void, ());
    CHECKTYPE(Player, move, void, (int));
    CHECKTYPE(Player, setDead, void, ());

    Pit(1, 1);
    CHECKTYPE(Pit, rows, int, () const);
    CHECKTYPE(Pit, cols, int, () const);
    CHECKTYPE(Pit, player, Player*, () const);
    CHECKTYPE(Pit, spiderCount, int, () const);
    CHECKTYPE(Pit, numberOfSpidersAt, int, (int, int) const);
    CHECKTYPE(Pit, display, void, () const);
    CHECKTYPE(Pit, addSpider, bool, (int, int));
    CHECKTYPE(Pit, addPlayer, bool, (int, int));
    CHECKTYPE(Pit, destroyOneSpider, bool, (int, int));
    CHECKTYPE(Pit, moveSpiders, bool, ());

    Game(1, 1, 1);
    CHECKTYPE(Game, play, void, ());
}

void doBasicTests()
{
    {
        Pit fall(10, 20);
        assert(fall.addPlayer(2, 6));
        Player* pp = fall.player();
        assert(pp->row() == 2 && pp->col() == 6 && !pp->isDead());
        pp->move(UP);
        assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
        pp->move(UP);
        assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
        pp->setDead();
        assert(pp->row() == 1 && pp->col() == 6 && pp->isDead());
    }
    {
        Pit tsburgh(2, 2);
        assert(tsburgh.addPlayer(1, 1));
        assert(tsburgh.addSpider(2, 2));
        Player* pp = tsburgh.player();
        assert(tsburgh.moveSpiders());
        assert(!pp->isDead());
        for (int k = 0; k < 1000 && tsburgh.moveSpiders(); k++)
            ;
        assert(pp->isDead());
    }
    {
        Pit brad(1, 5);
        assert(brad.addPlayer(1, 1));
        assert(brad.addSpider(1, 2));
        assert(brad.addSpider(1, 4));
        assert(brad.addSpider(1, 4));
        assert(brad.spiderCount() == 3 && brad.numberOfSpidersAt(1, 4) == 2);
        Player* pp = brad.player();
        pp->move(RIGHT);
        pp->move(RIGHT);
        assert(brad.spiderCount() == 1 && brad.numberOfSpidersAt(1, 4) == 1);
        pp->move(LEFT);
        assert(brad.spiderCount() == 0);
        assert(brad.addSpider(1, 5));
        assert(brad.spiderCount() == 1);
    }
    cout << "Passed all basic tests" << endl;
}

int main()
{
    // Create a game
      // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(2,4,5);

    // Play the game
    g.play();
    //doBasicTests(); // Remove this line after completing test.
    //return 0;       // Remove this line after completing test.

}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif