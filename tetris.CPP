/**************************************************************************

I made this game to demonstrate to you the power of graphics.h 
This game contains fast and smooth animations with great graphics and
sounds.   Feel free to use this code.  If you like this
program (or) if you find it useful,  please take your time
and vote for me. Your vote will be really appreciated.
	 Thanks... ENJOY

Note : This code compiles succesfully in Turbo C 3.0

*************************************************************************/

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

//Colors
#define BLUE 1
#define BLUEDR 2
#define BLUEBR 3
#define RED 4
#define REDDR 5
#define REDBR 6
#define GREEN 7
#define GREENDR 8
#define GREENBR 9
#define PURPLE 10
#define PURPLEDR 11
#define PURPLEBR 12
#define YELLOW 13
#define YELLOWDR 14
#define YELLOWBR 15
#define BLANK 0
#define REFRESH 0
#define ROWS 20
#define COLS 15

// Shapes
#define SHAPE_I  100
#define SHAPE_T  101
#define SHAPE_L  102
#define SHAPE_L2 103
#define SHAPE_O  104
#define SHAPE_S  105
#define SHAPE_S2 106

// Directions
#define RIGHT 201
#define DOWN  203
#define TOP   30
#define LEFT  160

// Keys
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_SPACE 32



//Functions
void Brick(int x, int y, char Color);
void InitPalette();
void InitScreenLayout();
void GetImages();    // Draws images and saves them in memory for later use
void DisplayInterface();
void AssignShape(int Shape, int Color); // Gives the shape to the current falling block
int  DetectCollision(int Direction);	// returns 1 when brick collides with something
int  MoveBlock(int Direction);  //Moves the falling block in the direction
void DisplayBlock(int x, int y);
void DisplayNextShape();
void GetNextBlock();
void RotateBlock();
void UpdateScreen();
void CheckForLine();
void RemoveLine(int Number);
int  GetRandomColor();
int  GetRandomShape();
void PrintScore();
void PrintSpeed();
void PrintLevel();
void PrintLinesCleared();
void DrawBox(int x1, int y1, int x2, int y2, int Color1, int Color2, int Color3);
void SoundDrop();
void SplashScreen();
int  IncreaseSpeed();
int  ShowGameOver();
char MessageBox(char *Message, int Width=240, int Size=3);

// Bitmaps
void far *bmpBlueBrick;
void far *bmpRedBrick;
void far *bmpGreenBrick;
void far *bmpYellowBrick;
void far *bmpPurpleBrick;
void far *bmpBlankBrick;
void far *bmpScore;
void far *bmpSpeed;
void far *bmpLevel;
void far *bmpLinesCleared;
void far *bmpScreen;

int BlockMatrix[4][4];      // Stores the shape of the current falling block
int ScreenLayout[COLS][ROWS];	// Will contain the layout with the falling block
int ScreenBackgroundLayout[COLS][ROWS]; //will contain the layout without the falling block
int PreviousScreenLayout[COLS][ROWS];	// Will contain the layout of the previous screen
int LinesCleared=0;
int Level=1;
int LineScore=100;
int Speed = 95;
int CurrentShape;
int NextShape;
int NextColor;
int BlockX = 7;
int BlockY = 0;
int Quit=0;
int GameOver=0;
int SoundOn=1;
char lpszScore[40];
char lpszSpeed[40];
char lpszLevel[40];
char lpszLinesCleared[40];
unsigned long Score=0;

//Shapes of the blocks
int ShapeI[4][4] = {
		 0,1,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 0,1,0,0
	};
int ShapeT[4][4] = {
		 0,1,0,0,
		 0,1,1,0,
		 0,1,0,0,
		 0,0,0,0
	};
int ShapeL[4][4] = {
		 0,0,1,0,
		 1,1,1,0,
		 0,0,0,0,
		 0,0,0,0
	};
int ShapeL2[4][4] = {
		 1,0,0,0,
		 1,1,1,0,
		 0,0,0,0,
		 0,0,0,0
	};
int ShapeS[4][4] = {
		 0,1,1,0,
		 1,1,0,0,
		 0,0,0,0,
		 0,0,0,0
	};
int ShapeS2[4][4] = {
		 1,1,0,0,
		 0,1,1,0,
		 0,0,0,0,
		 0,0,0,0
	};
