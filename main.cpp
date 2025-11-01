#include <iostream>
#include <raylib.h>
using namespace std;

// Define screen states
typedef enum {
    SCREEN_MENU = 0,
    SCREEN_GAME
} GameScreen;

const int GRID_SIZE = 3;

int CheckWinner(int board[GRID_SIZE][GRID_SIZE]) {
    // Check rows and columns
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }

    // Check diagonals
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return 0; // No winner
}

bool IsBoardFull(int board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            if (board[i][j] == 0)
                return false;
    return true;
}

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TIC TAC TOE");
    Texture2D background = LoadTexture("bg_2.jpg");
    Texture2D background_2 = LoadTexture("bg_main.jpg");
    Font myFont = LoadFontEx("font_5.otf",200 ,0,0);
    SetTextureFilter(myFont.texture , TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);

    GameScreen currentScreen = SCREEN_MENU;

    // Menu button setup
    Rectangle button = { 650, 650, 100, 50 };
    Color buttonColor = LIGHTGRAY;

    // Grid setup
    const int cellSize = 140;
    const int offsetX = 150;
    const int offsetY = 180;

    // --- Game Variables ---
    int board[GRID_SIZE][GRID_SIZE] = { 0 }; // 0=empty, 1=X, 2=O
    int currentPlayer = 1;                   // Player 1 starts
    int winner = 0;
    bool gameOver = false;

    // --- Main Loop ---
    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        // =============== MENU SCREEN ===============
        if (currentScreen == SCREEN_MENU)
        {
            if (CheckCollisionPointRec(mouse, button))
            {
                // buttonColor = SKYBLUE;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    // Reset everything for a new game
                    for (int i = 0; i < GRID_SIZE; i++)
                        for (int j = 0; j < GRID_SIZE; j++)
                            board[i][j] = 0;

                    currentPlayer = 1;
                    winner = 0;
                    gameOver = false;

                    currentScreen = SCREEN_GAME;
                }
            }
            else buttonColor = LIGHTGRAY;
        }

        // =============== GAME SCREEN ===============
        else if (currentScreen == SCREEN_GAME)
        {
            if (!gameOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                int col = (mouse.x - offsetX) / cellSize;
                int row = (mouse.y - offsetY) / cellSize;

                if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
                {
                    if (board[row][col] == 0)
                    {
                        board[row][col] = currentPlayer;
                        winner = CheckWinner(board);

                        if (winner != 0 || IsBoardFull(board))
                            gameOver = true;
                        else
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    }
                }
            }

            // if (IsKeyPressed(KEY_ESCAPE))
            //     currentScreen = SCREEN_MENU;

            // Restart after game over
            if (gameOver && IsKeyPressed(KEY_ENTER))
            {
                for (int i = 0; i < GRID_SIZE; i++)
                    for (int j = 0; j < GRID_SIZE; j++)
                        board[i][j] = 0;

                currentPlayer = 1;
                winner = 0;
                gameOver = false;
            }
        }

        // =============== DRAW SECTION ===============
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == SCREEN_MENU)
        {
            DrawTexture(background_2 , 0 ,0 , WHITE);
            DrawTextEx(myFont,"TIC TAC TOE", (Vector2){100 , 50}, 100, 2, BLACK);
            DrawText("Press ESC to quit", 10, 770, 15, SKYBLUE);

            Color btncolor = (Color){ 0, 0, 0, 0};

            DrawRectangleRec(button , btncolor);
            // DrawRectangleLinesEx(button, 2, WHITE);
            DrawText("START...", button.x + 10, button.y + 15, 25, WHITE);
        }

        else if (currentScreen == SCREEN_GAME)
        {
            ClearBackground(RAYWHITE);
            DrawTexture(background , 0 ,0 , WHITE);

            // --- Draw Tic Tac Toe grid ---
            for (int i = 1; i < 3; i++)
            {
                DrawLine(offsetX + i * cellSize, offsetY, offsetX + i * cellSize, offsetY + 3 * cellSize, BLACK);
                DrawLine(offsetX, offsetY + i * cellSize, offsetX + 3 * cellSize, offsetY + i * cellSize, BLACK);
            }

            // --- Draw X and O ---
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = 0; j < GRID_SIZE; j++)
                {
                    int x = offsetX + j * cellSize + cellSize / 2;
                    int y = offsetY + i * cellSize + cellSize / 2;

                    if (board[i][j] == 1)
                        DrawText("X", x - 40, y - 60, 100, RED);
                    else if (board[i][j] == 2)
                        DrawText("O", x - 40, y - 60, 100, BLACK);
                }
            }

            // --- Game over message ---
            if (gameOver)
            {
                if (winner == 1)
                    DrawText("PLAYER X WINS!", 240, 700, 40, RED);
                else if (winner == 2)
                    DrawText("PLAYER O WINS!", 240, 700, 40, BLACK);
                else
                    DrawText("IT'S A DRAW!", 280, 700, 40, BROWN);

                DrawText("Press ENTER to play again", 240, 750, 20, BLUE);
            }
        }

        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}
