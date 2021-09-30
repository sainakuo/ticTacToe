#include <iostream>
#include <chrono>
#include <random>
#include <windows.h>

using namespace std;

enum Cell : char
{
    CROSS = 'X',
    ZERO = '0',
    EMPTY = '_'
};

enum Progress
{
    IN_PROGRESS,
    COMPUTER_WON,
    HUMAN_WON,
    DRAW
};

struct Game
{
    Cell** field;
    int size = 3;
    Cell human = CROSS;
    Cell computer = ZERO;
    int turn = 1; //нечетный, значит ходит человек
    Progress progress = IN_PROGRESS;
};

struct Coord
{
    int y;
    int x;
};

void clearScreen() {
    system("CLS");
}

int32_t getRandomNum(int32_t min, int32_t max) {

    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

void initGame(Game& game, int size) {

    game.size = size;

    game.field = new Cell * [game.size];

    for (size_t i = 0; i < game.size; i++)
    {
        game.field[i] = new Cell[game.size];
    }

    for (size_t y = 0; y < game.size; y++)
    {
        for (size_t x = 0; x < game.size; x++)
        {
            game.field[y][x] = EMPTY;
        }
    }

    srand(time(NULL));

    if (getRandomNum(0, 1000) > 500)
    {
        game.turn = 1;
        game.human = CROSS;
        game.computer = ZERO;
        

    }
    else
    {       
        game.turn = 2;
        game.computer = CROSS;
        game.human = ZERO;
        
    }

}

void printGame(const Game& game) {

    if (game.size == 3)
    {
        cout << "  1 2 3" << endl;
    }
    else
    {
        cout << "  1 2 3 4" << endl;
    }

    for (size_t y = 0; y < game.size; y++)
    {
        cout << y + 1 << "|";
        for (size_t x = 0; x < game.size; x++)
        {
            cout << game.field[y][x] << '|';
        }
        cout << endl;
    }

    if (game.human == CROSS)
    {
        cout << "Human - X, Computer - 0" << endl;
    }
    else
    {
        cout << "Computer - X, Human - 0" << endl;
    }
}

Coord takeHumanCoord(Game& game) {
    int x, y;

    do {
        cout << "Input x: " << endl;
        cin >> x;
        cout << "Input y: " << endl;
        cin >> y;
        
    } while (x < 0 || x > game.size || y < 0 || y > game.size || game.field[y - 1][x - 1] != EMPTY);

    return { y-1, x-1 };
}

bool checkWin(Game& game) {
    int key = 0;
    size_t i, j;

    //по горизонтали
    for (i = 0; i < game.size; i++) {

        key = 0;
        for (j = 0; j < game.size-1; j++) {
            if (game.field[i][j] == game.field[i][j + 1] && game.field[i][j] != EMPTY)
                key++;
        }

        if (key == game.size - 1)
            return true;
    }

    //по вертикали
    for (j = 0; j < game.size; j++) {

        key = 0;
        for (i = 0; i < game.size - 1; i++) {
            if (game.field[i][j] == game.field[i+1][j] && game.field[i][j] != EMPTY)
                key++;
        }

        if (key == game.size - 1)
            return true;
    }

    //по одной диагонали
    key = 0;
    for (j = 1; j < game.size; j++)
    {
        if (game.field[0][0] == game.field[j][j] && game.field[0][0] != EMPTY)
        {
            key++;
        }
    }
    if (key == game.size - 1)
        return true;
    
    //по другой диагонали
    key = 0;
    j = game.size - 1;
    for (i = 1; i < game.size; i++)
    {
        if (game.field[0][game.size - 1] == game.field[i][--j] && game.field[0][game.size - 1] != EMPTY)
        {
            key++;
            
        }
        
    }
    if (key == game.size - 1)
        return true;

    return false;
}

Progress getWin(Game& game) {

    if (checkWin(game))
    {
        if (game.turn % 2 == 0)
            return HUMAN_WON;
        else
            return COMPUTER_WON;
    }
   
    for (size_t i = 0; i < game.size; i++)
    {
        for (size_t j = 0; j < game.size; j++)
        {
            if (game.field[i][j] == EMPTY)
                return IN_PROGRESS;
        }
    }

    return DRAW;
}

Coord takeComputerCoord(Game& game) {

    //computer preWin situation
    for (int y = 0; y < game.size; y++)
    {
        for (int x = 0; x < game.size; x++)
        {
            if (game.field[y][x] == EMPTY)
            {
                game.field[y][x] = game.computer;

                if (checkWin(game))
                {
                    game.field[y][x] = EMPTY;
                    return { y, x };
                }
                else
                {
                    game.field[y][x] = EMPTY;
                }
            }

        }
    }

    //human preWin situation
    for (int y = 0; y < game.size; y++)
    {
        for (int x = 0; x < game.size; x++)
        {
            if (game.field[y][x] == EMPTY)
            {
                game.field[y][x] = game.human;

                if (checkWin(game))
                {
                    game.field[y][x] = EMPTY;
                    return { y, x };
                }
                else
                {
                    game.field[y][x] = EMPTY;
                }
            }

        }
    }

    if (game.size == 3)
    {
        Coord arr[4];
        size_t num = 0;

        //середина
        if (game.field[1][1] == EMPTY)
            return { 1, 1 };

        //углы
        if (game.field[0][0] == EMPTY)
        {
            arr[num] = { 0,0 }; 
            num++;
        }

        if (game.field[0][2] == EMPTY)
        {
            arr[num] = { 0, 2 };
            num++;
        }

        if (game.field[2][0] == EMPTY)
        {
            arr[num] = { 2, 0 };
            num++;
        }

        if (game.field[2][2] == EMPTY)
        {
            arr[num] =  { 2, 2 };
            num++;
        }

        if (num > 0)
        {
            size_t index = getRandomNum(0, 1000) % num;
            return arr[index];
        }

        //оставшиеся клетки
        if (game.field[0][1] == EMPTY)
        {
            arr[num] = { 0, 1 };
            num++;
        }

        if (game.field[1][0] == EMPTY)
        {
            arr[num] = { 1, 0 };
            num++;
        }

        if (game.field[1][2] == EMPTY)
        {
            arr[num] = { 1, 2 };
            num++;
        }

        if (game.field[2][1] == EMPTY)
        {
            arr[num] = { 2, 1 };
            num++;
        }

        if (num > 0)
        {
            size_t index = getRandomNum(0, 1000) % num;
            return arr[index];
        }

    }
    //размер 4
    else 
    {
        Coord arr[4];
        size_t num = 0;

        //центральные 4 клетки
        if (game.field[1][1] == EMPTY)
        {
            arr[num] = { 1,1 };
            num++;
        }

        if (game.field[2][1] == EMPTY)
        {
            arr[num] = { 2,1 };
            num++;
        }

        if (game.field[1][2] == EMPTY)
        {
            arr[num] = { 1, 2 };
            num++;
        }

        if (game.field[2][2] == EMPTY)
        {
            arr[num] = { 2, 2 };
            num++;
        }

        if (num > 0)
        {
            size_t index = getRandomNum(0, 1000) % num;
            return arr[index];
        }

        //углы
        if (game.field[0][0] == EMPTY)
        {
            arr[num] = { 0,0 };
            num++;
        }

        if (game.field[0][3] == EMPTY)
        {
            arr[num] = { 0,3 };
            num++;
        }

        if (game.field[3][0] == EMPTY)
        {
            arr[num] = { 3, 0 };
            num++;
        }

        if (game.field[3][3] == EMPTY)
        {
            arr[num] = { 3, 3 };
            num++;
        }

        if (num > 0)
        {
            size_t index = getRandomNum(0, 1000) % num;
            return arr[index];
        }

        //оставшиеся клетки
        Coord arr2[8];

        if (game.field[0][1] == EMPTY)
        {
            arr2[num] = { 0,1 };
            num++;
        }

        if (game.field[0][2] == EMPTY)
        {
            arr2[num] = { 0,2 };
            num++;
        }

        if (game.field[1][0] == EMPTY)
        {
            arr2[num] = { 1, 0 };
            num++;
        }

        if (game.field[1][3] == EMPTY)
        {
            arr2[num] = { 1, 3 };
            num++;
        }

        if (game.field[2][0] == EMPTY)
        {
            arr2[num] = { 2,0 };
            num++;
        }

        if (game.field[2][3] == EMPTY)
        {
            arr2[num] = { 2,3 };
            num++;
        }

        if (game.field[3][1] == EMPTY)
        {
            arr2[num] = { 3, 1 };
            num++;
        }

        if (game.field[3][2] == EMPTY)
        {
            arr2[num] = { 3, 2 };
            num++;
        }

        if (num > 0)
        {
            size_t index = getRandomNum(0, 1000) % num;
            return arr2[index];
        }

    }
 
}

void printResult(Game& game) {

    switch (game.progress)
        {
        case COMPUTER_WON:
            cout << "Computer won!" << endl;
            break;
        case HUMAN_WON:
            cout << "Human won!" << endl;
            break;
        case DRAW:
            cout << "Draw!" << endl;
        }
}

int main()
{
    Game game;
    int size;
    do
    {
        cout << "Input game size: 3 or 4" << endl;
        cin >> size;
    } while (size != 3 && size != 4);
    
    initGame(game, size);
    Coord tempCoord;

    clearScreen();
    printGame(game);

    do
    {
        
        if (game.turn % 2 == 0)
        {
            Sleep(1000);
            tempCoord = takeComputerCoord(game);
            game.field[tempCoord.y][tempCoord.x] = game.computer;
            game.turn++;
        }
        else
        {
            tempCoord = takeHumanCoord(game);
            game.field[tempCoord.y][tempCoord.x] = game.human;
            game.turn++;
        }

        game.progress = getWin(game);

        clearScreen();
        printGame(game);

    } 
    while (game.progress == IN_PROGRESS);

    printResult(game);

    return 0;
}