int ShapeO[4][4] = {
		 1,1,0,0,
		 1,1,0,0,
		 0,0,0,0,
		 0,0,0,0
	};


int main() {
	int gd=DETECT, gm;
	int Return=0;
	char Key, ScanCode;
	int Counter=0;
	initgraph(&gd, &gm, "c:\\borlandc\\bgi");
	randomize();
	cleardevice();
	InitPalette();
	InitScreenLayout();
	GetImages();
	SplashScreen();
	cleardevice();
	AssignShape(GetRandomShape(), GetRandomColor());
	NextShape=GetRandomShape();
	NextColor=GetRandomColor();
	DisplayInterface();
	DisplayNextShape();
	MoveBlock(LEFT);
	while(kbhit()) getch();   //empty the keyboard buffer
	while (!Quit && !GameOver) {
		if(++Counter >= Speed) {
			Counter=0;
			MoveBlock(DOWN);
			SoundDrop();
		}
		if(kbhit()) {
		   Key = getch();
		   if(Key == 0) {
			   ScanCode = getch();
			   if(ScanCode == KEY_UP)
					RotateBlock();
			   else if(ScanCode == KEY_LEFT)
					Return=MoveBlock(LEFT);
			   else if(ScanCode == KEY_RIGHT)
					Return=MoveBlock(RIGHT);
			   else if(ScanCode == KEY_DOWN) {
					Score++;
					PrintScore();
					MoveBlock(DOWN);
			   }
			   if(!Return)
				   SoundDrop();
			   Return = 0;
		   }
		   else if(Key == KEY_ENTER || Key == KEY_SPACE)
				RotateBlock();
		   else if(Key == 'P' || Key == 'p') {
			  MessageBox("  Paused");
			  while(kbhit()) getch();  //clear the keyboard buffer
			  for(int x=0; x<COLS; x++)
				 for(int y=0; y<ROWS; y++)
					PreviousScreenLayout[x][y] = -1;  // Clear the previous screen layout to refresh the whole screen
			  UpdateScreen();
		   }
		   else if(Key == KEY_ESC) {
			  char ret = MessageBox("Are you sure you want to Quit?", 550, 2);
			  if(ret == 'y' || ret == 'Y' || ret == KEY_ENTER) {
				  Quit = 1;
				  break;
			  }
			  cleardevice();
			  while(kbhit()) getch();  //clear the keyboard buffer
			  for(int x=0; x<COLS; x++)
				 for(int y=0; y<ROWS; y++)
					PreviousScreenLayout[x][y] = -1;  // Clear the previous screen layout to refresh the whole screen
			  UpdateScreen();
			  DisplayInterface();
		   }
		   else if(Key == 's' || Key == 'S') {
			  SoundOn = !SoundOn;
		   }
		}
		delay(5);
	}
	if(GameOver) {
		DisplayBlock(6,0);
		ShowGameOver();
	}
	restorecrtmode();
	return 0;
}

void Brick(int x, int y, char Color) {
	int utri[6] = { x,y,  x+20,y,  x,y+20 };
	int ltri[6] = { x+20,y+20,  x+20,y,  x,y+20 };
	switch(Color) {
	case BLUE:
		setcolor(BLUEBR);
		setfillstyle(1,BLUEBR);
		fillpoly(3, utri);
		setcolor(BLUEDR);
		setfillstyle(1,BLUEDR);
		fillpoly(3, ltri);
		setcolor(BLUE);
		setfillstyle(1,BLUE);
		bar(x+5, y+5, x+15, y+15);
		break;
	case RED:
		setcolor(REDBR);
		setfillstyle(1,REDBR);
		fillpoly(3, utri);
		setcolor(REDDR);
		setfillstyle(1,REDDR);
		fillpoly(3, ltri);
		setcolor(RED);
		setfillstyle(1,RED);
		bar(x+5, y+5, x+15, y+15);
		break;
	case GREEN:
		setcolor(GREENBR);
		setfillstyle(1,GREENBR);
		fillpoly(3, utri);
		setcolor(GREENDR);
		setfillstyle(1,GREENDR);
		fillpoly(3, ltri);
		setcolor(GREEN);
		setfillstyle(1,GREEN);
		bar(x+5, y+5, x+15, y+15);
		break;
	case PURPLE:
		setcolor(PURPLEBR);
		setfillstyle(1,PURPLEBR);
		fillpoly(3, utri);
		setcolor(PURPLEDR);
		setfillstyle(1,PURPLEDR);
		fillpoly(3, ltri);
		setcolor(PURPLE);
		setfillstyle(1,PURPLE);
		bar(x+5, y+5, x+15, y+15);
		break;
	case YELLOW:
		setcolor(YELLOWBR);
		setfillstyle(1,YELLOWBR);
		fillpoly(3, utri);
		setcolor(YELLOWDR);
		setfillstyle(1,YELLOWDR);
		fillpoly(3, ltri);
		setcolor(YELLOW);
		setfillstyle(1,YELLOW);
		bar(x+5, y+5, x+15, y+15);
		break;
	}
}

