#include "stdafx.h"
#include "LaserDetectDoor.h"


LaserDetectDoor::LaserDetectDoor(UrgControl* urgC){_urgC=urgC;}

bool LaserDetectDoor::haveDoor(int distToDoor)
{
	int times=0;
	bool existDoor=true;
	while(times<5)
	{
		int count=0 , distAverage=0 , distTotal=0;
		_urgC->getScan682();
		_urgC->deleteNoiseData(10);
		for(int i=0; i<50; i++)
		{
			//std::cout<<_urgC->_vData[341+i]<<" "<<_urgC->_vData[341-i]<<std::endl;
			if(_urgC->_vData[341+i]!=0)
			{
				count++;
				distTotal = distTotal + _urgC->_vData[341+i];
			}
		
			if(_urgC->_vData[341-i]!=0)
			{
				count++;
				distTotal = distTotal + _urgC->_vData[341-i];
			}
	
		}
		if(count==0)
			count=1;
		
		distAverage = distTotal/count;

		if(distAverage/10>distToDoor)  //existDoor=false
		{
			if(!existDoor)
			{
				times++;
			}
			else
			{
				times=0;
				existDoor=false;
			}
			std::cout<<"go "<<distAverage/10<<std::endl;
			
		}
		else  //existDoor=true
		{		
			if(existDoor)
			{
				times++;
			}
			else
			{
				times=0;
				existDoor=true;
			}
			std::cout<<"stop "<<distAverage/10<<std::endl;
			
		}
	}

	if(existDoor)
	{
		std::cout<<"stop "<<std::endl;
		return true;
	}		
	else
	{
		std::cout<<"go "<<std::endl;
		return false;
	}
		
	
}