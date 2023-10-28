#include "field.h"
#include "console.h"
#include <queue>

#define fr first
#define sc second

void field::move(console& con)
{
    auto rechange = [](char& symbol)
    {
        if (symbol >= 'a' && symbol <= 'z') symbol += 'A' - 'a';
        return symbol;
    };


    char symbol = con.getch();

    if (symbol == -32) // признак escape последовательности, означает что был введён не символ а спец клавиша, код которой будет прочтён следующим _getch()
    {
        symbol = con.getch();
        if (symbol == 75) // влево
        {
            if (pt.X)
                pt.X--;
        }
        else if (symbol == 72)  // вверх
        {
            if (pt.Y)
                pt.Y--;
        }
        else if (symbol == 77) // вправо
        {
            if (pt.X < line_size - 1)
                pt.X++;
        }
        else if (symbol == 80) // вниз
        {
            if (pt.Y < col_size - 1)
                pt.Y++;
        }

        con.set_position(pt);
    }

    rechange(symbol);
    if (symbol == VK_ESCAPE)
    {
        the_end = 1;
        return;
    }
    else if (symbol == 13)
    {
        if (!first_dig)
        {
            generate();
            first_dig = 1;
        }

        if (curCell().flag)
            return;

        else if (curCell().mine)
        {
            con.game_over = 1;
            con.clear();
            con << "You lose!\n" << *this;
            con.getch();

            the_end = 1;
            return;
        }
        else
            bypass_in_breadth();
    }
    else if (symbol == 'F')
    {
        if (!first_dig)
            return;

        curCell().flag = curCell().flag ? 0 : 1;
        if (curCell().flag && amount_of_flags)
            amount_of_flags--;
        else
        {
            curCell().flag = 0;
            amount_of_flags++;
        }
    }

}

console& operator<<(console& con, const field& FIELD)
{
    for (int i = 0; i < FIELD.col_size; i++)
    {
        for (int j = 0; j < FIELD.line_size; j++)
            con << FIELD[i][j];
        con << '\n';
    }

    if (!con.game_over)
    {
        con.change_color(console::colors::White);
        con << "Amount of flags: " << FIELD.amount_of_flags;
    }
    con.set_position(FIELD.coords());

    return con;
}

void field::change_level(console& con)
{
    auto rechange = [](char& symbol)
    {
        if (symbol >= 'a' && symbol <= 'z') symbol += 'A' - 'a';
        return symbol;
    };

    con << "Enter difficult of game : \nEasy - E, Normal - N, Hard - H, Custom - C.\n";

    char difficult;

    do
    {
        difficult = con.getch();
        rechange(difficult);
    } while (difficult != 'E' && difficult != 'N' && difficult != 'H' && difficult != 'C');

    switch (difficult)
    {
    case 'E':

        line_size = rand() % 5 + 5;
        col_size = rand() % 5 + 5;
        amount_of_mines = rand() % 6 + 6;

        break;

    case 'N':

        line_size = rand() % 3 + 10;
        col_size = rand() % 3 + 10;
        amount_of_mines = rand() % 10 + rand() % 10 + 10;

        break;
    case 'H':

        line_size = rand() % 3 + 10;
        col_size = rand() % 3 + 10;
        amount_of_mines = rand() % 10 + rand() % 10 + 30;
        break;

    case 'C':

        con << ("Please, type the dimensions of the field size NxM:\n");

        con >> line_size >> col_size;

        while (line_size > 100 || line_size < 1 || col_size > 100 || col_size < 1)
        {
            con << "Error! Try again: ";
            con >> line_size >> col_size;
        }
        con << "Enter a amount of mines:\n";

        con >> amount_of_mines;

        while (amount_of_mines > line_size * col_size)
        {
            con << "Incorrect value. Please, try again.\n";
            con >> amount_of_mines;
        }
        break;
    }

    diff = difficult;
    amount_of_flags = amount_of_mines;
    resize(line_size, col_size);
}
void field::resize(int x_size, int y_size)
{
    if (!f.empty()) f.clear();
    f.resize(y_size + 1);
    for (int i = 0; i <= y_size; i++)
        f[i].resize(x_size + 1);
}

void Queue(std::queue<std::pair<int, int>>&, field&);

void field::generate()
{
    int amount = amount_of_mines;
    while (amount != 0)
    {
        for (int i = 0; i < col_size && amount > 0; i++)
            for (int j = 0; j < line_size && amount > 0; j++)
            {
                if (i == pt.Y && j == pt.X) continue;

                int xx = rand() % (line_size * col_size);
                if (xx == rand() % (line_size * line_size))
                {
                    (*this)[i][j].mine = 1;
                    (*this)[i][j].value_of_mine = 0;
                    amount--;
                }
            }

    }

    for (int i = 0; i < col_size; i++)
        for (int j = 0; j < line_size; j++)
            if (f[i][j].mine)
            {
                const std::pair <int, int> check[8] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
                for (int t = 0; t < 8; t++)
                    if (i + check[t].fr >= 0 && j + check[t].sc >= 0)
                        f[i + check[t].fr][j + check[t].sc].value_of_mine++;
            }
}


void field::bypass_in_breadth() //CAN BE BROKEN
{
    if (f[pt.Y][pt.X].flag || f[pt.Y][pt.X].dig_out)
        return;
    std::queue <std::pair <int, int> > q;

    f[pt.Y][pt.X].dig_out = 1;
    if (f[pt.Y][pt.X].value_of_mine > 0)
        return;

    q.push({ pt.Y, pt.X });
    while (!q.empty())
        Queue(q, *this);
}
void Queue(std::queue<std::pair<int, int>>& q, field& FIELD)
{
    const std::pair <int, int> check[8] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    int x1 = q.front().fr;
    int yy1 = q.front().sc;
    FIELD[x1][yy1].dig_out = 1;
    q.pop();
    for (int i = 0; i < 8; i++)
    {
        int xx = check[i].fr;
        int yy = check[i].sc;
        if (x1 + xx >= 0 && x1 + xx < FIELD.x_size() && yy1 + yy >= 0 && yy1 + yy < FIELD.y_size())
        {
            if (FIELD[x1 + xx][yy1 + yy].dig_out || FIELD[x1 + xx][yy1 + yy].flag)
                continue;
            if (FIELD[x1 + xx][yy1 + yy].value_of_mine >= 1)
            {
                FIELD[x1 + xx][yy1 + yy].dig_out = 1;
                continue;
            }
            else
            {
                FIELD[x1 + xx][yy1 + yy].dig_out = 1;
                q.push({ x1 + xx, yy1 + yy });
                continue;
            }
        }
        else
            continue;
    }
}
