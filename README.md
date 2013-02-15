Graphics.h
----------

Using functions of graphics.h in C compiler you can make graphics programs, animations, projects and games. You can
draw circles, lines, rectangles, bars and many other geometrical figures.


##Example

  //drawing concentric circles
  #include <graphics.h>
  int main() { 

   int gd = DETECT, gm;
   int x = 320, y = 240, radius;
 
   initgraph(&gd, &gm, "C:\\TC\\BGI");
 
   for ( radius = 10; radius <= 150 ; radius = radius + 20)
      circle(x, y, radius);
 
   getch();
   closegraph();
   return 0;
  }