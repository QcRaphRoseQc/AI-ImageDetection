#pragma once

#include "includes.hpp"
#include "string"




	class Key {       // The class
	public:             // Access specifier


		int key;
		//how to add a string variable to a class?
		//string key;
		int virtualKey;
		int virtualKeyInt;
		int primaryWeapon;
		int secondaryWeapon;
		float sensitivityX;
		float sensitivityY;
		float xOffset;
		float yOffset;
		bool isAim;
		bool isTrigger;
		bool isBodyTrigger;
		bool isEnable;
	};