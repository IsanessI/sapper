#pragma once
#include <vector>
#include <Windows.h>
#include "console.h"

class field
{
	friend class records;
	friend class cell;
private:
	class cell
	{
	public:
		int value_of_mine;
		bool mine, flag, dig_out;
		
		friend console& operator<<(console& con, const cell& CELL) {
			if (con.game_over)
			{
				if (CELL.mine)
				{
					con.change_color(console::colors::DarkRed);
					con << '*';
					return con;
				}
				if (CELL.flag)
				{
					con.change_color(console::colors::DarkMagenta);
					con << '\2';
					return con;
				}
			}
			if (CELL.flag)
			{
				con.change_color(console::colors::Cyan);
				con << 'F';
				return con;
			}
			if (CELL.dig_out)
			{
				switch (CELL.value_of_mine)
				{
				case 0:
					con.change_color(console::colors::Gray);
					break;
				case 1:
					con.change_color(console::colors::Blue);
					break;
				case 2:
					con.change_color(console::colors::Green);
					break;
				case 3:
					con.change_color(console::colors::Red);
					break;
				case 4:
					con.change_color(console::colors::Magenta);
					break;
				case 5:
					con.change_color(console::colors::Yellow);
					break;
				case 6:
					con.change_color(console::colors::DarkMagenta);
					break;
				case 7:
					con.change_color(console::colors::DarkYellow);
					break;
				case 8:
					con.change_color(console::colors::DarkGreen);
					break;
				}
				con << CELL.value_of_mine;
			}
			else
			{
				con.change_color(console::colors::White);
				con << '#';
				return con;
			}

			return con;
		}
	};
	
	std::vector<std::vector<cell>> f;
	int line_size, col_size;
	COORD pt;
	int amount_of_mines;
	bool first_dig;
public:
	char diff;
	int amount_of_flags;
	bool the_end;

	//overloads
	friend console& operator<<(console&, const field&); //cout.cpp
	std::vector<cell>& operator[](int pos)
	{
		return f[pos];
	}
	const std::vector<cell>& operator[](int pos) const
	{
		return f[pos];
	}
	const cell& curCell() const
	{
		return f[pt.Y][pt.X];
	}
	cell& curCell()
	{
		return f[pt.Y][pt.X];
	}
	
	//low-code functions
	COORD& coords()
	{
		return pt;
	}
	const COORD coords() const
	{
		return pt;
	}
	int x_size()
	{
		return line_size;
	}
	int y_size()
	{
		return col_size;
	}

	//large functions
	void generate();
	void bypass_in_breadth();

	void resize(int, int);
	void change_level(console&);

	void move(console&);

	operator bool() const
	{
		if (line_size == 0 || col_size == 0 || !first_dig) return false;
		bool f = 1;
		for (int i = 0; i < col_size; i++)
		{
			for (int j = 0; j < line_size; j++)
			{
				if (!(*this)[i][j].mine && !(*this)[i][j].dig_out)
					f = 0;
				if (!f) break;
			}
			if (!f) break;
		}
		return f;
	}
};