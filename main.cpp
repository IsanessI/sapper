#include "functions.h"
#include <unordered_set>

int main()
{
	auto rechange = [](char& symb)
	{
		if (symb >= 'a' && symb <= 'z') symb -= 'a' - 'A';
	}; 
	console con;

	con.change_color(console::colors::White);
	con << "Welcome to the Sapper.\n";

	while (true)
	{
		time_t BEGIN = 0, END = 0;
		con.change_color(console::colors::White);

		con.game_over = 0;
		con << "Play a game - 'Y'\nCheck table of records - 'T'\nExit - ESC\n";
		char choose;
		do
		{
			choose = con.getch();
			rechange(choose);
		} while (choose != 'Y' && choose != 'T' && choose != VK_ESCAPE);
		if (choose == VK_ESCAPE)
		{
			Exit(con);
			exit(1);
		}
		if (choose == 'T')
		{
			records rec(con);
			con << rec;
			continue;
		}

		field FIELD;
		FIELD.change_level(con);
		
		start(con, FIELD);
		BEGIN = clock();
		while(true)
		{
			con.clear();

			if (FIELD)
			{
				con.game_over = 1;
				END = clock();
				con.clear();
				con << "Conrgats! You win!\n" << FIELD;
				con.change_color(console::colors::White);

				con.getch();
				con.clear();
				
				records rec(con);
				rec.put(con, FIELD.diff, (END - BEGIN) / CLOCKS_PER_SEC);
				break;
			}
			con << FIELD;
			con.set_position(FIELD.coords());

			FIELD.move(con);
			if (FIELD.the_end) 
				break;
		}
		con.clear();
	}
}