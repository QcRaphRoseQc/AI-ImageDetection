#include "includes.hpp"
#include <thread>
#include "Scroll.h"


extern int StartRendering();
extern int project();
extern void aimGui();
extern void serializationKey();
extern void serializationWeapon();
extern void serializationCurrentWeapon();
extern void creerKey();
extern void creerWeapon();
extern void creerCurrentWeapon();

BOOL SetThreadPriority(HANDLE hThread, int nPriority);

int main()
{
	while (true) {
		
		
		if (!std::ifstream("configKey.json"))
		{
			std::cout << "config.json not found" << std::endl;
			creerKey();
			serializationKey();

		}
		if (!std::ifstream("configWeapon.json"))
		{
			std::cout << "config.json not found" << std::endl;
			creerWeapon();
			serializationWeapon();
		}
		if (!std::ifstream("configCurrentWeapon.json"))
		{
			std::cout << "config.json not found" << std::endl;
			creerCurrentWeapon();
			serializationCurrentWeapon();
		}


		std::thread worker1(project);
		//make worker1 priority the highest
		SetThreadPriority(worker1.native_handle(), THREAD_PRIORITY_HIGHEST);
		std::thread worker2(StartRendering);
		//make worker2 priority the lowest
		SetThreadPriority(worker2.native_handle(), THREAD_PRIORITY_LOWEST);

		std::thread t1(aimGui);

		
		
		

		int resultat = returnDelta();
		std::cout<< "resultat : " << resultat << std::endl;
		//getWheelState(resultat);

		worker1.join();
		worker2.join();
		t1.join();
	}
	
	

	


	

	
}