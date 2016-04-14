// this file containts the class for the object 

#include <iostream>
#include <windows.h>
#include <cmath>

#define ID_TIMER 100


class Animation{
	protected:
    COLORREF Bcolor;

public:
    POINT center;
    int SpeedX,SpeedY;

    Animation(POINT center,const int &speed);
    bool accelerateB(const int& DSpeedX,const int& DSpeedY);
    bool BallColor(const COLORREF &Bcolor);
    bool BallCollision(const RECT &rect);
    bool objectTransition; //form circle to rectangle
    virtual bool moveB(const HDC &hdc,const RECT& rect,HBRUSH &hBrush)=0;

	Animation() {
		objectTransition = false;
	}
};

class Sphere : public Animation {
public:
    Sphere(POINT center,const int &speed): Animation(center,speed) {// inherits the constructor from Animation

    }

    bool moveB(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};

bool Interaction(Animation &first,Animation &second);
