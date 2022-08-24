#pragma once

#include "includes.hpp"




class Weapon {       
public:             

	std::string name;
	
	float weaponId;
	
	float xSensitivity;
	
	float ySensitivity;

	
	float xOffsetClose;
	float yOffsetClose;

	float xOffsetMedium;
	float yOffsetMedium;	
	
	float xOffsetFar;
	float yOffsetFar;	
	
	float xTbOffsetClosePositive;
	float yTbOffsetClosePositive;
	float xTbOffsetCloseNegative;
	float yTbOffsetCloseNegative;
	
	float xTbOffsetMediumPositive;
	float yTbOffsetMediumPositive;
	float xTbOffsetMediumNegative;
	float yTbOffsetMediumNegative;
	
	float xTbOffsetFarPositive;
	float yTbOffsetFarPositive;
	float xTbOffsetFarNegative;
	float yTbOffsetFarNegative;
};