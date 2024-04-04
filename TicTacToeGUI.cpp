// Program ini memerlukan library external raylib untuk dijalankan
// Raylib: https://github.com/raysan5/raylib
// Referensi untuk fungsi, tipe data, warna yang disediakan Raylib: https://raylib.com/cheatsheet/cheatsheet.html
// Anggota kelompok: Steven XIIA6, Frederick XIIA7, Kenneth XIIA7, Vincent XIIA7
// Versi sederhana tanpa GUI (Tidak perlu menggunakan Raylib): https://pastebin.com/xmdR8DVc

#include <iostream>
#include <array>
#include <string>
#include <unordered_map>
#include "raylib.h" // Library untuk GUI

float screenSize; // Lebar & tinggi window
float fontSize; // Ukuran font (untuk X dan O)
const Color grayOverlay = {130, 130, 130, 200}; // Warna abu

enum class status
{
    empty, x, o // Status antara kosong, x, atau o
};

using boardType = std::array<std::array<status, 3>, 3>; // boardType sebagai nama lain untuk tipe data array 3x3

struct locationInGrid // Tipe data untuk menyetor baris & kolom input
{
    int row;
    int col;
};

std::unordered_map<int, float> drawPos;
// Posisi untuk menggambar (sumbu X)

void initDrawPos()
{
    drawPos[0] = screenSize/6; // Kolom & baris 0
    drawPos[1] = screenSize/2; // Kolom & baris 1
    drawPos[2] = screenSize*5/6; // Kolom & baris 2
}

void setupBoard(boardType &board) // Setup papan
{
    for (auto &row : board) // Untuk setiap baris di papan
    {
        for (auto &col : row) // Untuk setiap kolom di baris
        {
            col = status::empty; // Kolom dalam baris diset menjadi kosong
        }
    }
}

void drawXO(const std::string charToDraw, const int row, const int col, Color colorToUse) // Gambar X & O
{
    Vector2 drawTextOffset = MeasureTextEx(GetFontDefault(), &charToDraw[0], fontSize, (fontSize/GetFontDefault().baseSize));
    DrawText(&charToDraw[0], drawPos[col] - drawTextOffset.x/2, drawPos[row] - drawTextOffset.y/2 - screenSize/90, fontSize, 
            colorToUse);
    // Gambar X & O menggunakan fungsi DrawText
    // Format: DrawText(Karakter untuk digambar, Posisi X, Posisi Y, Ukuran font, Warna fint);
}

void drawGame(boardType &board, const bool gameOver, const char winner) // Gambar papan
{
    BeginDrawing(); // Mulai menggambar
    ClearBackground(LIGHTGRAY); // Background warna abu muda
    DrawLineEx({0, screenSize/3.0f}, {screenSize, screenSize/3.0f}, 5.0f, BROWN); // Garis pemisah vertikal 1
    DrawLineEx({0, screenSize*2/3.0f}, {screenSize, screenSize*2/3.0f}, 5.0f, BROWN); // Garis pemisah vertikal 2
    DrawLineEx({screenSize/3.0f, 0}, {screenSize/3.0f, screenSize}, 5.0f, BROWN); // Garis pemisah horizontal 1
    DrawLineEx({screenSize*2/3.0f, 0}, {screenSize*2/3.0f, screenSize}, 5.0f, BROWN); // Garis pemisah horizontal 2

    // Menggambar semua X & O
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

    if (gameOver) // Apabila game sudah selesai
    {
        DrawRectangle(0, 0, screenSize, screenSize, grayOverlay); // Overlay abu

        if (winner == 'x') // Apabila x menang
        {
            Vector2 winTextOffset = MeasureTextEx(GetFontDefault(), "X won the game!", screenSize/12, 
                                    (screenSize/12/GetFontDefault().baseSize)); // Offset posisi menulis
            DrawText("X won the game!", screenSize/2 - winTextOffset.x/2, screenSize*2/5 - winTextOffset.y/2, 
                      screenSize/12, WHITE);
        }
        else if (winner == 'o') // Apabila o menang
        {
            Vector2 winTextOffset = MeasureTextEx(GetFontDefault(), "O won the game!", screenSize/12,
                                    (screenSize/12/GetFontDefault().baseSize));
            DrawText("O won the game!", screenSize/2 - winTextOffset.x/2, screenSize*2/5 - winTextOffset.y/2, 
                      screenSize/12, WHITE);
        }
        else // Apabila seri
        {
            Vector2 drawTextOffset = MeasureTextEx(GetFontDefault(), "Game is a draw!", screenSize/12, 
                                     (screenSize/12/GetFontDefault().baseSize));
            DrawText("Game is a draw!", screenSize/2 - drawTextOffset.x/2, screenSize*2/5 - drawTextOffset.y/2, 
                      screenSize/12, WHITE);
        }
        // Tombol "Play again"
        DrawRectangle(screenSize/2 - screenSize/2.727f, screenSize*23/45, screenSize/3, screenSize/9, LIGHTGRAY);
        Vector2 drawTextOffset = MeasureTextEx(GetFontDefault(), "Play again", screenSize/18, 
                                (screenSize/18/GetFontDefault().baseSize));
        DrawText("Play again", screenSize * 2/3 - screenSize/2.727f - drawTextOffset.x/2 , screenSize * 17/30 - 
                 drawTextOffset.y/2, screenSize/18, BLACK);

        // Tombol "Close game"
        DrawRectangle(screenSize*8/15, screenSize*23/45, screenSize/3, screenSize/9, LIGHTGRAY);
        drawTextOffset = MeasureTextEx(GetFontDefault(), "Close game", screenSize/18, 
                        (screenSize/18/GetFontDefault().baseSize));
        DrawText("Close game", screenSize * 7/10 - drawTextOffset.x/2, screenSize * 17/30 - drawTextOffset.y/2, screenSize/18, 
                  BLACK);
    }
    EndDrawing(); // Selesai menggambar
}