void InitPalette() {
	//Blue Colors
	setpalette(BLUE,1);  //Normal
	setpalette(BLUEDR,8);  //Dark
	setpalette(BLUEBR,9);  //Bright
	//Red Colors
	setpalette(RED,4);  //Normal
	setpalette(REDDR,32);  //Dark
	setpalette(REDBR,36);  //Bright
	//Green Colors
	setpalette(GREEN,2);  //Normal
	setpalette(GREENDR,16);  //Dark
	setpalette(GREENBR,18);  //Bright
	//Purple Colors
	setpalette(PURPLE,13);  //Normal
	setpalette(PURPLEDR,33);  //Dark
	setpalette(PURPLEBR,61);  //Bright
	//Yellow Colors
	setpalette(YELLOW,6);  //Normal
	setpalette(YELLOWDR,48);  //Dark
	setpalette(YELLOWBR,62);  //Bright
}

void GetImages() {

	Brick(100,100,BLUE);
	Brick(130,100,RED);
	Brick(160,100,GREEN);
	Brick(190,100,YELLOW);
	Brick(220,100,PURPLE);

	bmpBlueBrick   = malloc(imagesize(0,0,20,20));
	bmpRedBrick    = malloc(imagesize(0,0,20,20));
	bmpGreenBrick  = malloc(imagesize(0,0,20,20));
	bmpYellowBrick = malloc(imagesize(0,0,20,20));
	bmpPurpleBrick = malloc(imagesize(0,0,20,20));
	bmpBlankBrick  = malloc(imagesize(0,0,20,20));
	bmpScore       = malloc(imagesize(501,51,607,69));
	bmpSpeed       = malloc(imagesize(501,51,607,69));
	bmpLevel       = malloc(imagesize(501,51,607,69));
	bmpLinesCleared= malloc(imagesize(501,51,607,69));
	bmpScreen      = malloc(imagesize(0,0,640,480));

	getimage(100,100,120,120,bmpBlueBrick);
	getimage(130,100,150,120,bmpRedBrick);
	getimage(160,100,180,120,bmpGreenBrick);
	getimage(190,100,210,120,bmpYellowBrick);
	getimage(220,100,240,120,bmpPurpleBrick);
	getimage(0,0,20,20,bmpBlankBrick);
	getimage(501,51,607,69,bmpScore);
	getimage(501,51,607,69,bmpSpeed);
	getimage(501,51,607,69,bmpLevel);
	getimage(501,51,607,69,bmpLinesCleared);
}


void InitScreenLayout() {
	for(int x=0; x<COLS; x++)
	   for(int y=0; y<ROWS; y++) {
		  ScreenLayout[x][y] = BLANK;
		  PreviousScreenLayout[x][y] = BLANK;
		  ScreenBackgroundLayout[x][y] = BLANK;
	   }
}

