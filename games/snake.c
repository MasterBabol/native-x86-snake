
// Snake.cpp : Defines the entry point for the console application.
//
#define BOOL int
#define TRUE 1
#define FALSE 0

void InitSnakeInfo ();
void DrawBoard ();
void InitApples ();

int MAX_APPLE = 4;
#define MAX_TAIL 256
short SnakeInfo[MAX_TAIL];
short AppleInfo[256];
int TailCount, DirX, DirY;
int AppleCount;
#define WIN 1
#define DEFEAT 2

int snakemain()
{
	MAX_APPLE = 4;
game_start:
	InitSnakeInfo ();
	DrawBoard ();
	InitApples ();

	text_out (*((char *)SnakeInfo), *(((char *)SnakeInfo)+1), "O", 7);
	BOOL fExit = FALSE;
	int fResult = 0;
	while (!fExit)
	{
		if (kbhit())
		{
			char c = getch ();
			switch (c)
			{
			case 'a':
			case 'A':
				DirX = -1;
				DirY = 0;
				break;
			case 's':
			case 'S':
				DirX = 0;
				DirY = 1;
				break;
			case 'd':
			case 'D':
				DirX = 1;
				DirY = 0;
				break;
			case 'w':
			case 'W':
				DirX = 0;
				DirY = -1;
				break;
			case 'q':
			case 'Q':
				return 0;
			}
		}
		int i = TailCount;
		if (*((char *)&SnakeInfo[i]) != 0 && *(((char *)&SnakeInfo[i])+1) != 0)
		text_out (*((char *)&SnakeInfo[i]), *(((char *)&SnakeInfo[i])+1), " ", 7);

		char temp1 = *((char *)SnakeInfo);
		char temp2 = *(((char *)SnakeInfo)+1);
		*((char *)SnakeInfo) += DirX;
		*(((char *)SnakeInfo)+1) += DirY;
int sc=1;
		for (;sc <= TailCount;sc++)
		{
			if (temp1 == *((char *)&SnakeInfo[sc]) && temp2 == *(((char *)&SnakeInfo[sc])+1))
			{
				fExit = TRUE;
				fResult = DEFEAT;
				break;
			}
		}
		*((char *)SnakeInfo) = temp1;
		*(((char *)SnakeInfo)+1) = temp2;
		for (; i > 0; i--)
		{
			SnakeInfo[i] = SnakeInfo[i-1];
		}
		*((char *)SnakeInfo) += DirX;
		*(((char *)SnakeInfo)+1) += DirY;

		if (*((char *)SnakeInfo) < 1 || *((char *)SnakeInfo) > 78
			|| *(((char *)SnakeInfo)+1) < 1 || *(((char *)SnakeInfo)+1) > 22)
		{
			fExit = TRUE;
			fResult = DEFEAT;
			break;
		}
int ac=0;
		for (;ac < MAX_APPLE;ac++)
		{
			if (AppleCount == MAX_APPLE)
			{
				fExit = TRUE;
				fResult = WIN;
				break;
			}
			if (*((char *)SnakeInfo) == *((char *)&AppleInfo[ac]) && *(((char *)SnakeInfo)+1) == *(((char *)&AppleInfo[ac])+1))
			{
				AppleCount++;
				TailCount++;
				*(((char *)&AppleInfo[ac])) = *(((char *)&AppleInfo[ac])+1) = 0;
			}
		}
		
		text_out (*((char *)SnakeInfo), *(((char *)SnakeInfo)+1), "O", 7);
		delay (10);
	}
	if (fResult == WIN)
	{
		MAX_APPLE += 2;
		if (MAX_APPLE > 20)
		{
			text_out (10, 10, "all Stage Clear !", 7);
			delay (100);
			return 0;
		}
		text_out (10, 10, "To Next Stage !", 7);
	}
	else if (fResult == DEFEAT)
	{
		text_out (10, 10, "dead !", 7);
	}
	delay (100);
	goto game_start;
	return 0;
}

void InitSnakeInfo ()
{
	TailCount = 0;
	DirX = 0;
	DirY = 0;
	AppleCount = 0;
	*((char *)SnakeInfo) = 30;
	*(((char *)SnakeInfo)+1) = 12;
int i=1;
	for (;i < MAX_TAIL;i++)
		SnakeInfo[i] = 0;
}

void DrawBoard ()
{
int x=0;
int y=1;
	for (;x < 80;x++)
		text_out (x, 0, "@", 5);
	for (;y < 23;y++)
	{
		text_out (0, y, "@", 5);
		x=1;
		for (;x < 78;x++)
			text_out (x, y, " ", 7);
		text_out (79, y, "@", 5);
	}
int x2=0;
	for (;x2 < 80;x2++)
		text_out (x2, 23, "@", 5);
	text_out (0, 24, "W - up / A - left / S - down / D - right / Q - quit", 5);
}

void InitApples ()
{
int i=0;
	for (;i<MAX_APPLE;i++)
	{
		*((char*)&AppleInfo[i]) = gettickcount () % 70 + 2;
		delay (1);
		*(((char*)&AppleInfo[i])+1) = gettickcount () % 22 + 1;
		text_out (*(char*)&AppleInfo[i], *(((char*)&AppleInfo[i])+1), "*", 5);
	}
}

