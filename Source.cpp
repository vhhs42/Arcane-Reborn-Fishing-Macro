#include <iostream>
#include <windows.h>
#include <vector>
//#include <fstream>

// Absolute function because I didn't feel like including a math library.
int abs(int x)
{
	return x >= 0 ? x : -x;
}

void startUpMenu()
{
	std::cout << "Arcane Reborn Auto Fisher" << std::endl << std::endl;
	std::cout << "To start, press '['. To pause, press ']'" << std::endl;
	std::cout << "For instructions on use, press 'I'." << std::endl;
	std::cout << "This app supports Windows only." << std::endl << std::endl;
}

void informationMenu()
{
	std::cout << std::endl << "Instructions:" << std::endl << "When running the macro, if you look closely at roughly the center of the screen, you will see three red pixels spaced evenly apart. These pixels mark three points the macro checks to identify if a fishing cue has come up. In the game, adjust the camera angle and zoom until the three red pixels are over the players head, where the fishing cues come up. From there, the macro should run fine. However, it is not a very smart macro, and will mistake fishing cues for other white objects (e.g. fishing line). Ensure the macro is paused or off when outside of the game, turn it on only when in." << std::endl << "Side note: the red pixels are not an exploit of the game and simply appear over the screen itself. They will disapear once you pause or exit the macro. This macro is undetectable by the game itself, but it is against the game's rules, and if you are caught using it you will likely be banned. Use it at your own risk." << std::endl << "Refer to the YouTube video if you have not already." << std::endl << std::endl;
}

void appLoop()
{
	// For resolution scaling.
	int res_X = GetSystemMetrics(SM_CXSCREEN);
	int res_Y = GetSystemMetrics(SM_CYSCREEN);

	std::vector<int> coords(3);
	double changeRatio_X = res_X / 3840.0;
	double changeRatio_Y = res_Y / 2160.0;
	int checkCoords_X = 1930 * changeRatio_X;
	int checkCoords_Y = 830 * changeRatio_Y;
	coords[0] = 1714 * changeRatio_X;
	coords[1] = checkCoords_X;
	coords[2] = 2146 * changeRatio_X;

	// For color detection.
	int red, green, blue;
	COLORREF color;
	bool found;

	// For mouse inputs.
	POINT cursor;
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
	input.mi.mouseData = 0;
	input.mi.dwExtraInfo = NULL;
	input.mi.time = 0;

	// For file recording.
	//std::ofstream colorFile;

	// For timer.
	int timer = 0;

	// Main application loop. Start/resume with "[", pause with "]".
	while (true)
	{
		while (!GetAsyncKeyState(VK_OEM_6))
		{
			Sleep(20);
			timer = timer + 60;
			std::cout << timer << std::endl;
			// If no fishing cue has come up for some time, we recast the line.
			if (timer >= 30000)
			{
				timer = 0;
				GetCursorPos(&cursor);
				input.mi.dx = cursor.x;
				input.mi.dy = cursor.y;
				SendInput(1, &input, sizeof(INPUT));
			}
			found = false;
			for (int x = 0; x < 3; x++)
			{
				HDC dc = GetDC(NULL);
				color = SetPixel(dc, coords[x] + 1, checkCoords_Y + 1, RGB(255, 0, 0));
				color = GetPixel(dc, coords[x], checkCoords_Y);
				red = GetRValue(color);
				green = GetGValue(color);
				blue = GetBValue(color);
				if (red == 255 && green == 255 && blue == 255)
				{
					return;
				}
				if (abs(red - green) > 5 or abs(green - blue) > 5 or red < 150 or green < 150 or blue < 150)
				{
					continue;
				}
				timer = 0;
				/*colorFile.open("colors.txt", std::ios::app);
				colorFile << "Detected RGB values: " << red << " " << green << " " << blue << std::endl;
				colorFile.close();*/
				found = true;
				ReleaseDC(NULL, dc);
				break;
			}
			if (!found)
			{
				continue;
			}
			GetCursorPos(&cursor);
			input.mi.dx = cursor.x;
			input.mi.dy = cursor.y;
			for (int i = 0; i < 14; i++)
			{
				SendInput(1, &input, sizeof(INPUT));
				Sleep(100);
			}
			Sleep(2500);
			SendInput(1, &input, sizeof(INPUT));
			Sleep(3000);
		}
		std::cout << "Paused." << std::endl;

		while (!GetAsyncKeyState(VK_OEM_4))
		{
			if (GetAsyncKeyState(0x49))
			{
				informationMenu();
			}
			Sleep(500);
			continue;
		}
		std::cout << "Running." << std::endl;
	}
}

int main()
{
	SetProcessDPIAware();

	startUpMenu();
  
	while (!GetAsyncKeyState(VK_OEM_4))
	{
		if (GetAsyncKeyState(0x49))
		{
			informationMenu();
		}
		Sleep(500);
		continue;
	}
	std::cout << "Running." << std::endl;

	while (true)
	{
		appLoop();
	}
}