void DisplayInterface() {
	setcolor(15);
	rectangle(158,28,476,450);
	DrawBox(488,300,620,450, YELLOWBR, YELLOW, YELLOWDR);
	DrawBox(488,28,620,290, YELLOWBR, YELLOW, YELLOWDR);
	DrawBox(10,260,150,450, YELLOWBR, YELLOW, YELLOWDR);
	setcolor(GREEN);
	settextstyle(0,0,1);
	outtextxy(500,310, "Next Brick:");
	outtextxy(500,40, "Score:");
	rectangle(500,50,608,70);
	outtextxy(500,100, "Level:");
	rectangle(500,110,608,130);
	outtextxy(500,160, "Speed:");
	rectangle(500,170,608,190);
	outtextxy(500,220, "Lines Cleared:");
	rectangle(500,230,608,250);
	PrintScore();
	PrintSpeed();
	PrintLevel();
	PrintLinesCleared();
	setcolor(BLUEDR);
	settextstyle(10,1,4);
	outtextxy(0,40, "pHroZeN");
	outtextxy(60,60, "Bricks");
	setcolor(GREEN);
	settextstyle(7,0,2);
	outtextxy(20,265,"KEYS:");
	settextstyle(0,0,1);
	setcolor(PURPLE);
	outtextxy(20,310,"Left   :  < ");
	line(103,313,112,313);
	outtextxy(20,330,"Right  :   >");
	line(103,333,112,333);
	outtextxy(20,350,"Rotate :  ^");
	line(103,350, 103, 360);
	outtextxy(20,370,"Drop   :");
	line(103,370, 103, 380);
	setlinestyle(0,0,3);
	line(100,377, 103, 380);
	line(106,377, 103, 380);
	outtextxy(20,390,"Pause  :  P");
	outtextxy(20,410,"Sound  :  S");
	outtextxy(20,430,"Exit   :  Esc");
	setcolor(GREEN);
}

void AssignShape(int Shape, int Color) {
	int i,j;
	CurrentShape = Shape;
	switch(Shape) {
	case SHAPE_I:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeI[i][j]*Color;
		break;
	case SHAPE_T:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeT[i][j]*Color;
		break;
	case SHAPE_L:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeL[i][j]*Color;
		break;
	case SHAPE_L2:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeL2[i][j]*Color;
		break;
	case SHAPE_O:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeO[i][j]*Color;
		break;
	case SHAPE_S:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeS[i][j]*Color;
		break;
	case SHAPE_S2:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  BlockMatrix[j][i] = ShapeS2[i][j]*Color;
		break;
	default:
		break;
	}
}

int GetRandomColor() {
	int Color = rand() % 5;
	switch(Color) {
	case 0 :
	   return BLUE;
	case 1:
	   return RED;
	case 2:
	   return GREEN;
	case 3:
	   return YELLOW;
	case 4:
	   return PURPLE;
	}
	return 0;
}

int GetRandomShape() {
	int Shape = rand() % 7;
	switch(Shape) {
	case 0 :
	   return SHAPE_I;
	case 1:
	   return SHAPE_L;
	case 2:
	   return SHAPE_T;
	case 3:
	   return SHAPE_O;
	case 4:
	   return SHAPE_S;
	case 5:
	   return SHAPE_S2;
	case 6:
	   return SHAPE_L2;
	}
	return 0;
}

int MoveBlock(int Direction) {
	switch(Direction) {
	case LEFT:
		if(DetectCollision(LEFT))
			return 1;
		DisplayBlock(--BlockX, BlockY);
		break;
	case RIGHT:
		if(DetectCollision(RIGHT))
			return 1;
		DisplayBlock(++BlockX, BlockY);
		break;
	case DOWN:
		if(DetectCollision(DOWN)) {
			GetNextBlock();
			return 1;
		}
		DisplayBlock(BlockX, ++BlockY);
		break;
	case REFRESH:
		DisplayBlock(BlockX, BlockY);
		break;
	}
	return 0;
}