int getRowCol(float Loc) // Ubah koordinat menjadi angka baris/kolom
{
    if (Loc < screenSize/3) // Apabila berada di baris/kolom pertama
    {
        return 0; // Kembalikan angka 0
    }
    else if (Loc < screenSize*2/3) // Apabila berada di baris/kolom kedua
    {
        return 1; // Kembalikan angka 1
    }
    else // Apabila berada di baris/kolom ketiga
    {
        return 2; // Kembalikan angka 2
    }
    return -1; // Kembalikan angka -1 (tidak bisa terjadi)
}

void fill(boardType &board, locationInGrid currLoc, status whoseTurn) // Isi kotak yang kosong
{
    if (whoseTurn == status::x) // Apabila giliran x
    {
        board[currLoc.row][currLoc.col] = status::x; // Ubah kotak menjadi x
    }
    else // Apabila giliran o
    {
        board[currLoc.row][currLoc.col] = status::o; // Ubah kotak menjadi o
    }
}

bool checkRowCol(const boardType &board) // Cek apabila ada pemenang (3 dalam satu baris)
{
    for (int i = 0; i < 3; i++) // Untuk setiap baris dalam papan
    {
        if (board[i][0] != status::empty && board[i][0] == board[i][1] && board[i][0] == board[i][2])
        // Apabila kolom 0 tidak kosong & semua kolom dalam baris memiliki nilai yang sama
        {
            return true; // Ada pemenang
        }
        else if (board[0][i] != status::empty && board[0][i] == board[1][i] && board[0][i] == board[2][i])
        // Apabila baris 0 tidak kosong & semua baris dalam kolom memiliki nilai yang sama
        {
            return true; // Ada pemenang
        }
    }
    return false; // Belum ada pemenang
}

bool checkDiag(const boardType &board) // Cek apabila ada pemenang (3 dalam diagonal yang sama)
{
    if (board[0][0] != status::empty && board[0][0] == board[1][1] && board[0][0] == board[2][2])
    // Apabila (0,0) tidak kosong & nilai (0,0) = nilai (1,1) = nilai (2,2)
    {
        return true; // Ada pemenang
    }
    else if (board[0][2] != status::empty && board[0][2] == board[1][1] && board[0][2] == board[2][0])
    // Apabila (0,2) tidak kosong & nilai (0,2) = nilai (1,1) = nilai (2,0)
    {
        return true; // Ada pemenang
    }
    return false; // Belum ada pemenang
}

