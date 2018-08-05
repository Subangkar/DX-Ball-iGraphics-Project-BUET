#include "iGraphics.h"


#define SCREEN_HEIGHT 680
#define SCREEN_WIDTH 1280
#define SIDEBAR_THICKNESS 20



#define FILE_IMG_SIDEBAR_LEFT  "SideBar_left.bmp"
#define FILE_IMG_SIDEBAR_RIGHT "SideBar_right.png"

#define BALL_RAD_NORMAL 10
#define BALL_RAD_BIG 15

#define STICK_THICKNESS 30


struct Point
{
	int x;
	int y;
};

struct Image
{
	char fileName[25];
	int height;
	int width;
};


struct Color
{
	int r;
	int g;
	int b;
};

struct Circle
{
	Point center;
	Color colour;
	int radius;

	int dx;
	int dy;
	int slope;
}Ball;

struct Rectangle
{
	Point start;
	Color colour;
	int height;
	int width;

	int dx;
	int dy;
} Stick,Left_SideBar,Right_SideBar,Upper_SideBar;


Point ballStart,Screen_START,Screen_END;


Color sidebar;


int ball_x, ball_y;
int dx, dy;


// Here drawing color of each object is assigned to default value
void assignColors()
{
	Left_SideBar.colour.r=195,Left_SideBar.colour.g=195,Left_SideBar.colour.b=195;

	Ball.colour.r=255,Ball.colour.g=128,Ball.colour.b=50;

	Stick.colour.r=56,Stick.colour.g=78,Stick.colour.b=177;
}

// Set current drawing color to passed argument
void SetDrawingColor(Color clr)
{
	iSetColor(clr.r,clr.g,clr.b);
}

// Set Side Boundaries Starting Point with respect to screen's height & width
void setSideBoundaries()
{
	Left_SideBar.start.x=0,Left_SideBar.start.y=0;

	Right_SideBar.start.x=SCREEN_WIDTH-SIDEBAR_THICKNESS,Right_SideBar.start.y=0;

	Upper_SideBar.start.x=0,Upper_SideBar.start.y=SCREEN_HEIGHT-SIDEBAR_THICKNESS;

	Screen_START.x=SIDEBAR_THICKNESS,Screen_START.y=0;
	Screen_END.x=SCREEN_WIDTH-SIDEBAR_THICKNESS,Screen_END.y=SCREEN_HEIGHT-SIDEBAR_THICKNESS;
}


void currentScreenBoundary()
{

}


void getBall()
{
}

// Draws SideBar Rectangles
void DrawSideBoundaries()
{
	SetDrawingColor(Left_SideBar.colour);

	iFilledRectangle(Left_SideBar.start.x,Left_SideBar.start.y,SIDEBAR_THICKNESS,SCREEN_HEIGHT); // for left sidebar

	iFilledRectangle(Right_SideBar.start.x,Right_SideBar.start.y,SIDEBAR_THICKNESS,SCREEN_HEIGHT); // for right sidebar

	iFilledRectangle(Upper_SideBar.start.x,Upper_SideBar.start.y,SCREEN_WIDTH,SIDEBAR_THICKNESS); // for upper sidebar
}


void DrawStick()
{
	SetDrawingColor(Stick.colour);
	iFilledRectangle(Stick.start.x,Stick.start.y,Stick.width,Stick.height);
}

void DrawBall()
{
	SetDrawingColor(Ball.colour);
	iFilledCircle(Ball.center.x,Ball.center.y,Ball.radius);
}


void iDraw()
{
	iClear();

	DrawSideBoundaries();

	DrawStick();
	DrawBall();


	iSetColor(255, 100, 10);

	iSetColor(255, 255, 255);
	iText(10, 10, "Press p for pause, r for resume, END for exit.");
}


// returns 1 if ball's current pos is found inside stick area else 0
int insideStickZone()
{
	/*
	When ball comes from left side ball_x+rad < Stick_left_x
						right side ball_x-rad > Stick_right_x
						upper side ball_y-rad > Stick_upper_y
								for avoiding getting inside Stick
	*/
	return Stick.start.x<=Ball.center.x+Ball.radius && Ball.center.x-Ball.radius<=Stick.start.x+Stick.width && Ball.center.y-Ball.radius<=Stick.start.y+Stick.height;
}

void ballChange()
{
	Ball.center.x += Ball.dx;
	Ball.center.y += Ball.dy;

	if( insideStickZone() )
	{
		Ball.dx = -Ball.dx;
		Ball.dy = -Ball.dy;
	}

	/*
	When ball comes from ball_x+rad < right_sidebar
						 ball_x-rad > left_sidebar
						 ball_y+rad > upper_sidebar
								to remain inside sidebars
	*/

	if(Ball.center.x+Ball.radius > Screen_END.x  || Ball.center.x-Ball.radius < Screen_START.x)Ball.dx = -Ball.dx;
	if(Ball.center.y+Ball.radius > Screen_END.y  || Ball.center.y-Ball.radius < Screen_START.y)Ball.dy = -Ball.dy;

}

// ensurely moves the stick so that it remains inside the sidebars
void stickMove(char dir)
{
	if(dir=='l' && Stick.start.x-Stick.dx>=Screen_START.x)
	{
		Stick.start.x -= Stick.dx;
	}
	if(dir=='r' && Stick.start.x+Stick.width+Stick.dx<=Screen_END.x)
	{
		Stick.start.x += Stick.dx;
	}
}


void assignInitialValues()
{
	assignColors();
	setSideBoundaries();

	Stick.height  = STICK_THICKNESS,Stick.width=200;

	Stick.start.x = SCREEN_WIDTH/2-Stick.width/2,Stick.start.y=0;
	Stick.dx = 10,Stick.dy=0;

	Ball.radius   = BALL_RAD_NORMAL;
	Ball.center.x = SCREEN_WIDTH/2,Ball.center.y=Stick.height+Ball.radius;
	Ball.dx=5,Ball.dy=7;

}


int main()
{

	iSetTimer(5, ballChange);

	assignInitialValues();


	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Ball Demo");
	return 0;
}



/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{

}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if( state == GLUT_LEFT) //button == GLUT_LEFT_BUTTON &&
	{
	//	printf("left""""""""""""""""""""\n\n");
		stickMove('l');
	}
	//if(state == GLUT_RIGHT) //button == GLUT_RIGHT_BUTTON &&
	//{
	//}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if(key == 'p')
	{
		//do something with 'q'
		iPauseTimer(0);
	}
	if(key == 'r')
	{
		iResumeTimer(0);
	}
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{
	if(key == GLUT_KEY_LEFT)
	{
		stickMove('l');
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		stickMove('r');
	}

	if(key == GLUT_KEY_END)
	{
		exit(0);
	}
}

