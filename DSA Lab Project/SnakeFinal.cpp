#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <queue>
#include <string>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include <sstream>


using namespace std;

bool gameover;
const int width = 20;
const int height = 20;
int o, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
int gameSpeedMode = 100; // Initial game speed
string playerName;

// Structure to represent a player with name and score
struct Player {
    string name;
    string difficulty;
    string gamemode;
    int score;

    bool operator<(const Player& other) const {
        return score < other.score;
    }
};

priority_queue<Player> highScores;
// Colors for snake customization
enum eColor { DEFAULT = 0, RED, GREEN, BLUE };
eColor snakeColor = DEFAULT;
// Game modes
enum eGameMode { CLASSIC = 0, TIME_TRIAL, SURVIVAL, SPEED };
eGameMode gameMode = CLASSIC;

// Function to save the player's score to their respective file
void SaveScore(const Player& player, int gameSpeed, string gameMode) {
    ofstream file(player.name + ".txt", ios::app);
    ofstream file2("highscores.txt", ios::app);
    if (file.is_open()) {
        file << "Score: " << player.score << endl;
        file << "Difficulty: " << gameSpeed << endl; // Store the game speed
        file << "Game Mode: " << gameMode << endl;
        file.close();
    }
    if (file2.is_open()) {
        file2 << "Player Name: " << player.name << endl;
        file2 <<  player.score << endl;
        file2 << "Difficulty: " << gameSpeed << endl; // Store the game speed
        file2 << "Game Mode: " << gameMode << endl;
        file2.close();
    }
}

// Function to view the high scores using priority queue
//void ViewHighScores() {
//      ifstream inputFile("highscores.txt"); // Replace "data.txt" with your file name
//    
//    if (!inputFile) {
//        cout << "Error opening file." << std::endl;
//        return scores;
//    }
//    
//    std::string line;
//    while (std::getline(inputFile, line)) {
//        std::cout << line << std::endl; // You can process the line here as per your requirements
//    }
//    
//    inputFile.close();
//    
//    return 0;
//}


// Function to see a player's records by reading their respective file
void SeePlayerRecords() {

    ifstream file(playerName + ".txt");
    if (file.is_open()) {
        string line;
        cout << "Scores for " << playerName << ":" << endl;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cout << "No records found for " << playerName << endl;
    }
}

// Function to choose the game difficulty level and return the corresponding game speed
int ChooseDifficulty() {
    int difficulty;
    cout << "Choose difficulty level (1-5): ";
    cin >> difficulty;
    int gameSpeed;

    switch (difficulty) {
    case 1:
        gameSpeed = 100;
        break;
    case 2:
        gameSpeed = 75;
        break;
    case 3:
        gameSpeed = 50;
        break;
    case 4:
        gameSpeed = 25;
        break;
    case 5:
        gameSpeed = 10;
        break;
    default:
        cout << "Invalid difficulty level. Setting to default speed." << endl;
        gameSpeed = 50; // Set default speed for invalid input
        break;
    }

    return gameSpeed;
}

// Function to choose the snake's color
void ChooseSnakeColor() {
    int colorChoice;
    cout << "Choose snake color:" << endl;
    cout << "1. Default" << endl;
    cout << "2. Red" << endl;
    cout << "3. Green" << endl;
    cout << "4. Blue" << endl;
    cout << "Enter your choice: ";
    cin >> colorChoice;

    switch (colorChoice) {
    case 1:
        snakeColor = DEFAULT;
        break;
    case 2:
        snakeColor = RED;
        break;
    case 3:
        snakeColor = GREEN;
        break;
    case 4:
        snakeColor = BLUE;
        break;
    default:
        cout << "Invalid color choice. Setting to default color." << endl;
        snakeColor = DEFAULT; // Set default color for invalid input
        break;
    }
}

// Function to show the main menu
void ShowMenu(const string& playerName) {
    cout << "===== Snake Game Menu =====" << endl;
    cout << "Player: " << playerName << endl;
    cout << "1. Play Game" << endl;
    cout << "2. View High Scores" << endl;
    cout << "3. See Your Records" << endl;
    cout << "4. Choose Difficulty Level" << endl;
    cout << "5. Customize Snake" << endl;
    cout << "6. Quit" << endl;
    cout << "==========================" << endl;
    cout << "Enter your choice: ";
}

void Setup()
{
    gameover = false;
    dir = STOP;
    o = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw()
{
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";

            if (i == y && j == o)
            {
                // Set snake color
                switch (snakeColor) {
                case DEFAULT:
                    cout << "O";
                    break;
                case RED:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Red color
                    cout << "O";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                    break;
                case GREEN:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // Green color
                    cout << "O";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                    break;
                case BLUE:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1); // Blue color
                    cout << "O";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                    break;
                }
            }
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else
            {
                bool printTail = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        // Set snake color for tail segments
                        switch (snakeColor) {
                        case DEFAULT:

                            cout << "o";
                           
                            break;
                        case RED:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // Red color
                            cout << "o";
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                            break;
                        case GREEN:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // Green color
                            cout << "o";
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                            break;
                        case BLUE:
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1); // Blue color
                            cout << "o";
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset to default color
                            break;
                        }
                        printTail = true;
                    }
                }
                if (!printTail)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}


