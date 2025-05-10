#include <iostream>
#include <limits>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
using namespace std::chrono;

// ANSI escape codes for color
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"
#define CLEAR_SCREEN "\033[2J\033[1;1H"

const vector<char> rowLabels = {'A', 'B', 'C', 'D'};

int boardSize = 3;
vector<vector<char>> board;
char currentPlayer = 'X';
int scoreX = 0, scoreO = 0;
bool playWithBot = false;

// Initialize board
void initializeBoard(int size) {
    boardSize = size;
    board.assign(size, vector<char>(size, '-'));
}

// Print top bar
void printTopBar() {
    cout << CLEAR_SCREEN;
    cout << CYAN << " ===== X/O ===== " << boardSize << "x" << boardSize << " =====" << RESET << endl;
    cout << "Mode: " << (playWithBot ? "Player vs Bot" : "Player vs Player") << "\n";
    cout << "Player " << GREEN << "X" << RESET << ": " << scoreX << " | Player " << RED << "O" << RESET << ": " << scoreO << endl;
    cout << "Current turn: " << (currentPlayer == 'X' ? GREEN : RED) << currentPlayer << RESET << "\n\n";
}

// Display the board
void printBoard() {
    printTopBar();
    cout << "   ";
    for (int j = 0; j < boardSize; ++j) {
        cout << " " << static_cast<char>('A' + j) << "  ";
    }
    cout << endl;
    for (int i = 0; i < boardSize; ++i) {
        cout << " " << i + 1 << " ";
        for (int j = 0; j < boardSize; ++j) {
            char c = board[i][j];
            if (c == 'X') cout << GREEN << " " << c << " " << RESET;
            else if (c == 'O') cout << RED << " " << c << " " << RESET;
            else cout << "   ";
            if (j < boardSize - 1) cout << "|";
        }
        cout << endl;
        if (i < boardSize - 1) {
            cout << "   ";
            for (int j = 0; j < boardSize; ++j) {
                cout << "---";
                if (j < boardSize - 1) cout << "+";
            }
            cout << endl;
        }
    }
    cout << endl;
}

// Switch player
void switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

// Place move
bool makeMove(int row, int col) {
    if (row < 0 || col < 0 || row >= boardSize || col >= boardSize)
        return false;
    if (board[row][col] == '-') {
        board[row][col] = currentPlayer;
        return true;
    }
    return false;
}

// Bot move
void botMove() {
    srand(time(nullptr));
    int row, col;
    do {
        row = rand() % boardSize;
        col = rand() % boardSize;
    } while (!makeMove(row, col));
    this_thread::sleep_for(milliseconds(500));
}

// Check win
bool checkWin() {
    for (int i = 0; i < boardSize; ++i) {
        bool rowWin = true, colWin = true;
        for (int j = 1; j < boardSize; ++j) {
            rowWin &= (board[i][j] == board[i][0]);
            colWin &= (board[j][i] == board[0][i]);
        }
        if ((rowWin && board[i][0] != '-') || (colWin && board[0][i] != '-'))
            return true;
    }
    bool diag1 = true, diag2 = true;
    for (int i = 1; i < boardSize; ++i) {
        diag1 &= (board[i][i] == board[0][0]);
        diag2 &= (board[i][boardSize - 1 - i] == board[0][boardSize - 1]);
    }
    if ((diag1 && board[0][0] != '-') || (diag2 && board[0][boardSize - 1] != '-'))
        return true;

    return false;
}

// Check draw
bool checkDraw() {
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            if (board[i][j] == '-')
                return false;
    return true;
}

// Get move from user
pair<int, int> getMove() {
    string input;
    while (true) {
        cout << "Player " << currentPlayer << ", enter your move (ex: a1/A1 or ra9m lkhana): ";
        cin >> input;

        if (input.length() == 2 && isalpha(input[0]) && isdigit(input[1])) {
            int col = toupper(input[0]) - 'A';
            int row = input[1] - '1';
            if (row >= 0 && row < boardSize && col >= 0 && col < boardSize)
                return {row, col};
        } else if (input.length() == 1 && isdigit(input[0])) {
            int cell = input[0] - '1';
            int row = cell / boardSize;
            int col = cell % boardSize;
            if (row >= 0 && row < boardSize && col >= 0 && col < boardSize)
                return {row, col};
        }

        cout << "Invalid input. i3ada.\n";
    }
}

// Print game instructions
void printInstructions() {
    cout << CLEAR_SCREEN;
    cout << CYAN << "===== bghit-nefham =====" << RESET << endl;
    cout << "Welcome 3nd!/ aji nfahmek" << endl;
    cout << "Dakxi bssiit tebba3 m3aya:" << endl;
    cout << "1. clicki 'p' tl3ab ded 3xranek / clickiti 'b' tl3ab m3a l pc." << endl;
    cout << "2. l board mre9ma b7al lklavi diyal tilifon 3dam (1 to 9 for 3x3) \n o kayna had l7ala lli ttxbeh l chess-board(ex: A1, B2, etc.)." << endl;
    cout << "3. t9der tl3ab f joj dyal lboard types ya 4*4 ya lma3rofa 3*3." << endl;
    cout << "4. 'X' dima ty bda howa lowel so ttafe9 m3a 3xirek xkon llowel ghaybda." << endl;
    cout << "5. mn ba3d kola game t9der ya trematxi b twerrek 3la ayy botna mn ghir 'q' 7it ila mabghitix tkemel tt kteb 'q'." << endl;
    cout << "Press any key to return to the main menu..." << endl;
    cin.get();
    cin.get();  // Wait for player to press a key
}

// Main menu
void menu() {
    char mode, size;
    cout << CLEAR_SCREEN;
    cout << CYAN << "3regt bax n9ad had l game wayni haniya doz t9asser" << RESET << endl;
    cout << "Press 'h' for instructions or press any key to start: ";
    cin >> mode;

    if (mode == 'h') {
        printInstructions();
        return;  // Return to menu after showing instructions
    }

    cout << "Choose game mode - (p)layer vs player or (b)ot: ";
    cin >> mode;
    playWithBot = (mode == 'b');

    cout << "Choose board size - (3) for 3x3, (4) for 4x4: ";
    cin >> size;
    if (size == '4') boardSize = 4;
    else boardSize = 3;

    initializeBoard(boardSize);
}

// Main game loop
int main() {
    while (true) {
        menu();
        auto start = steady_clock::now();
        while (true) {
            printBoard();
            auto moveStart = steady_clock::now();
            if (playWithBot && currentPlayer == 'O') botMove();
            else {
                auto [row, col] = getMove();
                if (!makeMove(row, col)) {
                    cout << "Cell already taken. chof mzyan.\n";
                    continue;
                }
            }
            auto moveEnd = steady_clock::now();
            auto moveDuration = duration_cast<seconds>(moveEnd - moveStart).count();
            cout << MAGENTA << "Time taken for the move: " << moveDuration << " seconds." << RESET << endl;
            
            if (checkWin()) {
                printBoard();
                cout << "Player " << currentPlayer << " wins!\n";
                (currentPlayer == 'X' ? scoreX : scoreO)++;
                break;
            }
            if (checkDraw()) {
                printBoard();
                cout << "It's a draw!\n";
                break;
            }

            switchPlayer();
        }
        cout << "Press 'q' to quit or any other key to play again: ";
        char playAgain;
        cin >> playAgain;
        if (playAgain == 'q') break;
    }
    return 0;
}
