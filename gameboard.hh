#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <vector>
#include <string>

enum Element_type {ZERO, ONE, EMPTY};
const int NUMBER_OF_SYMBOLS = 3;
const int SIZE = 2 * NUMBER_OF_SYMBOLS;
const int DISTR_UPPER_BOUND = 7;

// Width of the left-most column, needed in printing the gameboard.
const unsigned int LEFT_COLUMN_WIDTH = 5;

class GameBoard
{
public:
    // Constructor. Calls init() to fill the board with EMPTYs.
    GameBoard();

    bool fill_randomly(unsigned int seed, unsigned int size);

    bool fill_from_input(const std::string& input, unsigned int size);

    bool ok_adjacent_symbols() const;

    bool ok_amount_of_symbols() const;

    bool add_symbol(unsigned int x, unsigned int y, char symbol_char);

    bool is_game_over() const;

    void recreate_board(unsigned int size); // Recreates the board with the given size.

    void print() const;

    // returns the gameboard
    std::vector<std::vector<Element_type>> get_board() const;

private:
    void init();

    void print_line(unsigned int length, char fill_character) const;

    bool ok_adjacent_syms_in_rows(Element_type elem) const;

    bool ok_adjacent_syms_in_columns(Element_type elem) const;

    // Gameboard
    std::vector<std::vector<Element_type>> board_;

    const std::vector<unsigned int> BAD_SEEDS = { 2, 8, 12, 13, 16, 20, 21, 23,
                                                  26, 29, 31, 32, 34, 41, 42,
                                                  43, 44, 46 };
};

#endif // GAMEBOARD_HH
