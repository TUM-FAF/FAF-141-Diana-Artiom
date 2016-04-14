#include "Object.h"
   
Animation::Animation(POINT center,const int &speed) {
    this->center=center;
    this->SpeedX=speed;
    this->SpeedY=speed;
    Bcolor=RGB(0,0,0);
}

bool Animation::accelerateB(const int& DSpeedX,const int& DSpeedY) {
    SpeedX=DSpeedX;
    SpeedY=DSpeedY;
    return TRUE;
}

bool Animation::BallColor(const COLORREF &Bcolor) {
    this->Bcolor=Bcolor;
    return TRUE;
}

bool Sphere::moveB(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) {
    hBrush=CreateSolidBrush(Bcolor);
    SelectObject(hdc,hBrush);
    BallCollision(rect);
    center.x-=SpeedX;
    center.y-=SpeedY;

    if(objectTransition) {
		//objectTransition = !objectTransition;
        Rectangle(hdc, center.x - 20, center.y - 25, center.x + 25, center.y + 25 );
        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
        DeleteObject(hBrush);
        return TRUE;
    }

    Ellipse(hdc,center.x - 25, center.y - 25, center.x + 25, center.y + 25);
    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;
}

bool Interaction(Animation &first, Animation &second) {

    float distance;
    POINT totalSpeed;

    distance = sqrt( std::pow(static_cast<double>(first.center.x-second.center.x),2)+ std::pow(static_cast<double>(first.center.y-second.center.y),2) );


    if ( distance < 9 ) {
		totalSpeed.x=(abs(first.SpeedX)+abs(second.SpeedX))/2;
		totalSpeed.y=(abs(first.SpeedY)+abs(second.SpeedY))/2;

		first.accelerateB(-first.SpeedX*totalSpeed.x/abs(first.SpeedX),-first.SpeedY*totalSpeed.y/abs(first.SpeedY));
		second.accelerateB(-second.SpeedX*totalSpeed.x/abs(second.SpeedX),-second.SpeedY*totalSpeed.y/abs(second.SpeedY));

		first.BallColor(RGB(totalSpeed.x * 50 % 255, totalSpeed.x * 50 % 255, totalSpeed.x * 50 % 255));
    }

    return TRUE;
}

bool Animation::BallCollision(const RECT &rect) {

    if(center.x - 20 > rect.right - 1) {
            SpeedX=abs(SpeedX);
            objectTransition = true;   
	}

    if(center.x + 20 < rect.left + 1) {
            SpeedX=-abs(SpeedX);
            objectTransition = true;  
	}

    if(center.y + 20 > rect.bottom - 1) {
            SpeedY=abs(SpeedY + 5);
	}

    if(center.y - 20 < rect.top + 1) {
            SpeedY=-abs(SpeedY + 5) ;
	}

    return TRUE;
}
