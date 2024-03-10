#include <iostream>
#include <array>
#include <string>
#include <unordered_map>
#include "raylib.h" // Library for GUI

const float screenWidth = 900;
const float screenHeight = 900;
const int fontSize = 300;
const Color grayOverlay = {130, 130, 130, 200};

enum class status
{
    empty, x, o
};

using boardType = std::array<std::array<status, 3>, 3>;

struct locationInGrid
{
    int row;
    int col;
};

std::unordered_map<int, float> drawPosX = {{0, screenWidth/6}, {1, screenWidth/2}, {2, screenWidth*5/6}};
std::unordered_map<int, float> drawPosY = {{0, screenHeight/6}, {1, screenHeight/2}, {2, screenHeight*5/6}};

void setupBoard(boardType &board)
{
    for (auto &row : board)
    {
        for (auto &col : row)
        {
            col = status::empty;
        }
    }
}

void drawXO(const std::string charToDraw, const int row, const int col, Color colorToUse)
{
    DrawText(&charToDraw[0], drawPosX[col] - MeasureText(&charToDraw[0], fontSize)/2, 
             drawPosY[row]- MeasureText(&charToDraw[0], fontSize) - 10.0f, fontSize, colorToUse);
}

void drawGame(boardType &board, const bool isWin, const char winner)
{
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    DrawLineEx({0, screenHeight/3.0f}, {screenWidth, screenHeight/3.0f}, 5.0f, BROWN);
    DrawLineEx({0, screenHeight*2/3.0f}, {screenWidth, screenHeight*2/3.0f}, 5.0f, BROWN);
    DrawLineEx({screenWidth/3.0f, 0}, {screenWidth/3.0f, screenHeight}, 5.0f, BROWN);
    DrawLineEx({screenWidth*2/3.0f, 0}, {screenWidth*2/3.0f, screenHeight}, 5.0f, BROWN);

    for (int i = 0; i < 3; i++)
    {
        for (int c = 0; c < 3; c++)
        {
            switch (board[i][c])
            {
                case status::x:
                    drawXO("x", i, c, BLUE);
                    break;
                case status::o:
                    drawXO("o", i, c, RED);
                    break;
                default:
                    break;
            }
        }
    }

    if (isWin)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, grayOverlay);

        if (winner == 'x')
        {
            Vector2 winTextOffset = MeasureTextEx(GetFontDefault(), "X won the game!", 75, 0);
            DrawText("X won the game!", screenWidth/2 - winTextOffset.x/2 - 50, screenHeight/2 - winTextOffset.y/2 - 90, 
                      75, WHITE);
        }
        else if (winner == 'o')
        {
            Vector2 winTextOffset = MeasureTextEx(GetFontDefault(), "O won the game!", 75, 0);
            DrawText("O won the game!", screenWidth/2 - winTextOffset.x/2 - 50, screenHeight/2 - winTextOffset.y/2 - 90, 
                      75, WHITE);
        }
        else
        {
            Vector2 drawTextOffset = MeasureTextEx(GetFontDefault(), "Game is a draw!", 75, 0);
            DrawText("Game is a draw!", screenWidth/2 - drawTextOffset.x/2 - 50, screenHeight/2 - drawTextOffset.y/2 - 90, 
                      75, WHITE);
        }

        DrawRectangle(screenWidth/2 - 330, screenHeight/2 + 10, 300, 100, LIGHTGRAY);
        DrawText("Play again", screenWidth/2 - 308, screenHeight/2 + 35, 50, BLACK);

        DrawRectangle(screenWidth/2 + 30, screenHeight/2 + 10, 300, 100, LIGHTGRAY);
        DrawText("Close game", screenWidth/2 + 43, screenHeight/2 + 35, 50, BLACK);
    }
    EndDrawing();
}

int getRowCol(float Loc)
{
    if (Loc < 300)
    {
        return 0;
    }
    else if (Loc > 300 && Loc < 600)
    {
        return 1;
    }
    else if (Loc > 600)
    {
        return 2;
    }
    return -1;
}

void fill(boardType &board, locationInGrid currLoc, status whoseTurn)
{
    if (whoseTurn == status::x)
    {
        board[currLoc.row][currLoc.col] = status::x;
    }
    else
    {
        board[currLoc.row][currLoc.col] = status::o;
    }
}

bool checkRow(const boardType &board)
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] != status::empty && board[i][1] != status::empty && board[i][2] != status::empty)
        {
            if (board[i][0] == board[i][1] && board[i][0] == board[i][2])
            {
                return true;
            }
        }
    }
    return false;
}

bool checkCol(const boardType &board)
{
    for (int i = 0; i < 3; i++)
    {
        if (board[0][i] != status::empty && board[1][i] != status::empty && board[2][i] != status::empty)
        {
            if (board[0][i] == board[1][i] && board[0][i] == board[2][i])
            {
                return true;
            }
        }
    }
    return false;
}

bool checkDiag(const boardType &board)
{
   
    if (board[0][0] != status::empty && board[0][0] == board[1][1] && board[0][0] == board[2][2])
    {
        return true;
    }
    else if (board[0][2] != status::empty && board[0][2] == board[1][1] && board[0][2] == board[2][0])
    {
        return true;
    }
    return false;
}

bool checkWin(const boardType &board)
{
    if (checkRow(board) || checkCol(board) || checkDiag(board))
    {
        return true;
    }
    return false;
}

void mouseInput(boardType &board, bool &isWin, bool &whoseTurn, int &turnCount, char &winner)
{
    Vector2 mousePos = GetMousePosition();
    locationInGrid currLoc = {getRowCol(mousePos.y), getRowCol(mousePos.x)};
    if (board[currLoc.row][currLoc.col] == status::empty)
    {
        fill(board, currLoc, (whoseTurn) ? status::x : status::o);
    }
    else
    {
        return;
    }

    isWin = checkWin(board);
    if (isWin)
    {
        winner = (whoseTurn) ? 'x' : 'o';
    }
    else if (turnCount == 9)
    {
        winner = '='; // Draw
        isWin = true;
    }
    whoseTurn = !whoseTurn; // x -> o/o -> x
    ++turnCount;
    return;
}

int main()
{
    InitWindow(screenWidth, screenWidth, "Tic Tac Toe");
    Image icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    SetTargetFPS(60);

    boardType board;
    setupBoard(board);
    bool isWin = false;
    bool whoseTurn = true; // true = x, false = o
    int turnCount = 1;
    char winner = '-';

    while(!WindowShouldClose())
    {
        drawGame(board, isWin, winner);
        if (!isWin)
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // If left click
            {
                mouseInput(board, isWin, whoseTurn, turnCount, winner);
            }
        }
        else if (isWin)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mousePos = GetMousePosition();
                if (mousePos.y >= screenHeight/2 + 10 && mousePos.y <= screenHeight/2 + 110)
                {
                    if (mousePos.x >= screenWidth/2 - 308 && mousePos.x <= screenWidth/2 - 8)
                    {
                        setupBoard(board);
                        isWin = false;
                        whoseTurn = true; // reset to x's turn
                        turnCount = 1;
                        winner = '-';
                    }
                    else if (mousePos.x >= screenWidth/2 + 43 && mousePos.x <= screenWidth/2 + 343)
                    {
                        CloseWindow();
                        return 0;
                    }
                }
            }
        }
    }

    CloseWindow();
    return 0;
}