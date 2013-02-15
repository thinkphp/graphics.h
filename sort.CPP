#include <dos.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#define LEFT 40

class billa
{

private:
       int xmax;//getmaxx()...
       int ymax;//getmaxy()...
       int r,x,y;//raza left top....
       char s[10];

public:
       billa(int,int,int);        //constructorul clasei
       ~billa();                  //destructorul clasei
       void deseneaza();
       char* getLabel();
       void  setLabel(int);
	int  GetX();
	int  GetY();
       void  SetX(int);
       void  SetY(int);
       void operator=(billa&ob)
       {
       x=ob.x;
       y=ob.y;
       r=ob.r;
       strcpy(s,ob.s);
       }

};


//constructorul clasei-------------------------------------

billa::billa(int label,int a=100,int b=100)
{
itoa(label,s,10);
xmax=getmaxx();
ymax=getmaxy();
x=a;
y=b;
settextjustify(CENTER_TEXT,CENTER_TEXT);
r=40;
};


//destructorul clasei

billa::~billa()
{
closegraph();
}


//metode...........

void billa::deseneaza()
{
circle(x,y,r);
outtextxy(x,y,s);
}


char* billa::getLabel()
{
return s;
}


void billa::setLabel(int x)
{
char *string;
itoa(x,string,10);
strcpy(s,string);
}



int billa::GetX()
{
return x;
}



int billa::GetY()
{
return y;
}

void billa::SetX(int left)
{
x=left;
}


void billa::SetY(int top)
{
y=top;
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
billa *vec[100];
int GetNumber(int x,int y);
void interschimba(billa *a,billa *b,int n,int nr);
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


//functia principala..........

void main()
{
int ch;
int factor;
int i,n;
int left,top,label;
int sortat;
int aux;
int j,k,min;
int m;
static int h[]={3,1};


billa *c,*x;

int gDriver=DETECT,gm;
initgraph(&gDriver,&gm,"c:\\borlandc\\bgi");


printf("n-->");scanf("%d",&n);

factor=0;

for(i=0;i<n;i++)
{
printf("label=");
scanf("%d",&label);
vec[i]=new billa(label,factor+LEFT,getmaxy()/2);
factor+=80;
}


cleardevice();

for(i=0;i<n;i++) vec[i]->deseneaza();


getch();

printf("Alegeti metoda de sortare:\n");
printf("1-------------->bubble sort\n");
printf("2-------------->selection by minim\n");
printf("3-------------->shell sort\n");

scanf("%d",&ch);


switch(ch)
{

//sortarea prin metoda bulelor
/*
case 1:

do
{
sortat=1;

for(i=0;i<n-1;i++)
{

if(atoi(vec[i]->getLabel())  >  atoi(vec[i+1]->getLabel()))
{
interschimba(vec[i],vec[i+1],GetNumber(i,i+1),n);
c=vec[i];
vec[i]=vec[i+1];
vec[i+1]=c;
sortat=0;

}//end if


}//end for

}while(!sortat);

break;
*/

/*
//sortare prin selectarea minimului
case 2:

for(i=0;i<n-1;i++)
{
min=atoi(vec[i]->getLabel());
k=i;

for(j=i+1;j<n;j++)
{

if(atoi(vec[j]->getLabel()) < min)
{

min=atoi(vec[j]->getLabel());
k=j;

interschimba(vec[i],vec[k],GetNumber(i,k),n);

c=vec[i];
vec[i]=vec[k];
vec[k]=c;

}//end if


}

}
*/


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//sortare shell
case 3:

//h[]={3,1}
//9 8 1 2
//2 8 1 9
//2 1 8 9


//1 9 3 0
//0 9 3 1
//0 3 9 1
//0 1 3 9


for(m=0;m<2;m++)
{

k=h[m];//k=1

for(i=k;i<n;i++)//i=2;i=3
{
x=vec[i];

for(j=i-k;(j>=0) &&  (atoi(vec[j]->getLabel()) > atoi(x->getLabel())) ;j-=k)
{

interschimba(vec[j],vec[j+k],GetNumber(j,j+k),n);
c=vec[j+k];
vec[j+k]=vec[j];
vec[j]=c;
}//end for j

vec[j+k]=x;

} //end for i

}//end for m



//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}//end switch


getch();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************

void interschimba(billa *a,billa *b,int n,int nr)
{
int x1,y1,x2,y2;
int dx,dy;

int xsec1,ysec1,xsec2,ysec2;

xsec1=a->GetX();ysec1=a->GetY();
xsec2=b->GetX();ysec2=b->GetY();


const int ax=b->GetX();

x1=a->GetX();y1=a->GetY();
x2=b->GetX();y2=b->GetY();

dx=1;
dy=1;

do
{
setcolor(BLACK);
a->deseneaza();
b->deseneaza();

    x1+=dx;a->SetX(x1);//y1=dy;a->SetY(y1);

    x2-=dx;b->SetX(x2);//y2+=dy;b->SetY(y2);

setcolor(WHITE);
a->deseneaza();
b->deseneaza();


for(int i=nr-1;i>=0;i--)
{
if(!n&1)

vec[i]->deseneaza();

n=n>>1;
}

delay(40);

}while(x1!=ax);

}//end function

//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************




int GetNumber(int x,int y)
{
int n1=1;
int n2=1;

if(x && y)
{
n1=n1<<x;
n2=n2<<y;

  return n1 | n2;
}
else

if(x==0 && y)
{
n2=n2<<y;
n1=1;
return n1 | n2;
}

else  if(y==0 && x)
       {
       n1=n1<<x;
       n2=1;

       return n1 | n2;
       }



/*
00000010
00001000
00001010
  */
}

//*********************************************************************
//*********************************************************************
//*********************************************************************
//*********************************************************************