bool checkWin(const boardType &board) // Cek apabila ada pemenang
{
    if (checkRowCol(board) || checkDiag(board)) // Apabila salah satu dari 3 fungsi mengembalikan true
    {
        return true; // Ada pemenang
    }
    return false; // Belum ada pemenang
}

void mouseInput(boardType &board, bool &gameOver, bool &whoseTurn, int &turnCount, char &winner) // Proses input mouse
{
    Vector2 mousePos = GetMousePosition(); // Ambil koordinat mouse
    locationInGrid currLoc = {getRowCol(mousePos.y), getRowCol(mousePos.x)}; // Ubah koordinat menjadi baris & kolom, setor 
                                                                             // dalam variabel currLoc
    if (board[currLoc.row][currLoc.col] == status::empty) // Apabila lokasi yang dipilih masih kosong
    {
        fill(board, currLoc, (whoseTurn) ? status::x : status::o); // Isi kotak
        // format: fill(Papan, Baris & kolom, Giliran siapa (true = x, false = o));
    }
    else // Apabila lokasi yang dipilih sudah diisi
    {
        return; // Akhiri fungsi, kembali ke fungsi main()
    }

    gameOver = checkWin(board); // Cek apabila sudah ada pemenang
    if (gameOver) // Apabila sudah ada pemenang
    {
        winner = (whoseTurn) ? 'x' : 'o'; // Set pemenang menjadi x/o tergantung giliran siapa (true = x, false = o)
    }
    else if (turnCount == 9) // Apabila semua kotak telah terisi
    {
        winner = '='; // Seri
        gameOver = true; // Game sudah selesai
    }
    whoseTurn = !whoseTurn; // Ubah x (true) -> o (false) atau o (false) -> x (true)
    ++turnCount; // Jumlah giliran +1
    return; // Kembali ke main()
}

int main()
{
    InitWindow(0, 0, "Tic Tac Toe"); // Buka window baru bernama "Tic Tac Toe"
    // Window diposisikan di tengah monitor
    SetWindowPosition(GetScreenWidth()/2 - GetScreenHeight()/4, GetScreenHeight()/2 - GetScreenHeight()/4); 
    screenSize = GetScreenHeight()/2; // Ukuran layar disesuaikan dengan tinggi monitor
    SetWindowSize(screenSize, screenSize);
    fontSize = screenSize/3;
    initDrawPos();

    SetTargetFPS(60); // Window digambar sebanyak 60 kali per detik

    boardType board; // Papan dengan tipe boardType (array 3x3)
    setupBoard(board); // Setup papan
    bool gameOver = false; // Permainan belum selesai
    bool whoseTurn = true; // true = x, false = o
    int turnCount = 1; // Giliran pertama
    char winner = '-'; // Belum ada pemenang

    while(!WindowShouldClose()) // Selama window tidak ditutup
    {
        drawGame(board, gameOver, winner); // Gambar papan
        if (!gameOver) // Selama belum menang/seri
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Apabila ada input left click
            {
                mouseInput(board, gameOver, whoseTurn, turnCount, winner); // Proses input mouse
            }
        }
        else if (gameOver) // Apabila permainan selesai
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Apabila ada input left click
            {
                Vector2 mousePos = GetMousePosition(); // Ambil koordinat mouse
                if (mousePos.y >= screenSize*23/45 && mousePos.y <= screenSize*28/45) // Play again/close game (Cek sumbu Y)
                {
                    // Play again (Cek sumbu X)
                    if (mousePos.x >= screenSize/2 - screenSize/2.727f && mousePos.x <= screenSize*5/6 - screenSize/2.727f)
                    {
                        setupBoard(board); // Reset papan
                        gameOver = false; // Permainan belum selesai
                        whoseTurn = true; // Reset menjadi giliran X
                        turnCount = 1; // Reset menjadi giliran pertama
                        winner = '-'; // Reset sehingga belum ada pemenang
                    }
                    else if (mousePos.x >= screenSize*8/15 && mousePos.x <= screenSize*13/15) // Close game (Cek sumbu X)
                    {
                        CloseWindow(); // Tutup window
                        return 0; // Akhiri program
                    }
                }
            }
        }
    }

    CloseWindow(); // Tutup window
    return 0;
}