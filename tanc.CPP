/*
       Created by Adrian Statescu "_"
*/
#include <graphics.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#define RAZA 3
#define nrbricks 10
#define RAZA_ATAC 5

//prototips:
//---------------------------
void drawmenu();
void score(int);
void healt(int);
void atack(int);
//---------------------------

//globals variables;

int glontz;     //variabila glontz;
int xg,yg;       //abscina si ordonata pentru GLONTZ;

int xa,ya;    //abscisa si ordonata pentru glontul atac;

int sc,scor=0;

int life,copy_life;       //viata pentru tanc

//end


class brick
{
public:

int x,y;
int culoare;
int dx;
int viata;


public:

      brick(){};
      brick(int,int,int);//constructorul clasei
      ~brick(){};          //destructorul clasei
      void drawbrick();
      void setX(int);
      void setY(int);
      void setC(int);
       int getX();
       int getY();
      void motion();
      void sterge();

};

void brick::setX(int a)
{
x=a;
}
void brick::setY(int a)
{
y=a;
}
void brick::setC(int a)
{
culoare=a;
}

brick::brick(int a,int b,int col)
{
x=a;
y=b;
culoare=col;
};

void brick::drawbrick()
{
setcolor(culoare);
rectangle(x,y,x+40,y+15);
setfillstyle(SLASH_FILL,culoare);
bar(x,y,x+40,y+15);
setcolor(WHITE);
};

void brick::sterge()
{
setcolor(BLACK);
rectangle(x,y,x+40,y+15);
setfillstyle(SLASH_FILL,BLACK);
bar(x,y,x+40,y+15);
}


void brick::motion()
{
sterge();

if(x>=getmaxx()-41) dx=1;

if(x<=1) dx=0;

if(dx==1) x--;

     else

	  x++;

drawbrick();
}

int brick::getX()
{
return x;
}

int brick::getY()
{
return y;
}


//---------------------------------------
//---------------------------------------
//---------------------------------------

class glont
{

public:
      glont(){};   //constructorul clasei
      ~glont(){};  //destructorul
      void deseneaza();
      void sterge();
      void misca();
};

void glont::deseneaza()
{
setfillstyle(SOLID_FILL,YELLOW);
ellipse(xg,yg,0,360,RAZA,RAZA);
fillellipse(xg,yg,RAZA,RAZA);
}

void glont::sterge()
{
setcolor(BLACK);
setfillstyle(SOLID_FILL,BLACK);
ellipse(xg,yg,0,360,RAZA,RAZA);
fillellipse(xg,yg,RAZA,RAZA);
setcolor(WHITE);
}


void glont::misca()
{
sterge();
yg--;

if(yg<=34) {glontz=0;sterge();}

 else

	deseneaza();
}



//---------------------------------------
//---------------------------------------
//---------------------------------------



//---------------------------------------
//---------------------------------------
//---------------------------------------

class tanc
{
public:

      //date membre
      int x;      //abscisa
      int y;      //ordonata
      int dx;
      int dy;
      tanc(){};//constructorul implicit al clasei
      tanc(int,int);  //al doilea constructor
      ~tanc(){};// destructorul clasei

     //metode membre
public:

      int getX();
      int getY();

      void drawtanc();
      void removetanc();
      void movetanc();



};

tanc::tanc(int x0,int y0)
{
x=x0;
y=y0;
};


int tanc::getX()
{
return x;
}

int tanc::getY()
{
return y;
}


void tanc::drawtanc()
{
line(x,y,x+50,y);
line(x,y,x,y-20);
line(x,y-20,x+20,y-20);
line(x+30,y-20,x+50,y-20);
line(x+50,y-20,x+50,y);
line(x+20,y-20,x+20,y-30);
line(x+30,y-20,x+30,y-30);
circle(x+25,y-30,5);
}


void tanc::removetanc()
{
setcolor(BLACK);
drawtanc();
setcolor(WHITE);
}


void tanc::movetanc()
{
int key;
dx=0;

if(kbhit())
{

key=getch();

     switch(key)
	{

	case 75:dx-=1;break;

	case 77:dx+=1;break;

	case 27:exit(EXIT_SUCCESS);break;

	case 32:if(!glontz){xg=x+25;yg=y-30;glontz=1;for(int i=1;i<=10;i++)sound(20);nosound();break;}

	}

removetanc();
x+=10*dx;

if(x<1)x=1;
if(x>588)x=588;

drawtanc();
}

}



