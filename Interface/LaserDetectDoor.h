#ifndef _LASERDETECTDOOR_H
#define _LASERDETECTDOOR_H
#include "stdafx.h"
#include "UrgControl.hpp"
#include <iostream>

class LaserDetectDoor
{
public:
	LaserDetectDoor(UrgControl* urgC);
	bool haveDoor(int distToDoor);
	UrgControl* _urgC;
};

#endif