void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'o':
            gameover = true;
            break;
        case 27: // 27 is the ASCII code for the Escape (Esc) key
            gameover = true;
            break;
        }
    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = o;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
    case LEFT:
        o--;
        break;
    case RIGHT:
        o++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (o >= width)
        o = 0;
    else if (o < 0)
        o = width - 1;

    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == o && tailY[i] == y)
        {
            gameover = true;
            break;
        }
    }

    if (o == fruitX && y == fruitY)
    {

        score += 10;
        if (gameMode != TIME_TRIAL) {
            PlaySound(TEXT("snake-hissing-6092- (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;



        // Increase game speed in Speed mode
        if (gameMode == SPEED) {
            gameSpeedMode -= 10;
        }
    }
}
// Modify the Logic() function

void Logic(int& gameSpeed) {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = o;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        o--;
        break;
    case RIGHT:
        o++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Check if the snake hits itself
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == o && tailY[i] == y) {
            gameover = true;
            break;
        }
    }

    // Check if the snake eats the fruit
    if (o == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;

    }

    // Check game mode specific conditions
    if (gameMode == SURVIVAL) {
        // Check if the snake hits the boundaries
        if (o >= width || o < 0 || y >= height || y < 0) {
            gameover = true;
            return;
        }
    }
}

// Function to handle the survival game mode
void SurvivalGame(int& gameSpeed) {

    Setup();
    while (!gameover) {
        Draw();
        Input();
        Logic(gameSpeed);
        Sleep(gameSpeed); // Sleep for 10 milliseconds to control the speed of the game
    }
    Player player;
    player.name = playerName;
    player.score = score;
    PlaySound(TEXT("mixkit-game-over-trombone-1940.wav"), NULL, SND_FILENAME | SND_ASYNC);
    cout << "Game Over!" << endl;
    SaveScore(player, 1, "Survival"); // Save score with difficulty 1 (Classic) and game mode "Classic"
}

// Function to handle the speed game mode
void SpeedGame() {
    Setup();
    while (!gameover) {
        Draw();
        Input();
        Logic();
        Sleep(gameSpeedMode);
    }
    Player player;
    player.name = playerName;
    player.score = score;
    PlaySound(TEXT("mixkit-game-over-trombone-1940.wav"), NULL, SND_FILENAME | SND_ASYNC);
    cout << "Game Over!" << endl;
    SaveScore(player, 1, "Speed Mode"); // Save score with difficulty 1 (Classic) and game mode "Classic"
}

void ClassicGameLoop(int& gameSpeed)
{
    while (!gameover)
    {
        Draw();
        Input();
        Logic();
        Sleep(gameSpeed); // Sleep for 10 milliseconds to control the speed of the game
    }

    Player player;
    player.name = playerName;
    player.score = score;
    PlaySound(TEXT("mixkit-game-over-trombone-1940.wav"), NULL, SND_FILENAME | SND_ASYNC);
    cout << "Game Over!" << endl;
    SaveScore(player, 1, "Classic"); // Save score with difficulty 1 (Classic) and game mode "Classic"
}


void TimeTrialGameLoop(int& gameSpeed)
{
     PlaySound(TEXT("ticking (online-audio-converter.com)-[AudioTrimmer.com].wav"), NULL, SND_FILENAME | SND_ASYNC | SND_SYNC);
    // Set a timer for 60 seconds
    time_t startTime = time(NULL);
    time_t endTime = startTime + 20; // 60 seconds

    while (!gameover && time(NULL) < endTime)
    {
        Draw();
        Input();
        Logic();
        Sleep(gameSpeed); // Sleep for 10 milliseconds to control the speed of the game
    }

    Player player;
    player.name = playerName;
    player.score = score;
    PlaySound(TEXT("mixkit-game-over-trombone-1940.wav"), NULL, SND_FILENAME | SND_ASYNC);
    cout << "Game Over!" << endl;
    SaveScore(player, 1, "Time Trial"); // Save score with difficulty 1 (Classic) and game mode "Time Trial"
}

