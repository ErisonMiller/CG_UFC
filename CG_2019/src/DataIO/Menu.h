#pragma once
#include "ImGUI.h"
#include <vector>
#include <string>

using namespace std;

class Menu
{
public:
	string title;
	vector <string> subMenus;

	Menu();
	Menu(std::string title);
	void OpenMenu();
	void CloseMenu();
	void AddSubMenu(string SubMenuName);
	void BoxList();
	~Menu();

};