//---------------------------------------
//---------------------------------------
//---------------------------------------



void main()
{
int xx,yy,cc;
int i;
int contor;

//initializare mod random
randomize();

//initializare mode grafic
int gd=DETECT,gm;
initgraph(&gd,&gm,"c:\\borlandc\\bgi");

life=3;   //avem la dispozitie 3 vieti

drawmenu();
score(10);
healt(life);

brick B[nrbricks];


for(i=0;i<nrbricks;i++)
{
xx=random(500);
yy=random(400)+35;
again:cc=random(10);
if(cc==0)  goto again;
B[i].setX(xx);
B[i].setY(yy);
B[i].setC(cc);
B[i].drawbrick();
B[i].viata=1;
}

xa=100;ya=40;

tanc T(200,getmaxy()-2);

glont G;
glontz=0;

contor=nrbricks;

do
{



T.movetanc();


if(glontz!=0)

{

G.misca();

for(i=0;i<nrbricks;i++)

if((xg+RAZA>=B[i].getX()) && (xg-RAZA<=B[i].getX()+40) && (yg+RAZA>=B[i].getY()) && (yg-RAZA<=B[i].getY()+15) &&  (B[i].viata!=0))
{
glontz=0;
G.sterge();
B[i].sterge();
B[i].viata=0;
contor--;
for(int i=1;i<=10;i++)sound(20);nosound();
score(10);
}


}//end if glontz


//-------enamy -----------------

atack(T.getX());

if(ya>=getmaxy()-6)
{
setcolor(BLACK);
setfillstyle(SOLID_FILL,BLACK);
ellipse(xa,ya,0,360,RAZA_ATAC,RAZA_ATAC);
fillellipse(xa,ya,RAZA_ATAC,RAZA_ATAC);
xa=random(630)+2;ya=40;
}

if(ya+RAZA_ATAC>=T.getY() && xa+RAZA_ATAC<=T.getX()+50 && xa-RAZA_ATAC>=T.getX())
{
copy_life=life;
life--;
healt(life);
}

if(life==0) contor=0;

//-------enamy-----------------



for(i=0;i<nrbricks;i++) if(B[i].viata) B[i].motion(); //bricks motion

T.drawtanc();


delay(5);
}while(contor!=0);//end while


setcolor(RED);
outtextxy(getmaxx()/2-50,getmaxy()/2,"GAME OVER");


getch();
closegraph();

}




//---------------------------------------
//---------------------------------------
//---------------------------------------


void drawmenu()
{
setcolor(MAGENTA);
rectangle(0,0,getmaxx(),getmaxy());
line(0,30,getmaxx(),30);
line(100,0,100,30);
line(300,0,300,30);
}



void score(int value)
{
char string[50],string2[50];
sc=scor;
scor=scor+value;

settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);

setcolor(BLACK);
sprintf(string2,"Score: %d",sc);
outtextxy(530,2,string2);

setcolor(GREEN);
sprintf(string,"Score: %d",scor);
outtextxy(530,2,string);
setcolor(WHITE);
}



void healt(int value)
{

char string[50],string2[50];


settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);

setcolor(BLACK);
sprintf(string2,"Life: %d",copy_life);
outtextxy(10,2,string2);

setcolor(GREEN);
sprintf(string,"Life: %d",value);
outtextxy(10,2,string);
setcolor(WHITE);

}



void atack(int x0)
{

setcolor(BLACK);
setfillstyle(SOLID_FILL,BLACK);
ellipse(xa,ya,0,360,RAZA_ATAC,RAZA_ATAC);
fillellipse(xa,ya,RAZA_ATAC,RAZA_ATAC);


if( (x0+20) > xa ) {xa++;ya++;}

   else

if( (x0+20) < xa ) {xa--;ya++;}

     else

      { xa=xa;ya++;}


setcolor(RED);
setfillstyle(SOLID_FILL,RED);
ellipse(xa,ya,0,360,RAZA_ATAC,RAZA_ATAC);
fillellipse(xa,ya,RAZA_ATAC,RAZA_ATAC);

}



/*

 x,y
   _____
 .|     |.
  |_____|   x+30,y+15
      r
     .
     (x,y)


    y-r<=y+15 and y+r>=y and x-r<=x+30 and x+r>=x

*/