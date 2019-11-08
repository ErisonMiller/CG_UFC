#include "Menu.h"



Menu::Menu(string title)
{
	this->title = title;
}

Menu::Menu()
{

}
void Menu::OpenMenu() {
	ImGui::Begin(this->title.c_str());
}

void Menu::CloseMenu() {
	ImGui::End();
}

void Menu::AddSubMenu(string SubMenuName) {
	this->subMenus.push_back(SubMenuName);
}

void Menu::BoxList() {

}

Menu::~Menu()
{

}
