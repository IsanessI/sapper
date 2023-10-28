#include "functions.h"

void start(console& con, field& FIELD)
{
    con << "Game stated in... \n";

    con.sleep(1000);
    con << "3\n";

    con.sleep(1000);
    con << "2\n";

    con.sleep(1000);
    con << "1\n";

    con.sleep(1000);
}
void Exit(console& con)
{
    con << ("Game was created by SANESS.");
    con.change_color(console::colors::Black);\
}
