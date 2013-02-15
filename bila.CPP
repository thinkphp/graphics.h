#include <dos.h>
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>

class billa
{

private:
       int xmax;
       int ymax;
       int r,x,y,dx,dy;
       char s[10];

public:
       billa(int,int,int);//constructorul clasei
       ~billa(){closegraph();};//destructorul clasei
       void init();
       void deseneaza();
       void animatie();

};


//constructorul clasei-------------------------------------

billa::billa(int label,int a=100,int b=100)
{

init();
itoa(label,s,10);
xmax=getmaxx();
ymax=getmaxy();
dx=1;
dy=1;
x=a;
y=b;
settextjustify(CENTER_TEXT,CENTER_TEXT);
r=50;

};

//metoda de initializare a graficului-------------------------------------

void billa::init()
{
int gDriver=DETECT,gm;
initgraph(&gDriver,&gm,"c:\\borlandc\\bgi");
}


void billa::deseneaza()
{
circle(x,y,r);
outtextxy(x,y,s);
}

void billa::animatie()
{

setcolor(BLACK);
deseneaza();

if(y+dy<r || y+dy>ymax-r) dy=-dy;

if(x+dx<r || x+dx>xmax-r) dx=-dx;

x+=dx;
y+=dy;

setcolor(WHITE);
deseneaza();
}



void main()
{
billa* b = new billa(888,500,400);
billa* c = new billa (999, 270, 75);
billa* d = new billa (777, 50, 50);

do
{
b->animatie();
c->animatie();
d->animatie();
delay(10);
}while (!kbhit());

getch();
}