int snakeGame() {
    cout << "Enter your name: ";
    cin >> playerName;

    int gameSpeed;

    while (true)
    {
        ShowMenu(playerName);

        int choice, gameMode;
        cin >> choice;

        switch (choice) {
        case 1:
        gamemodemenu:
            cout << "Select any game mode to challenge yourself: \n\n";
            cout << "1. Classic" << endl;
            cout << "2. Time Trial" << endl;
            cout << "3. Survival" << endl;
            cout << "4. Speed" << endl;
            //cout << "3. Obstacles" << endl;
            cin >> gameMode;


            switch (gameMode)
            {
            case 1:
                gameSpeed = ChooseDifficulty();
                gameMode = CLASSIC;
                Setup();
                ClassicGameLoop(gameSpeed);
                break;
            case 2:
                gameSpeed = ChooseDifficulty();
                gameMode = TIME_TRIAL;
                Setup();
                TimeTrialGameLoop(gameSpeed);
                break;

            case 3:
                gameSpeed = ChooseDifficulty();
                gameMode = SURVIVAL;
                SurvivalGame(gameSpeed);
                break;
            case 4:
                gameMode = SPEED;
                SpeedGame();
                break;
            default:
                cout << "Invalid option!\n";
                goto gamemodemenu;
                break;
            }


            break;
        case 2:
            SeePlayerRecords();
            cout << endl << endl;
            break;
        case 3:
            SeePlayerRecords();
            cout << endl << endl;
            break;
        case 4:
            gameSpeed = ChooseDifficulty();
            break;
        case 5:
            ChooseSnakeColor(); // Call function to choose snake color
            break;
        case 6:
            cout << "Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

}



using namespace std;
using namespace std::chrono;

const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
vector<vector<char>> mat;
char player = 'X';
int boardColor = 6;
int n;
int player1Score = 0;
int player2Score = 0;
const int MOVE_TIMEOUT = 10;  // Timeout in seconds

void clearBoard(int size) {
    mat.assign(size, vector<char>(size, ' '));
    player = 'X';
    n = 0;
}

void drawBoard() {
    system("cls");
    SetConsoleTextAttribute(h, 6);
    cout << " ==============================" << endl;
    cout << "|      TIC TAC TOE GAME        |" << endl;
    cout << " ==============================" << endl;
    SetConsoleTextAttribute(h, 15);

    for (int i = 0; i < mat.size(); i++) {
        SetConsoleTextAttribute(h, boardColor); // Set board color

        cout << " ";
        for (int j = 0; j < mat[i].size(); j++) {
            if (mat[i][j] == ' ')
                cout << "     ";
            else
                cout << "  " << mat[i][j] << "  ";

            if (j != mat[i].size() - 1)
                cout << "|";
        }
        cout << endl;

        if (i != mat.size() - 1) {
            cout << " ";
            for (int j = 0; j < mat[i].size(); j++) {
                cout << "-----";
                if (j != mat[i].size() - 1)
                    cout << "+";
            }
            cout << endl;
        }
    }
    cout << endl;
    SetConsoleTextAttribute(h, 15);
}

void inputMove() {
    int a;
    cout << "It's " << player << "'s Turn!" << endl;
    cout << "Enter the number where you want to mark => ";
    PlaySound(TEXT("mixkit-arcade-fast-game-over-233.wav"), NULL, SND_FILENAME | SND_ASYNC);

    time_point<system_clock> start = system_clock::now();  // Start the timer

    cin >> a;
    cout << endl;

    time_point<system_clock> end = system_clock::now();  // Stop the timer

    int elapsedSeconds = duration_cast<seconds>(end - start).count();  // Calculate elapsed seconds

    if (elapsedSeconds >= MOVE_TIMEOUT) {
        cout << "Timeout! You took more than " << MOVE_TIMEOUT << " seconds. Game over." << endl;
        PlaySound(TEXT("aww-8277 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
        exit(0);
    }

    int row = (a - 1) / mat.size();
    int col = (a - 1) % mat.size();

    if (row >= 0 && row < mat.size() && col >= 0 && col < mat[row].size() && mat[row][col] == ' ') {
        mat[row][col] = player;
    }
    else {
        cout << "Invalid Move! Try Again!" << endl;
        inputMove();
    }
}

void switchPlayer() {
    if (player == 'X')
        player = 'O';
    else
        player = 'X';
}

bool checkWin() {
    int size = mat.size();
    char currentPlayer = player;

    // Check rows
    for (int i = 0; i < size; i++) {
        bool win = true;
        for (int j = 0; j < size; j++) {
            if (mat[i][j] != currentPlayer) {
                win = false;
                break;
            }
        }
        if (win)
            return true;
    }

    // Check columns
    for (int j = 0; j < size; j++) {
        bool win = true;
        for (int i = 0; i < size; i++) {
            if (mat[i][j] != currentPlayer) {
                win = false;
                break;
            }
        }
        if (win)
            return true;
    }

    // Check diagonal top-left to bottom-right
    bool win = true;
    for (int i = 0; i < size; i++) {
        if (mat[i][i] != currentPlayer) {
            win = false;
            break;
        }
    }
    if (win)
        return true;

    // Check diagonal (top-right to bottom-left)
    win = true;
    for (int i = 0; i < size; i++) {
        if (mat[i][size - 1 - i] != currentPlayer) {
            win = false;
            break;
        }
    }
    if (win)
        return true;

    return false;
}

bool checkDraw() {
    int size = mat.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (mat[i][j] == ' ')
                return false;
        }
    }
    return true;
}

void playGameAgainstComputer() {
    int level;
    int size = 3;

    cout << "Choose Difficulty Level (1-5) => ";
    cin >> level;

    if (level < 1 || level > 5) {
        cout << "Invalid difficulty level! Exiting game." << endl;
        return;
    }
    // increase the size based on difficulty level
    size += (level - 1);
    clearBoard(size);
    drawBoard();

    while (true) {
        n++;
        if (player == 'X') {
            inputMove();
        }
        else {
            // Computer's turn
            int row, col;
            do {
                srand(time(0));
                row = rand() % size;
                col = rand() % size;
            } while (mat[row][col] != ' ');

            mat[row][col] = player;
            cout << "Computer marked: " << (row * size + col + 1) << endl;
        }

        drawBoard();

        if (checkWin()) {
            if (player == 'X') {
                PlaySound(TEXT("yay-6326 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
                cout << "Player 1 (X) won!" << endl;
                player1Score++;
            }
            else {
                PlaySound(TEXT("aww-8277 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
                cout << "Computer won!" << endl;
                player2Score++;
            }
            break;
        }
        else if (checkDraw()) {
            PlaySound(TEXT("game-over-arcade-6435 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
            cout << "It's a Draw!" << endl;
            break;
        }

        switchPlayer();
    }
}

void playGameMultiplayer() {
    int level;
    int size = 3; // Default size

    cout << "Choose Difficulty Level (1-5) => ";
    cin >> level;

    if (level < 1 || level > 5) {
        cout << "Invalid difficulty level! Exiting game." << endl;
        return;
    }
    // increase the size based on difficulty level
    size += (level - 1);
    clearBoard(size);
    drawBoard();

    while (true) {
        n++;
        inputMove();

        drawBoard();

        if (checkWin()) {
            cout << "Player " << player << " won!" << endl;
            PlaySound(TEXT("yay-6326 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
            if (player == 'X') {
                player1Score++;
            }
            else {
                PlaySound(TEXT("yay-6326 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
                player2Score++;
            }
            break;
        }
        else if (checkDraw()) {
            PlaySound(TEXT("aww-8277 (online-audio-converter.com).wav"), NULL, SND_FILENAME | SND_ASYNC);
            cout << "It's a Draw!" << endl;
            break;
        }

        switchPlayer();
    }
}

void displayMenu() {
    system("cls");
    SetConsoleTextAttribute(h, 9);
    cout << " ==============================" << endl;
    cout << "|      TIC TAC TOE GAME        |" << endl;
    cout << " ==============================" << endl;
    SetConsoleTextAttribute(h, 15);

    cout << "Select Game Mode:" << endl;
    cout << "1. Play against the computer" << endl;
    cout << "2. Play multiplayer with another human" << endl;
    cout << "Enter your choice => ";
}

void resetScores() {
    player1Score = 0;
    player2Score = 0;
}

void displayScores() {
    cout << "Player 1 (X) Score: " << player1Score << endl;
    cout << "Player 2 (O) Score: " << player2Score << endl;
}

int tictactoeGame() {

    
    char playAgain;
    do {
        displayMenu();
        int choice;
        cin >> choice;
        cout << "Enter the board color (0-15) => ";
        cin >> boardColor;

        switch (choice) {
        case 1:
            playGameAgainstComputer();
            break;
        case 2:
            playGameMultiplayer();
            break;
        default:
            cout << endl;
            cout << "Invalid choice! Exiting game." << endl;
            return 0;
        }

        displayScores();

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    resetScores();
}

int main()
{
    int option;
    SetConsoleTextAttribute(h, 13);
    system("cls");
    cout << "\n\n\n\n\n\n\t\t\t\t";
    string str = "W E L C O M E  TO  G A M I N G  A R E N A";
    for (int i = 0; i < str.size(); i++) {
        Sleep(60);
        cout << str[i];
    }
    cout << "\n\n\n\n\n\n";
    SetConsoleTextAttribute(h, 15);

    main_menu:
    cout << "\t\t\t\tPRESS 1 TO PLAY THE SNAKE GAME\n";
    cout << "\t\t\t\tPRESS 2 TO PLAY THE TIC TAC TOE\n";
    cin >> option;

    switch (option) {
        case 1:
            snakeGame();
            break;

        case 2:
            tictactoeGame();
            break;

        default:
            cout << "Invalid Option\n";
            goto main_menu;
            break;
    }

    return 0;
}



