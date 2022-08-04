#ifndef BRAINFUCK_H
#define BRAINFUCK_H

/**
 * @brief Interprets brainfuck code- O(n)
 * Uses a vector of cells to interpret brainfuck code with recursive while loops
 * @param code - brainfuck code to be interpreted 
 */
void interpret(const auto &code);

/**
 * @brief Begins while loops from brainfuck interpretation- O(n)
 * Uses a stack to determine correct loop level and passes loop code to interpret function
 * @param code - brainfuck code
 * @param vp - pointer to cell in memory vector of interpreted brainfuck code
 * @param index - index at which to start loop checking
 */
void while_loop(const auto &code, auto &vp, auto &index);

/**
 * @brief Checks brainfuck code for loop safety
 * Uses a stack to ensure all brackets in brainfuck code are fully closed
 * @param code - brainfuck code to be checked for safety
 * @return true if code contains no loop errors
 * @return false if code contains loop errors
 */
bool safe_check(const auto &code);

/**
 * @brief Get the help object
 * Show command line options and usage
 */
void print_help();
#endif