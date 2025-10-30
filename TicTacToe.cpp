#include <array>
#include <iostream>
#include <conio.h>

//https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replaceInString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


constexpr int ENTER = 13;

constexpr int ESCAPE_CHAR = 224;
constexpr int UP_ARROW = 72;
constexpr int LEFT_ARROW = 75;
constexpr int RIGHT_ARROW = 77;
constexpr int DOWN_ARROW = 80;

enum TileState {
    NONE = ' ',
    X = 'X',
    O = 'O'
};

enum GameEnd {
    CONTINUE,
    TIE,
    X_WON,
    O_WON
};

void drawGameBoard(const std::array<TileState, 9>* gameBoard, const bool& turn, const int& pointerX, const int& pointerY) {
    std::string text = "111|222|333\n1A1|2B2|3C3\n111|222|333\n-----------\n444|555|666\n4D4|5E5|6F6\n444|555|666\n-----------\n777|888|999\n7G7|8H8|9I9\n777|888|999\n";

    for (int i = 0;i<9;i++) {
        for (int counter = 0;counter<8;counter++) {
            if (i == pointerX + (pointerY*3)) {
                replaceInString(text, std::to_string(i+1), "#");
            } else {
                replaceInString(text, std::to_string(i+1), " ");
            }
        }

        std::string character;
        character += static_cast<char>(i + 65);

        std::string piece;
        piece += static_cast<char>(gameBoard -> at(i));

        replaceInString(text,
            character,
            piece
        );
    }

    if (turn) std::cout << "Player X turn!" << std::endl;
    else std::cout << "Player O turn!" << std::endl;

    std::cout << std::endl << text << std::endl;
    std::cout << "Press enter to confirm turn, use arrow keys to move\nPress Q to exit..." << std::endl;
}

void handleInput(const int& ch, int& pointerX, int& pointerY) {
    if (ch == UP_ARROW) pointerY--;
    if (ch == DOWN_ARROW) pointerY++;

    if (ch == LEFT_ARROW) pointerX--;
    if (ch == RIGHT_ARROW) pointerX++;

    if (pointerX < 0) pointerX = 2;
    if (pointerX > 2) pointerX = 0;

    if (pointerY < 0) pointerY = 2;
    if (pointerY > 2) pointerY = 0;
}

GameEnd checkBoard(const std::array<TileState, 9>* gameBoard) {
    // Winning combinations
    static const int wins[8][3] = {
        {0, 1, 2}, // Row 1
        {3, 4, 5}, // Row 2
        {6, 7, 8}, // Row 3
        {0, 3, 6}, // Column 1
        {1, 4, 7}, // Column 2
        {2, 5, 8}, // Column 3
        {0, 4, 8}, // Diagonal 1
        {2, 4, 6}  // Diagonal 2
    };

    // Check for X or O wins
    for (const auto& combo : wins) {
        if (gameBoard->at(combo[0]) == X &&
            gameBoard->at(combo[1]) == X &&
            gameBoard->at(combo[2]) == X)
            return X_WON;

        if (gameBoard->at(combo[0]) == O &&
            gameBoard->at(combo[1]) == O &&
            gameBoard->at(combo[2]) == O)
            return O_WON;
    }

    // Check for empty tiles
    for (int i = 0; i < 9; i++)
        if (gameBoard->at(i) == NONE)
            return CONTINUE;

    // Otherwise it's a tie
    return TIE;
}

GameEnd gameLoop() {
    int pointerX = 0;
    int pointerY = 0;
    bool turn = false;

    auto* gameBoard = new std::array<TileState, 9>{
        NONE, NONE, NONE,
        NONE, NONE, NONE,
        NONE, NONE, NONE
    };

    while (true) {
        system("CLS");

        drawGameBoard(gameBoard, turn, pointerX, pointerY);

        int ch = getch();

        if (ch == 'q' || ch == 27) break;

        if (ch == ESCAPE_CHAR) {
            ch = getch();
            handleInput(ch, pointerX, pointerY);
            continue;
        }

        if (ch == ENTER && gameBoard -> at(pointerX + pointerY*3) == NONE) {
            gameBoard -> at(pointerX + pointerY*3) = turn ? X : O;

            turn = !turn;
        }

        GameEnd gameEnd = checkBoard(gameBoard);
        if (gameEnd != CONTINUE) {
            system("CLS");

            drawGameBoard(gameBoard, turn, pointerX, pointerY);

            delete gameBoard;
            return gameEnd;
        }
    }

    delete gameBoard;
    return checkBoard(gameBoard);
}

int main() {
    while (true) {
        GameEnd gameEnd = gameLoop();

        if (gameEnd == TIE) std::cout << "\nIt's a Tie!" << std::endl;
        else if (gameEnd == X_WON) std::cout << "\nPlayer X Won!" << std::endl;
        else if (gameEnd == O_WON) std::cout << "\nPlayer O Won!" << std::endl;

        std::cout << "\nAnother round y/n?" << std::endl;

        int ch = getch();

        if (ch == 'y') continue;
        else if (ch == 'n') break;
    }

    return 0;
}
