#include "Scroll.h"

#include <windows.h>
#include <ctime>
#include <iostream>



LRESULT CALLBACK Scroll::Mouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
	{
		return CallNextHookEx(mh, nCode, wParam, lParam);
	}

	MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

	if (pMouseStruct != NULL)
	{
		if (wParam == WM_MOUSEWHEEL)
		{
			if (HIWORD(pMouseStruct->mouseData) == 120)
			{
				delta = 1;
			}
			else
			{
				delta = -1;
			}
		}
		else
		{
			delta = 0;
		}
	}

	return CallNextHookEx(mh, nCode, wParam, lParam);
}

int Scroll::GetScrollDelta()
{

	delta = 0;
	

	MSG message;
	bool peek = true;
	long tm = time(0);

	while (peek)
	{
		PeekMessage(&message, NULL, 0, 0, PM_REMOVE);
		if (!delta == 0 || tm < time(0))
		{
			peek = false;
		}
	}

	UnhookWindowsHookEx(mh);
	return delta;
}


int Scroll::returnDelta() {


	while (1)
	{
		delta = GetScrollDelta();
		if (delta == -1)
		{
			std::cout << "delta : " << delta << std::endl;

		}
		else if (delta == 1)
		{
			std::cout << "delta : " << delta << std::endl;

		}

	}
	return delta;
}

