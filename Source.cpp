
#include <glut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <windows.h>

#pragma comment (lib,"kernel32.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"advapi32.lib")

using namespace std;
#define width 80
#define height 45


class game {
public:
	int delay;
	float scale;
	int WinScale;
	int WinPosX;
	int WinPosY;
	int StartPosX;
	int StartPosY;
	int StartSize;
	game(){
		StartSize=3;
		StartPosX = (int)(width/2);
		StartPosY = (int)(height/2);
		delay =100;
		scale = 1;
		WinScale = 10;
		WinPosX = 200;
		WinPosY = 0;
	}
	void init();
}settings;

class block
{

public:
	int x;
	int y;
	block()
	{
		x=-1;
		y=-1;
	}
	void draw();

}blocks[width*height];


class food
{
public:
	int x;
	int y;
	food(){
		x=-1;
		y=-1;
	}
	void drawfood();
	void spawn();

}food;

class snake
{
public:
	int direction;
	int size;
	bool turn;
	snake()
	{
		turn = false;
		size = settings.StartSize;
		direction = GLUT_KEY_UP;

	}
	void DrawSnake();
	void move();
	void cut();
	void eat();

}player;
 
void food::drawfood()
{
glColor3f(1,1,1);
		glVertex2f(x*settings.scale,y*settings.scale);
		glVertex2f(x*settings.scale+settings.scale,y*settings.scale);
		glVertex2f(x*settings.scale+settings.scale,y*settings.scale+settings.scale);
		glVertex2f(x*settings.scale,y*settings.scale+settings.scale);
}
void snake::move()
{
	for(int i=size;i>0;i--){
		blocks[i].x=blocks[i-1].x;
		blocks[i].y=blocks[i-1].y;
	}
	switch (direction)
	{
	case GLUT_KEY_UP:
		blocks[0].y++;
		break;
	case GLUT_KEY_DOWN:
		blocks[0].y--;
		break;
	case GLUT_KEY_LEFT:
		blocks[0].x--;
		break;
	case GLUT_KEY_RIGHT:
		blocks[0].x++;
		break;
	}
	if(blocks[0].x<0)blocks[0].x += width;
	if(blocks[0].x>=width)blocks[0].x -= width;
	if(blocks[0].y<0)blocks[0].y+=height;
	if(blocks[0].y>=height)blocks[0].y-=height;
}

void snake::eat()
{
	if((blocks[0].x==food.x)&&(blocks[0].y==food.y))
	{
		size++;
		food.spawn();
	}
}

void snake::cut(){
	for(int i=3;i<size; i++)
	{
		if((blocks[0].x==blocks[i].x)&&(blocks[0].y==blocks[i].y))
		{
			size=i;
			break;
		}
	}
}

void keyboard(int key,int x,int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		if((player.direction != GLUT_KEY_DOWN)&&(player.turn))player.direction = GLUT_KEY_UP;
		player.turn =false;
		break;

		case GLUT_KEY_DOWN:
		if((player.direction != GLUT_KEY_UP)&&(player.turn))player.direction = GLUT_KEY_DOWN;
		player.turn =false;
		break;

		case GLUT_KEY_LEFT:
		if((player.direction != GLUT_KEY_RIGHT)&&(player.turn))player.direction = GLUT_KEY_LEFT;
		player.turn =false;
		break;

		case GLUT_KEY_RIGHT:
		if((player.direction != GLUT_KEY_LEFT)&&(player.turn))player.direction = GLUT_KEY_RIGHT;
		player.turn =false;
		break;
	}
		
}

void Timer(int value)
{
	player.turn = true;
	player.move();
	player.cut();
	player.eat();
	glutPostRedisplay();
	glutTimerFunc(settings.delay,Timer,0);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	food.drawfood();
	player.DrawSnake();
	glEnd();
	glutSwapBuffers();
}

void snake::DrawSnake()
{

	for(int i;i<size;i++)
	{
		blocks[i].draw();
	}
}

	void block::draw(){
		glColor3f(1,1,1);
		glVertex2f(x*settings.scale,y*settings.scale);
		glVertex2f(x*settings.scale+settings.scale,y*settings.scale);
		glVertex2f(x*settings.scale+settings.scale,y*settings.scale+settings.scale);
		glVertex2f(x*settings.scale,y*settings.scale+settings.scale);
}

void game::init()
{
	blocks[0].x=StartPosX;
	blocks[0].y=StartPosY;
}

void food::spawn()
{
	x=(rand()%width);
	y=(rand()%height);
	for(int i; i<player.size; i++){
		if((x==blocks[i].x)&&(y==blocks[i].y))spawn();

	}
}



void drawSquare()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1.0, 0.0, 0.0);
    int xsize=0,ysize=0;
    for(int j=0;j<10;j++)
    {

        xsize=0;
        for(int i=0;i<10;i++)
        {
             glBegin(GL_POLYGON);
               glVertex3f(-50.0+xsize, -50.0+ysize, 0.0);
               glVertex3f(-40.0+xsize, -50.0+ysize, 0.0);
               glVertex3f(-40.0+xsize, -40.0+ysize, 0.0);
               glVertex3f(-50.0+xsize, -40.0+ysize, 0.0);
            glEnd();
         xsize+=10.0;
     }
     ysize+=10.0;
     }
     glFlush();
  }

int main (int argc, char * argv[])
{
	drawSquare();
	settings.init();
	food.spawn();
	srand(time(NULL));

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); 
       
		glutInitWindowSize(width*settings.WinScale,height*settings.WinScale);
		glutInitWindowPosition(settings.WinPosX, settings.WinPosY);

        glutCreateWindow("SNAKE");
		glutDisplayFunc(draw);
		glutTimerFunc(settings.delay,Timer,0);
		glutSpecialFunc(keyboard);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,width,0,height,-1,1);

        glutMainLoop();
 
        return 0;
}