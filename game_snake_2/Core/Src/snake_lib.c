#include"snake_lib.h"
void start_game()
{
    N5110_Clear();
    N5110_SetFont(SmallFont);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*6), N5110_MAX_HEIGHT/2 - 4, "NOKIA5110", true);
    N5110_Update();
	HAL_Delay(2000);

    N5110_Clear();
    N5110_SetFont(SmallFont);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*6), N5110_MAX_HEIGHT/2 - 4, "GAME SNAKE", true);
    N5110_Update();
	HAL_Delay(2000);
}

void snake(int x, int y)
{
	N5110_DrawCircle(x,y,1,true,false);
}

void egg(int x, int y)
{
	N5110_DrawRect(x-1,y-1,x+1,y+1,true,true);
}