void RotateBlock() {
	// Code to rotate the falling block
	int TempBlockMatrix[4][4];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
		   TempBlockMatrix[i][j] = BlockMatrix[i][j];
	switch(CurrentShape) {
	case SHAPE_O:
		return;
	case SHAPE_L:
	case SHAPE_L2:
	case SHAPE_S:
	case SHAPE_S2:
	case SHAPE_T:
		BlockMatrix[0][0] = TempBlockMatrix[2][0];
		BlockMatrix[0][2] = TempBlockMatrix[0][0];
		BlockMatrix[2][2] = TempBlockMatrix[0][2];
		BlockMatrix[2][0] = TempBlockMatrix[2][2];

		BlockMatrix[0][1] = TempBlockMatrix[1][0];
		BlockMatrix[1][2] = TempBlockMatrix[0][1];
		BlockMatrix[2][1] = TempBlockMatrix[1][2];
		BlockMatrix[1][0] = TempBlockMatrix[2][1];
		break;
	case SHAPE_I:
		BlockMatrix[0][1] = TempBlockMatrix[1][0];
		BlockMatrix[1][0] = TempBlockMatrix[0][1];

		BlockMatrix[1][2] = TempBlockMatrix[2][1];
		BlockMatrix[2][1] = TempBlockMatrix[1][2];

		BlockMatrix[1][3] = TempBlockMatrix[3][1];
		BlockMatrix[3][1] = TempBlockMatrix[1][3];
		break;
	}
	if(DetectCollision(REFRESH)) {
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
			   BlockMatrix[i][j] = TempBlockMatrix[i][j];
		return;
	}
	MoveBlock(REFRESH);
}

void DisplayBlock(int x, int y) {
	int i, j;
	for(i=0; i<ROWS; i++)
	   for(j=0; j<COLS; j++)
		   ScreenLayout[j][i] = ScreenBackgroundLayout[j][i];

	for(i=0; i<4; i++) {
	   if((x+i)<0 || (x+i) > COLS)
		  continue;
	   for(j=0; j<4; j++) {
		   if((y+j)>ROWS)
				continue;
		   if(BlockMatrix[i][j] == 0)
				continue;
		   ScreenLayout[x+i][y+j] = BlockMatrix[i][j];
	   }
	}
	UpdateScreen();
}

int DetectCollision(int Direction) {
	int x,y;
	int Bx=BlockX;
	int By=BlockY;
	switch(Direction) {
	case LEFT:
		--Bx;
		break;
	case RIGHT:
		++Bx;
		break;
	case DOWN:
		By++;
		break;
	default:
		break;
	}

	// Left Boundry check
	if(Bx < 0) {
	   for(x=0; (x+Bx)<0; x++)
		  for(y=0; y<4; y++)
			 if(BlockMatrix[x][y]!=0)
				return 1;
	}
	// Right Boundry check
	if(Bx > COLS-4) {
	   for(x=Bx+3; x>=COLS; x--)
		  for(y=0; y<4; y++)
			 if(BlockMatrix[x-Bx][y]!=0)
				return 1;
	}
	// Bottom boundry check
	for(x=0; x<4; x++)
		  for(y=3; y>=0; y--) {
			 if((ScreenBackgroundLayout[Bx+x][By+y]!=BLANK &&
				 BlockMatrix[x][y]!=BLANK) ||
				 ((By+y)>=ROWS && BlockMatrix[x][y]!=BLANK))
				return 1;
	}

	return 0;
}

void GetNextBlock() {
	for(int x=0; x<4; x++)
	   for(int y=0; y<4; y++)
		  if(BlockMatrix[x][y] != BLANK)
			  ScreenBackgroundLayout[BlockX+x][BlockY+y] = BlockMatrix[x][y];
	CheckForLine();
	AssignShape(NextShape, NextColor);
	NextShape = GetRandomShape();
	NextColor = GetRandomColor();
	DisplayNextShape();
	BlockX = 7;
	BlockY = 0;
	if(MoveBlock(LEFT)) {
	   GameOver=1;
	}
}

void DisplayNextShape() {
	int NextTop = 340;
	int NextLeft = 520;
	int TempMatrix[4][4];
	int i,j;

	switch(NextShape) {
	case SHAPE_I:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeI[i][j]*NextColor;
		break;
	case SHAPE_T:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeT[i][j]*NextColor;
		break;
	case SHAPE_L:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeL[i][j]*NextColor;
		break;
	case SHAPE_L2:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeL2[i][j]*NextColor;
		break;
	case SHAPE_O:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeO[i][j]*NextColor;
		break;
	case SHAPE_S:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeS[i][j]*NextColor;
		break;
	case SHAPE_S2:
		for(i=0; i<4; i++)
		   for(j=0; j<4; j++)
			  TempMatrix[j][i] = ShapeS2[i][j]*NextColor;
		break;
	default:
		break;
	}

	for(int x=0; x<4; x++)
	   for(int y=0; y<4; y++) {
		  switch(TempMatrix[x][y]) {
		  case BLUE:
			putimage(NextLeft+x*21, NextTop+y*21, bmpBlueBrick, 0);
			break;
		  case RED:
			putimage(NextLeft+x*21, NextTop+y*21, bmpRedBrick, 0);
			break;
		  case GREEN:
			putimage(NextLeft+x*21, NextTop+y*21, bmpGreenBrick, 0);
			break;
		  case YELLOW:
			putimage(NextLeft+x*21, NextTop+y*21, bmpYellowBrick, 0);
			break;
		  case PURPLE:
			putimage(NextLeft+x*21, NextTop+y*21, bmpPurpleBrick, 0);
			break;
		  default:
			putimage(NextLeft+x*21, NextTop+y*21, bmpBlankBrick, 0);
		  }
	   }
}

void UpdateScreen() {
	for(int x=0; x<COLS; x++)
	   for(int y=0; y<ROWS; y++) {
		  if(PreviousScreenLayout[x][y] == ScreenLayout[x][y])
			  continue;   //No need to draw the same image again
		  PreviousScreenLayout[x][y] = ScreenLayout[x][y];
		  switch(ScreenLayout[x][y]) {
		  case BLUE:
			putimage(LEFT+x*21, TOP+y*21, bmpBlueBrick, 0);
			break;
		  case RED:
			putimage(LEFT+x*21, TOP+y*21, bmpRedBrick, 0);
			break;
		  case GREEN:
			putimage(LEFT+x*21, TOP+y*21, bmpGreenBrick, 0);
			break;
		  case YELLOW:
			putimage(LEFT+x*21, TOP+y*21, bmpYellowBrick, 0);
			break;
		  case PURPLE:
			putimage(LEFT+x*21, TOP+y*21, bmpPurpleBrick, 0);
			break;
		  default:
			putimage(LEFT+x*21, TOP+y*21, bmpBlankBrick, 0);
		  }
	   }
}

void SoundDrop() {
   if(SoundOn) {
	  for(int i=5; i<10; i++) {
		  sound(200*i);
		  delay(1);
		  nosound();
	  }
   }
}

void CheckForLine() {
	int RemoveLines[10];
	int Remove;
	int x,y;
	int factor=1;
	for(y=0; y<ROWS; y++) {
	   Remove=1;
	   for(x=0; x<COLS; x++) {
		  if(ScreenLayout[x][y] == BLANK)
			 Remove=0;
	   }
	   if(Remove) {
		  RemoveLine(y);
		  Score += LineScore*factor;
		  factor++;
		  PrintScore();
	   }
	}
	while (kbhit()) getch(); //Empty the keyboard buffer...
}

void RemoveLine(int LineNumber) {
	int i,j;
	int count=0;
	int Temp[COLS];
	for(i=0; i<COLS; i++) {
	   Temp[i]=ScreenLayout[i][LineNumber];
	}
	for(j=0; j<6; j++) {
		for(i=0; i<COLS; i++) {
		  ScreenLayout[i][LineNumber]=(count ? Temp[i] : BLANK);
		}
		count = !count;
		UpdateScreen();
		delay(50);
	}
	for(i=LineNumber; i>0; i--)
	   for(j=0; j<COLS; j++) {
		  ScreenBackgroundLayout[j][i] = ScreenBackgroundLayout[j][i-1];
	   }
	for(j=0; j<COLS; j++)
	   ScreenBackgroundLayout[j][0] = BLANK;
	for(i=0; i<COLS; i++)
	   for(j=0; j<ROWS; j++)
		  ScreenLayout[i][j] = ScreenBackgroundLayout[i][j];
	++LinesCleared;
	PrintLinesCleared();
	if(!(LinesCleared % 20))
	   IncreaseSpeed();
}

int IncreaseSpeed() {
	if(Speed-5 <= 0)
		return 1;
	Speed -= 5;  //increase the speed... lower the number, higher the speed
	Level++;
	PrintSpeed();
	PrintLevel();
	return 0;
}

void DrawBox(int x1, int y1, int x2, int y2, int Color1, int Color2, int Color3) {
	setlinestyle(0,0,1);
	setcolor(Color1);
	rectangle(x1,y1,x2,y2);
	setcolor(Color2);
	rectangle(x1+2,y1+2,x2-2,y2-2);
	setcolor(Color3);
	rectangle(x1+4,y1+4,x2-4,y2-4);
}

void PrintScore() {
	settextstyle(7,0,1);
	sprintf(lpszScore, "%11ld", Score);
	putimage(501,51, bmpScore,0);
	outtextxy(499,48,lpszScore);
}

void PrintLevel() {
	settextstyle(7,0,1);
	sprintf(lpszLevel, "%11d", Level);
	putimage(501,111, bmpLevel,0);
	outtextxy(499,108,lpszLevel);
}

void PrintSpeed() {
	settextstyle(7,0,1);
	sprintf(lpszSpeed, "%11d", 100-Speed);
	putimage(501,171, bmpSpeed,0);
	outtextxy(499,168,lpszSpeed);
}


void PrintLinesCleared() {
	settextstyle(7,0,1);
	sprintf(lpszLinesCleared, "%11d", LinesCleared);
	putimage(501,231, bmpLinesCleared,0);
	outtextxy(499,228,lpszLinesCleared);
}

int ShowGameOver() {
	MessageBox("Game Over");
	return 0;
}

char  MessageBox(char *Message, int Width, int Size) {  //Width defaults to 240
	int Color=0;
	int TextX= 320-(Width/2)+25;
	setfillstyle(10, YELLOWDR);
	bar(320-(Width/2)+6,206,320+(Width/2)-6,274);
	DrawBox(320-(Width/2),200,320+(Width/2),280, YELLOWBR, YELLOW, YELLOWDR);
	setfillstyle(10, GREENDR);
	setcolor(GREENBR);
	settextstyle(10,0,Size);
	outtextxy(TextX,210, Message);
	while(!kbhit()) {
	   switch(Color++) {
	   case 0:
			setcolor(GREENBR);
			break;
	   case 1:
			setcolor(GREEN);
			break;
	   case 2:
			setcolor(GREENDR);
			break;
	   case 3:
			setcolor(GREEN);
			break;
	   }
	   outtextxy(TextX,210, Message);
	   delay(80);
	   if(Color>3)
		  Color=0;
	}
	setcolor(GREEN);
	return getch();  // return the last key pressed form the keyboard buffer
}

void SplashScreen() {
	cleardevice();
	int Color=0;
	int i,j;
	int Bricks[20][30] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,1,1,0,
		0,1,0,0,1,1,0,1,0,0,1,1,0,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,
		0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,
		0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1,0,0,0,
		0,1,0,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,
		0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,
		0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,
		0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,
		0,1,0,0,0,1,0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,
		0,1,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,
		0,1,1,1,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,1,1,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};
//		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9

	   setfillstyle(10, BLUEDR);
	   bar(0,0,640,480);
	   for(j=0; j<20; j++)
			for(i=0; i<6; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpBlueBrick,0);
			}
	   for(j=0; j<20; j++)
			for(i=6; i<12; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpGreenBrick,0);
			}
	   for(j=0; j<20; j++)
			for(i=12; i<14; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpRedBrick,0);
			}
	   for(j=0; j<20; j++)
			for(i=14; i<20; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpYellowBrick,0);
			}
	   for(j=0; j<20; j++)
			for(i=20; i<25; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpPurpleBrick,0);
			}
	   for(j=0; j<20; j++)
			for(i=25; i<30; i++) {
			  if(!Bricks[j][i])
				  continue;
				putimage(i*21, j*21, bmpRedBrick,0);
			}
		setcolor(YELLOWBR);
		settextstyle(7,0,5);
		outtextxy(10,350,"Created by Ali Akbar");
		setcolor(GREENBR);
		settextstyle(10,0,3);
		Color=0;
		while(!kbhit()) {
		   switch(Color++) {
		   case 0:
				setcolor(GREENBR);
				break;
		   case 1:
				setcolor(GREEN);
				break;
		   case 2:
				setcolor(GREENDR);
				break;
		   case 3:
				setcolor(GREEN);
				break;
		   }
		   outtextxy(300,400,"[ pHroZeN GeeK ]");
		   delay(80);
		   if(Color>3)
			  Color=0;
		}
}
