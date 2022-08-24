#include "includes.hpp"

#include <windows.h>
#include <ctime>
#include <iostream>
#include <Settings.h>






bool isTriggerOn = false;
bool isAimOn = false;
bool isBodyTrigger = false;
static int selectedPrimary = 0;
static int selectedSecondary = 0;
static int selectedKey = 0;
int wheelState = 0;
float range = 0;
constexpr float CONFIDENCE_THRESHOLD = 0.5;
constexpr float NMS_THRESHOLD = 0;
constexpr int NUM_CLASSES = 1; //the number of classes you have labeled - I had body and head... so 2
const int width = 2560;
const int height = 1440;
bool isMenu2 = true;


float sen;
float xOffset;
float yOffset;
Key keyList[5];
Weapon weaponList[5];
CurrentWeapon currentWeapon;
Key currentK;
Weapon currentW;


float x;
float y;
HANDLE hSerial;

int count, closest_index, FOV_x = 1200, FOV_y = 700, screen_width = width, screen_height = height;
#define WM_MOUSEWHEEL 0x020A
bool weapon = false;
bool isCv = false;
bool isSonar = true;
bool isConsole = false;
bool isPrimary = true;
int currentWeaponIndex = 0;
bool Detected = false;
bool firstLaunch = true;
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;


float x_array[100];
float y_array[100];
int x_array_index = 0;
float x_average = 0;
float y_average = 0;

bool CreateDeviceD3D(HWND);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace std::chrono_literals;

template <
	class result_t = std::chrono::milliseconds,
	class clock_t = std::chrono::steady_clock,
	class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
	return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}



const cv::Scalar colors[] = {
	{0, 255, 255},
	{255, 255, 0},
	{0, 255, 0},
	{255, 0, 0}
};
const auto NUM_COLORS = sizeof(colors) / sizeof(colors[0]);



void Colors() {
	ImGuiStyle& style = ImGui::GetStyle();


	style.WindowMinSize = ImVec2(960, 590);

	style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_ChildBg] = ImColor(25, 25, 25);

	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);
}

void creerWeapon() {
	//creer un nouveau weapon
	Weapon handCannon;

	handCannon.name = "HandCannon";
	handCannon.weaponId = 0;
	handCannon.xSensitivity = 0;
	handCannon.ySensitivity = 0;

	handCannon.xOffsetClose = 0;
	handCannon.yOffsetClose = 0;

	handCannon.xOffsetMedium = 0;
	handCannon.yOffsetMedium = 0;

	handCannon.xOffsetFar = 0;
	handCannon.yOffsetFar = 0;

	handCannon.xTbOffsetClosePositive = 0;
	handCannon.yTbOffsetClosePositive = 0;

	handCannon.xTbOffsetCloseNegative = 0;
	handCannon.yTbOffsetCloseNegative = 0;

	handCannon.xTbOffsetMediumPositive = 0;
	handCannon.yTbOffsetMediumPositive = 0;

	handCannon.xTbOffsetMediumNegative = 0;
	handCannon.yTbOffsetMediumNegative = 0;

	handCannon.xTbOffsetFarPositive = 0;
	handCannon.yTbOffsetFarPositive = 0;

	handCannon.xTbOffsetFarNegative = 0;
	handCannon.yTbOffsetFarNegative = 0;

	//add handCannon to weaponList
	weaponList[0] = handCannon;

	//creer un nouveau weapon
	Weapon smg;

	smg.name = "SMG";
	smg.weaponId = 1;
	smg.xSensitivity = 0;
	smg.ySensitivity = 0;

	smg.xOffsetClose = 0;
	smg.yOffsetClose = 0;

	smg.xOffsetMedium = 0;
	smg.yOffsetMedium = 0;

	smg.xOffsetFar = 0;
	smg.yOffsetFar = 0;

	smg.xTbOffsetClosePositive = 0;
	smg.yTbOffsetClosePositive = 0;

	smg.xTbOffsetCloseNegative = 0;
	smg.yTbOffsetCloseNegative = 0;

	smg.xTbOffsetMediumPositive = 0;
	smg.yTbOffsetMediumPositive = 0;

	smg.xTbOffsetMediumNegative = 0;
	smg.yTbOffsetMediumNegative = 0;

	smg.xTbOffsetFarPositive = 0;
	smg.yTbOffsetFarPositive = 0;

	smg.xTbOffsetFarNegative = 0;
	smg.yTbOffsetFarNegative = 0;


	//add smg to weaponList
	weaponList[1] = smg;

	//creer un nouveau weapon
	Weapon sniper;

	sniper.name = "Sniper";
	sniper.weaponId = 3;
	sniper.xSensitivity = 0;
	sniper.ySensitivity = 0;

	sniper.xOffsetClose = 0;
	sniper.yOffsetClose = 0;

	sniper.xOffsetMedium = 0;
	sniper.yOffsetMedium = 0;

	sniper.xOffsetFar = 0;
	sniper.yOffsetFar = 0;

	sniper.xTbOffsetClosePositive = 0;
	sniper.yTbOffsetClosePositive = 0;

	sniper.xTbOffsetCloseNegative = 0;
	sniper.yTbOffsetCloseNegative = 0;

	sniper.xTbOffsetMediumPositive = 0;
	sniper.yTbOffsetMediumPositive = 0;

	sniper.xTbOffsetMediumNegative = 0;
	sniper.yTbOffsetMediumNegative = 0;

	sniper.xTbOffsetFarPositive = 0;
	sniper.yTbOffsetFarPositive = 0;

	sniper.xTbOffsetFarNegative = 0;
	sniper.yTbOffsetFarNegative = 0;

	//add sniper to weaponList
	weaponList[2] = sniper;

	//creer un nouveau weapon
	Weapon shotgun;

	shotgun.name = "Shotgun";
	shotgun.weaponId = 4;
	shotgun.xSensitivity = 0;
	shotgun.ySensitivity = 0;

	shotgun.xOffsetClose = 0;
	shotgun.yOffsetClose = 0;

	shotgun.xOffsetMedium = 0;
	shotgun.yOffsetMedium = 0;

	shotgun.xOffsetFar = 0;
	shotgun.yOffsetFar = 0;

	shotgun.xTbOffsetClosePositive = 0;
	shotgun.yTbOffsetClosePositive = 0;

	shotgun.xTbOffsetCloseNegative = 0;
	shotgun.yTbOffsetCloseNegative = 0;

	shotgun.xTbOffsetMediumPositive = 0;
	shotgun.yTbOffsetMediumPositive = 0;

	shotgun.xTbOffsetMediumNegative = 0;
	shotgun.yTbOffsetMediumNegative = 0;

	shotgun.xTbOffsetFarPositive = 0;
	shotgun.yTbOffsetFarPositive = 0;

	shotgun.xTbOffsetFarNegative = 0;
	shotgun.yTbOffsetFarNegative = 0;


	//add shotgun to weaponList
	weaponList[3] = shotgun;

	//creer un nouveau weapon
	Weapon lastWord;

	lastWord.name = "LastWord";
	lastWord.weaponId = 2;
	lastWord.xSensitivity = 0;
	lastWord.ySensitivity = 0;

	lastWord.xOffsetClose = 0;
	lastWord.yOffsetClose = 0;

	lastWord.xOffsetMedium = 0;
	lastWord.yOffsetMedium = 0;

	lastWord.xOffsetFar = 0;
	lastWord.yOffsetFar = 0;

	lastWord.xTbOffsetClosePositive = 0;
	lastWord.yTbOffsetClosePositive = 0;

	lastWord.xTbOffsetCloseNegative = 0;
	lastWord.yTbOffsetCloseNegative = 0;

	lastWord.xTbOffsetMediumPositive = 0;
	lastWord.yTbOffsetMediumPositive = 0;

	lastWord.xTbOffsetMediumNegative = 0;
	lastWord.yTbOffsetMediumNegative = 0;

	lastWord.xTbOffsetFarPositive = 0;
	lastWord.yTbOffsetFarPositive = 0;

	lastWord.xTbOffsetFarNegative = 0;
	lastWord.yTbOffsetFarNegative = 0;

	//add lastWord to weaponList
	weaponList[4] = lastWord;




}

void creerKey() {
	for (int i = 0; i < 5; i++) {
		keyList[i].key = i;
		keyList[i].virtualKey = 1;
		keyList[i].virtualKeyInt = 0;
		keyList[i].sensitivityX = 0;
		keyList[i].sensitivityY = 0;
		keyList[i].xOffset = 0;
		keyList[i].yOffset = 0;
		keyList[i].isAim = false;
		keyList[i].isTrigger = false;
		keyList[i].isBodyTrigger = false;
		keyList[i].isEnable = false;

	}
}

void creerCurrentWeapon() {

	currentWeapon.primaryWeapon = 0;
	currentWeapon.secondaryWeapon = 0;
}

void serializationCurrentWeapon() {



	Json::Value root;


	Json::Value object(Json::objectValue);
	object["primaryWeapon"] = currentWeapon.primaryWeapon;
	object["secondaryWeapon"] = currentWeapon.secondaryWeapon;




	root = object;

	std::ofstream o("configCurrentWeapon.json");
	o << root;
	o.close();

	//std::cout << root << std::endl;


}

void serializationKey() {



	Json::Value root;

	Json::Value objectArray(Json::arrayValue);



	for (int i = 0; i < 5; i++) {
		Json::Value object(Json::objectValue);
		object["key"] = keyList[i].key;
		object["virtualKey"] = keyList[i].virtualKey;
		object["virtualKeyInt"] = keyList[i].virtualKeyInt;
		object["sensitivityX"] = keyList[i].sensitivityX;
		object["sensitivityY"] = keyList[i].sensitivityY;
		object["xOffset"] = keyList[i].xOffset;
		object["yOffset"] = keyList[i].yOffset;
		object["isAim"] = keyList[i].isAim;
		object["isTrigger"] = keyList[i].isTrigger;
		object["isBodyTrigger"] = keyList[i].isBodyTrigger;
		object["isEnable"] = keyList[i].isEnable;
		objectArray.append(object);

	}

	root["keyList"] = objectArray;

	std::ofstream o("configKey.json");
	o << root;
	o.close();

	//std::cout << root << std::endl;


}

void serializationWeapon() {


	Json::Value root;

	Json::Value objectArray(Json::arrayValue);

	for (int i = 0; i < 5; i++) {

		Json::Value object(Json::objectValue);
		object["name"] = weaponList[i].name;
		object["weaponId"] = weaponList[i].weaponId;
		object["xSensitivity"] = weaponList[i].xSensitivity;
		object["ySensitivity"] = weaponList[i].ySensitivity;
		object["xOffsetClose"] = weaponList[i].xOffsetClose;
		object["yOffsetClose"] = weaponList[i].yOffsetClose;
		object["xOffsetMedium"] = weaponList[i].xOffsetMedium;
		object["yOffsetMedium"] = weaponList[i].yOffsetMedium;
		object["xOffsetFar"] = weaponList[i].xOffsetFar;
		object["yOffsetFar"] = weaponList[i].yOffsetFar;
		object["xTbOffsetClosePositive"] = weaponList[i].xTbOffsetClosePositive;
		object["yTbOffsetClosePositive"] = weaponList[i].yTbOffsetClosePositive;
		object["xTbOffsetCloseNegative"] = weaponList[i].xTbOffsetCloseNegative;
		object["yTbOffsetCloseNegative"] = weaponList[i].yTbOffsetCloseNegative;
		object["xTbOffsetMediumPositive"] = weaponList[i].xTbOffsetMediumPositive;
		object["yTbOffsetMediumPositive"] = weaponList[i].yTbOffsetMediumPositive;
		object["xTbOffsetMediumNegative"] = weaponList[i].xTbOffsetMediumNegative;
		object["yTbOffsetMediumNegative"] = weaponList[i].yTbOffsetMediumNegative;
		object["xTbOffsetFarPositive"] = weaponList[i].xTbOffsetFarPositive;
		object["yTbOffsetFarPositive"] = weaponList[i].yTbOffsetFarPositive;
		object["xTbOffsetFarNegative"] = weaponList[i].xTbOffsetFarNegative;
		object["yTbOffsetFarNegative"] = weaponList[i].yTbOffsetFarNegative;
		objectArray.append(object);

	}

	root["keyList"] = objectArray;

	std::ofstream o("configWeapon.json");
	o << root;
	o.close();

	//std::cout << root << std::endl;






}

void deserializationCurrentWeapon() {

	//std::cout << "deserializationCurrentWeapon" << std::endl;

	//open file
	std::ifstream i("configCurrentWeapon.json");
	Json::Value root;
	Json::CharReaderBuilder builder;
	std::string errs;
	bool ok = Json::parseFromStream(builder, i, &root, &errs);
	if (!ok) {
		std::cout << "Failed to parse JSON: " << errs << std::endl;
		return;
	}

	//read file
	currentWeapon.primaryWeapon = root["primaryWeapon"].asFloat();
	currentWeapon.secondaryWeapon = root["secondaryWeapon"].asFloat();



	//close file
	i.close();


}

void deserializationKey() {

	//open file
	std::ifstream i("configKey.json");
	Json::Value root;
	Json::CharReaderBuilder builder;
	std::string errs;
	bool ok = Json::parseFromStream(builder, i, &root, &errs);
	if (!ok) {
		std::cout << "Failed to parse JSON: " << errs << std::endl;
		return;
	}



	//read file
	for (int i = 0; i < 5; i++) {
		keyList[i].key = root["keyList"][i]["key"].asFloat();
		keyList[i].virtualKey = root["keyList"][i]["virtualKey"].asFloat();
		keyList[i].virtualKeyInt = root["keyList"][i]["virtualKeyInt"].asFloat();
		keyList[i].sensitivityX = root["keyList"][i]["sensitivityX"].asFloat();
		keyList[i].sensitivityY = root["keyList"][i]["sensitivityY"].asFloat();
		keyList[i].xOffset = root["keyList"][i]["xOffset"].asFloat();
		keyList[i].yOffset = root["keyList"][i]["yOffset"].asFloat();
		keyList[i].isAim = root["keyList"][i]["isAim"].asBool();
		keyList[i].isTrigger = root["keyList"][i]["isTrigger"].asBool();
		keyList[i].isBodyTrigger = root["keyList"][i]["isBodyTrigger"].asBool();
		keyList[i].isEnable = root["keyList"][i]["isEnable"].asBool();

	}

	//close file
	i.close();


}

void deserializationWeapon() {

	//open file
	std::ifstream i("configWeapon.json");
	Json::Value root;
	Json::CharReaderBuilder builder;
	std::string errs;
	bool ok = Json::parseFromStream(builder, i, &root, &errs);
	if (!ok) {
		std::cout << "Failed to parse JSON: " << errs << std::endl;
		return;
	}



	//read file
	for (int i = 0; i < 5; i++) {

		weaponList[i].name = root["keyList"][i]["name"].asString();
		weaponList[i].weaponId = root["keyList"][i]["weaponId"].asFloat();
		weaponList[i].xSensitivity = root["keyList"][i]["xSensitivity"].asFloat();
		weaponList[i].ySensitivity = root["keyList"][i]["ySensitivity"].asFloat();
		weaponList[i].xOffsetClose = root["keyList"][i]["xOffsetClose"].asFloat();
		weaponList[i].yOffsetClose = root["keyList"][i]["yOffsetClose"].asFloat();
		weaponList[i].xOffsetMedium = root["keyList"][i]["xOffsetMedium"].asFloat();
		weaponList[i].yOffsetMedium = root["keyList"][i]["yOffsetMedium"].asFloat();
		weaponList[i].xOffsetFar = root["keyList"][i]["xOffsetFar"].asFloat();
		weaponList[i].yOffsetFar = root["keyList"][i]["yOffsetFar"].asFloat();
		weaponList[i].xTbOffsetClosePositive = root["keyList"][i]["xTbOffsetClosePositive"].asFloat();
		weaponList[i].yTbOffsetClosePositive = root["keyList"][i]["yTbOffsetClosePositive"].asFloat();
		weaponList[i].xTbOffsetCloseNegative = root["keyList"][i]["xTbOffsetCloseNegative"].asFloat();
		weaponList[i].yTbOffsetCloseNegative = root["keyList"][i]["yTbOffsetCloseNegative"].asFloat();
		weaponList[i].xTbOffsetMediumPositive = root["keyList"][i]["xTbOffsetMediumPositive"].asFloat();
		weaponList[i].yTbOffsetMediumPositive = root["keyList"][i]["yTbOffsetMediumPositive"].asFloat();
		weaponList[i].xTbOffsetMediumNegative = root["keyList"][i]["xTbOffsetMediumNegative"].asFloat();
		weaponList[i].yTbOffsetMediumNegative = root["keyList"][i]["yTbOffsetMediumNegative"].asFloat();
		weaponList[i].xTbOffsetFarPositive = root["keyList"][i]["xTbOffsetFarPositive"].asFloat();
		weaponList[i].yTbOffsetFarPositive = root["keyList"][i]["yTbOffsetFarPositive"].asFloat();
		weaponList[i].xTbOffsetFarNegative = root["keyList"][i]["xTbOffsetFarNegative"].asFloat();
		weaponList[i].yTbOffsetFarNegative = root["keyList"][i]["yTbOffsetFarNegative"].asFloat();

	}

	//close file
	i.close();


}


void keyUserPick(int p_key, int p_userPick) {




	if (p_userPick == 0) {
		//std::cout << "LButton" << std::endl;
		keyList[p_key].virtualKey = 0;
		keyList[p_key].virtualKeyInt = 1;
	}
	if (p_userPick == 1) {
		//std::cout << "RButton" << std::endl;
		keyList[p_key].virtualKey = 1;
		keyList[p_key].virtualKeyInt = 2;
	}
	if (p_userPick == 2) {
		//std::cout << "Xbutton1" << std::endl;

		keyList[p_key].virtualKey = 2;
		keyList[p_key].virtualKeyInt = 5;
	}
	if (p_userPick == 3) {
		//std::cout << "Xbutton2" << std::endl;

		keyList[p_key].virtualKey = 3;
		keyList[p_key].virtualKeyInt = 6;
	}
	if (p_userPick == 4) {
		//std::cout << "LShift" << std::endl;

		keyList[p_key].virtualKey = 4;
		keyList[p_key].virtualKeyInt = 160;
	}
	if (p_userPick == 5) {
		//std::cout << "Alt" << std::endl;

		keyList[p_key].virtualKey = 5;
		keyList[p_key].virtualKeyInt = 18;
	}
	if (p_userPick == 6) {
		//std::cout << "LControl" << std::endl;

		keyList[p_key].virtualKey = 6;
		keyList[p_key].virtualKeyInt = 162;
	}



}

void afficherMenuKey(int p_key) {


	ImGui::Separator();


	static const char* primary[]{
		"Hand Cannon",
		"SMG/Auto",
		"Last Word",
		"Sniper",
		"Shotgun"
	};

	selectedPrimary = currentWeapon.primaryWeapon;
	selectedSecondary = currentWeapon.secondaryWeapon;


	if (ImGui::Combo("Primary", &selectedPrimary, primary, IM_ARRAYSIZE(primary), 4)) {
		currentWeapon.primaryWeapon = selectedPrimary;
		serializationCurrentWeapon();
	}





	if (ImGui::Combo("Secondary", &selectedSecondary, primary, IM_ARRAYSIZE(primary), 4)) {
		currentWeapon.secondaryWeapon = selectedSecondary;
		serializationCurrentWeapon();
	}

	ImGui::Separator();




	static const char* keys[]{

		"LButton",
		"RButton",
		"XButton1",
		"XButton2",
		"LShift",
		"Alt",
		"LCtrl",
	};



	selectedKey = keyList[p_key].virtualKey;




	ImGui::Text(("AimKey " + std::to_string(p_key)).c_str());
	if (ImGui::Combo("Key", &selectedKey, keys, IM_ARRAYSIZE(keys), 7)) {
		keyUserPick(p_key, selectedKey);
	}

	//make a checkbox pour enable la Key
	ImGui::Checkbox("Enable", &keyList[p_key].isEnable);
	ImGui::InputFloat("SensitivityX", &keyList[p_key].sensitivityX);
	ImGui::InputFloat("SensitivityY", &keyList[p_key].sensitivityY);
	ImGui::InputFloat("X Offset", &keyList[p_key].xOffset);
	ImGui::InputFloat("Y Offset", &keyList[p_key].yOffset);
	ImGui::Checkbox("Is Aim", &keyList[p_key].isAim);
	ImGui::Checkbox("Is Trigger", &keyList[p_key].isTrigger);
	ImGui::Checkbox("Is Body Trigger", &keyList[p_key].isBodyTrigger);

	//apply button
	if (ImGui::Button("Apply"))
	{
		serializationKey();
		serializationCurrentWeapon();

	}
}

void afficherMenuWeapon(int p_key) {






	ImGui::Text(weaponList[p_key].name.c_str());
	ImGui::Separator();


	ImGui::Text("Sensitivity Multiplier");
	ImGui::InputFloat("##label Sensitivity x", &weaponList[p_key].xSensitivity);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Sensitivity y", &weaponList[p_key].ySensitivity);
	ImGui::SameLine();
	ImGui::Text("Y");

	ImGui::Separator();
	ImGui::Text("Ab Offset Close");
	ImGui::InputFloat("##label Ab Offset Close x", &weaponList[p_key].xOffsetClose);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Ab Offset Close y", &weaponList[p_key].yOffsetClose);
	ImGui::SameLine();
	ImGui::Text("Y");

	ImGui::Separator();
	ImGui::Text("Ab Offset Medium");
	ImGui::InputFloat("##label Ab Offset Medium x ", &weaponList[p_key].xOffsetMedium);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Ab Offset Medium y", &weaponList[p_key].yOffsetMedium);
	ImGui::SameLine();
	ImGui::Text("Y");

	ImGui::Separator();
	ImGui::Text("Ab Offset Far");
	ImGui::InputFloat("##label Ab Offset Far x", &weaponList[p_key].xOffsetFar);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Ab Offset Far y", &weaponList[p_key].yOffsetFar);
	ImGui::SameLine();
	ImGui::Text("Y");

	ImGui::Separator();
	ImGui::Text("Tb Offset Close");
	ImGui::InputFloat("##label Tb Offset Close x", &weaponList[p_key].xTbOffsetClosePositive);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Tb Offset Close -x", &weaponList[p_key].xTbOffsetCloseNegative);
	ImGui::SameLine();
	ImGui::Text("-X");
	ImGui::InputFloat("##label Tb Offset Close y ", &weaponList[p_key].yTbOffsetClosePositive);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::InputFloat("##label Tb Offset Close -y", &weaponList[p_key].yTbOffsetCloseNegative);
	ImGui::SameLine();
	ImGui::Text("-Y");
	ImGui::Separator();

	ImGui::Text("Tb Offset Medium");
	ImGui::InputFloat("##label Tb Offset Medium x", &weaponList[p_key].xTbOffsetMediumPositive);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Tb Offset Medium -x", &weaponList[p_key].xTbOffsetMediumNegative);
	ImGui::SameLine();
	ImGui::Text("-X");
	ImGui::InputFloat("##label Tb Offset Medium y", &weaponList[p_key].yTbOffsetMediumPositive);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::InputFloat("##label Tb Offset Medium -y", &weaponList[p_key].yTbOffsetMediumNegative);
	ImGui::SameLine();
	ImGui::Text("-Y");

	ImGui::Separator();
	ImGui::Text("Tb Offset Far");
	ImGui::InputFloat("##label Tb Offset Far x", &weaponList[p_key].xTbOffsetFarPositive);
	ImGui::SameLine();
	ImGui::Text("X");
	ImGui::InputFloat("##label Tb Offset Far -x", &weaponList[p_key].xTbOffsetFarNegative);
	ImGui::SameLine();
	ImGui::Text("-X");
	ImGui::InputFloat("##label Tb Offset Far y", &weaponList[p_key].yTbOffsetFarPositive);
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::InputFloat("##label Tb Offset Far -y", &weaponList[p_key].yTbOffsetFarNegative);
	ImGui::SameLine();
	ImGui::Text("-Y");

	ImGui::Separator();


	//apply button
	if (ImGui::Button("Apply"))
	{
		serializationWeapon();

	}
}




void currentWeaponPick(int p_currentKey, bool isPrimary) {




	int currentWeaponPick = 0;

	if (isPrimary == true) {

		//std::cout << "Primary Weapon : " << currentWeapon.primaryWeapon << std::endl;
		currentWeaponPick = currentWeapon.primaryWeapon;


	}
	else {
		//std::cout << "Secondary Weapon : " << currentWeapon.secondaryWeapon << std::endl;
		currentWeaponPick = currentWeapon.secondaryWeapon;
	}

	for (int i = 0; i < 5; i++) {
		if (weaponList[i].weaponId == currentWeaponPick) {

			currentW.xSensitivity = weaponList[i].xSensitivity;
			currentW.ySensitivity = weaponList[i].ySensitivity;
			currentW.xOffsetClose = weaponList[i].xOffsetClose;
			currentW.yOffsetClose = weaponList[i].yOffsetClose;
			currentW.xOffsetMedium = weaponList[i].xOffsetMedium;
			currentW.yOffsetMedium = weaponList[i].yOffsetMedium;
			currentW.xOffsetFar = weaponList[i].xOffsetFar;
			currentW.yOffsetFar = weaponList[i].yOffsetFar;
			currentW.xTbOffsetClosePositive = weaponList[i].xTbOffsetClosePositive;
			currentW.yTbOffsetClosePositive = weaponList[i].yTbOffsetClosePositive;
			currentW.xTbOffsetCloseNegative = weaponList[i].xTbOffsetCloseNegative;
			currentW.yTbOffsetCloseNegative = weaponList[i].yTbOffsetCloseNegative;
			currentW.xTbOffsetMediumPositive = weaponList[i].xTbOffsetMediumPositive;
			currentW.yTbOffsetMediumPositive = weaponList[i].yTbOffsetMediumPositive;
			currentW.xTbOffsetMediumNegative = weaponList[i].xTbOffsetMediumNegative;
			currentW.yTbOffsetMediumNegative = weaponList[i].yTbOffsetMediumNegative;
			currentW.xTbOffsetFarPositive = weaponList[i].xTbOffsetFarPositive;
			currentW.yTbOffsetFarPositive = weaponList[i].yTbOffsetFarPositive;
			currentW.xTbOffsetFarNegative = weaponList[i].xTbOffsetFarNegative;
			currentW.yTbOffsetFarNegative = weaponList[i].yTbOffsetFarNegative;
			currentW.name = weaponList[i].name;
			currentW.weaponId = weaponList[i].weaponId;

		}
	}


	for (int i = 0; i < 5; i++) {
		if (keyList[i].virtualKeyInt == p_currentKey && keyList[i].isEnable == true) {
			currentK.sensitivityX = keyList[i].sensitivityX;
			currentK.sensitivityY = keyList[i].sensitivityY;
			currentK.xOffset = keyList[i].xOffset;
			currentK.yOffset = keyList[i].yOffset;
			currentK.isAim = keyList[i].isAim;
			currentK.isTrigger = keyList[i].isTrigger;
			currentK.isBodyTrigger = keyList[i].isBodyTrigger;
			currentWeapon.isPressed = true;
		}

		//if not enabled currentWeapon.isPressed = false
		if (keyList[i].virtualKeyInt == p_currentKey && keyList[i].isEnable == false) {
			currentWeapon.isPressed = false;
		}

	}



	//std::cout << currentWeapon.isPressed << std::endl;
	currentW.xSensitivity = currentK.sensitivityX * currentW.xSensitivity;
	currentW.ySensitivity = currentK.sensitivityY * currentW.ySensitivity;

}

void getWheelState(int p_state) {

	if (p_state == 1) {
		isPrimary = true;
	}
	if (p_state == -1) {
		isPrimary = false;
	}

	//std::cout << "Wheel State : " << isPrimary << std::endl;

}

int StartRendering()
{

	const auto title = utils::random_string();
	utils::spoof_exe(false);

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		title.c_str(),
		0
	};

	RegisterClassExA(&wc);
	HWND hwnd = CreateWindowExA(0, wc.lpszClassName, title.c_str(), WS_SYSMENU, 100, 100, 1050, 700, 0, 0, wc.hInstance, 0);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)hwnd);

	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		UnregisterClassA(wc.lpszClassName, wc.hInstance);

		return 1;
	}






	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	io.WantSaveIniSettings = false;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 32);

	Colors();


	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	deserializationKey();
	deserializationCurrentWeapon();
	deserializationWeapon();

	while (true)
	{
		MSG msg;



		while (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				break;
		}






		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{

			int aimkey = 0;
			currentWeapon.isPressed = false;


			if (GetKeyState(5) & 0x8000)
			{
				aimkey = 5;



			}
			else if (GetKeyState(6) & 0x8000)
			{

				aimkey = 6;


			}
			else if (GetKeyState(160) & 0x8000)
			{

				aimkey = 160;

			}
			else if (GetKeyState(18) & 0x8000)
			{

				aimkey = 18;

			}
			else if (GetKeyState(162) & 0x8000)
			{

				aimkey = 162;


			}
			else if (GetKeyState(2) & 0x8000)
			{

				aimkey = 2;











			}
			else if (GetKeyState(1) & 0x8000)
			{
				aimkey = 1;

			}

			currentWeaponPick(aimkey, isPrimary);




			if (GetKeyState(0x22) & 0x8000) {

				Sleep(150);

				isSonar = !isSonar;
				//add a sleep

			}




			if (isSonar == true) {
				ShowWindow(hwnd, SW_SHOW);
			}
			else {
				ShowWindow(hwnd, SW_HIDE);



			}





			static bool isAimKeyButtonPressed = false;
			static bool isWeaponMenuPressed = false;
			static bool onLoad = true;



			ImGui::Begin("SONAR1.0", 0, ImGuiWindowFlags_NoTitleBar);







			ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
			ImGui::BeginChild("##LeftSide", ImVec2(240, ImGui::GetContentRegionAvail().y), true);
			{






				if (ImGui::Button("Aim Key")) {
					isAimKeyButtonPressed = true;
					isWeaponMenuPressed = false;


				}

				if (ImGui::Button("Weapon Settings")) {
					isAimKeyButtonPressed = false;
					isWeaponMenuPressed = true;
					firstLaunch == false;

				}

				if (ImGui::Button("Show CV Window")) {
					isCv = !isCv;
				}


				if (ImGui::Button("Show Console")) {
					isConsole = !isConsole;
				}




				if (isConsole == true) {
					ShowWindow(GetConsoleWindow(), SW_SHOW);

				}
				else {
					ShowWindow(GetConsoleWindow(), SW_HIDE);
				}




			}


			ImGui::EndChild();

			{
				ImGui::SameLine(0);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
				ImGui::SameLine();
			}

			ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
			{
				if (isAimKeyButtonPressed == true)
				{


					for (int i = 0; i < 5; i++)
					{

						ImGui::PushID(i);
						if (ImGui::Button(("AimKey " + std::to_string(i)).c_str()))
						{
							Settings::Tab = i;

						}
						ImGui::SameLine();
						ImGui::PopID();
					}
					ImGui::NewLine();
					afficherMenuKey(Settings::Tab);

				}

				if (isWeaponMenuPressed == true) {
					ImGui::Text("Weapon Settings");
					ImGui::NewLine;
					ImGui::Separator();



					if (ImGui::Button("Hand Cannon"))
					{
						Settings::Tab = 0;

					}
					ImGui::SameLine();
					if (ImGui::Button("SMG/Auto"))
					{
						Settings::Tab = 1;

					}
					ImGui::SameLine();
					if (ImGui::Button("Sniper"))
					{
						Settings::Tab = 2;

					}
					ImGui::SameLine();
					if (ImGui::Button("Shotgun"))
					{
						Settings::Tab = 3;

					}
					ImGui::SameLine();
					if (ImGui::Button("LastWord"))
					{
						Settings::Tab = 4;

					}


					afficherMenuWeapon(Settings::Tab);










				}


				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::End();
			}




			ImGui::Render();

			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			g_pSwapChain->Present(0, 0);


		}
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClassA(wc.lpszClassName, wc.hInstance);

	return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		ExitProcess(0);
		PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}



void aimGui() {



	while (true) {


		if (Detected == true) {

			int rangeBool = 0;
			float AimX = x;
			float AimY = y;
			float sensitivityX = currentW.xSensitivity;
			float sensitivityY = currentW.ySensitivity;
			float DirX = 0;
			float DirY = 0;
			float xCenter = width / 2;
			float yCenter = height / 2;
			std::string move;
			std::string click;
			std::string send;

			float xTrigger = 0;
			float x2Trigger = 0;
			float yTrigger = 0;
			float y2Trigger = 0;
			float rangePercent = (range - 260) / (330 - 260);
			float yOffset = 0;
			float AimOffsetX = 0;
			bool falsePositive = false;
			bool inRange = false;

			if (currentW.name == "SMG") {
				currentK.isTrigger = false;
				currentK.isAim = true;
			}



			if (range < 190 || range > 400) {

				AimX = 0;
				AimY = 0;
				falsePositive = true;
			}
			else {

				falsePositive = false;
			}





			if (rangePercent > 1) {
				rangePercent = 1;
			}
			if (rangePercent < 0) {
				rangePercent = 0;
			}


			float rangeDiff = 0;


			yOffset = currentK.yOffset + currentW.yOffsetClose;
			rangeDiff = (yOffset * rangePercent);
			yOffset = yOffset + rangeDiff;


			std::cout << yOffset << std::endl;






			/*std::cout << "range : " << range << std::endl;
			std::cout << "percent : " << rangePercent << std::endl;*/





			








			if (Detected == true) {
				AimX = AimX + currentK.xOffset - currentW.xOffsetClose;
				AimY = AimY - yOffset;
			}
			else {
				AimX = 0;
				AimY = 0;
			}



			if (currentW.name == "Sniper") {


				if (AimX > 400 || AimX < -400 || AimY > 400 || AimY < -400) {
					inRange = false;
				}
				else {
					//std::cout << "IN RANGE : " << AimX << " : " << AimY << std::endl;
					inRange = true;
				}


			}
			else {
				if (AimX > 250 || AimX < -250 || AimY > 250 || AimY < -300) {
					inRange = false;
				}
				else {
					//std::cout << "IN RANGE : " << AimX << " : " << AimY << std::endl;
					inRange = true;
				}
			}


			/*std::cout << "Sensitivity X : " << currentW.xSensitivity << " Sensitivity Y : " << currentW.ySensitivity << std::endl;




			std::cout << "isPressed " << currentWeapon.isPressed << std::endl;


			std::cout << "range: " << range << std::endl;

			std::cout << currentW.name << std::endl;*/

			//std::cout << "Weapon " << currentW.name << std::endl;






			//if (range < 190 || range > 400) {
			//
			//	AimX = 0;
			//	AimY = 0;
			//	falsePositive = true;
			//}
			//else {
			//	
			//	falsePositive = false;
			//}






			//
			//if (rangePercent > 1) {
			//	rangePercent = 1;
			//}
			//else if (rangePercent < 0) {
			//	rangePercent = 0;
			//}
			//
			////sensitivityY = sensitivityY - (sensitivityY * rangePercent);


			//
			///*if (currentW.name == "Sniper") {
			//	rangePercent = rangePercent * 75;
			//	yOffset = currentW.yOffsetClose * (rangePercent);
			//	

			//}
			//else {
			//	
			//	yOffset = currentW.yOffsetClose * (rangePercent);
			//}*/
			//



			//	
			//
			//



			//

			//if (currentW.name == "Sniper") {



			//	if (range < 275) {
			//		yOffset = yOffset - 15;
			//	}
			//	
			//	else if (range > 320) {
			//		std::cout << std::endl;
			//		yOffset = yOffset + 25;
			//		AimY = yOffset + AimY;
			//	}


			//	if (AimX > 400 || AimX < -400 || AimY > 400 || AimY < -400) {
			//		inRange = false;
			//	}
			//	else {
			//		//std::cout << "IN RANGE : " << AimX << " : " << AimY << std::endl;
			//		inRange = true;
			//	}

			//	
			//}
			//else {
			//	if (AimX > 125 || AimX < -125 || AimY > 200 || AimY < -175) {
			//		inRange = false;
			//	}
			//	else {
			//		//std::cout << "IN RANGE : " << AimX << " : " << AimY << std::endl;
			//		inRange = true;
			//	}
			//}




			//if (currentW.name == "HandCannon")
			//{
			//	if (range > 320) {
			//		std::cout << std::endl;
			//		yOffset = yOffset + 50;
			//	}
			//	
			//}

			//AimY = AimY + yOffset;



			if (currentK.isAim == true && currentWeapon.isPressed == true && currentK.isTrigger == false && falsePositive == false && inRange == true)
			{


				/*	std::cout << "AimX : " << AimX << std::endl;
					std::cout << "AimY : " << AimY << std::endl;
					std::cout << "AIM ONLY " << std::endl;*/


					/*if (AimX > 135 || AimX < -135)
					{

						DirX = sensitivityX / 16;
						DirY = sensitivityY / 16;

					}
					else if (AimX > 90 || AimX < -90)
					{

						DirX = sensitivityX / 11;
						DirY = sensitivityY / 11;

					}
					else if (AimX > 15 || AimX < -15)
					{

						DirX = sensitivityX / 10;
						DirY = sensitivityY / 10;

					}
					else {



					}*/


				DirX = sensitivityX / 9;
				DirY = sensitivityY / 9;









				float AimOffsetX = (AimX * DirX) * DirX;
				float AimOffsetY = (AimY * DirY) * DirY;

				AimOffsetX = floor(AimOffsetX * 1);
				AimOffsetY = floor(AimOffsetY * 1);


				AimX = AimOffsetX;
				AimY = AimOffsetY;

				int x = AimX;
				int y = AimY;

				//std::cout << "x : " << AimX << " y : " << AimY << std::endl;



				/*std::cout << "-------------------------- - " << std::endl;
				std::cout << "aim at x : " << x << " y : " << y << std::endl;*/
				move = std::to_string(x) + ":" + std::to_string(y);



				click = ";0";
				send = move + click;

				WriteFile(hSerial, &send, sizeof(move), NULL, NULL);
				//sleep 2ms
				std::this_thread::sleep_for(std::chrono::milliseconds(2));







			}

			if (currentK.isTrigger == true && currentK.isAim == false && currentWeapon.isPressed == true && currentW.name != "SMG" && currentW.name != "Sniper" && falsePositive == false && inRange == true)
			{
				std::cout << "TRIGGER ONLY " << std::endl;

				std::cout << "range : " << range << std::endl;
				if (range < 275) {

					xTrigger = currentW.xTbOffsetFarPositive;
					x2Trigger = currentW.xTbOffsetFarNegative;
					yTrigger = currentW.yTbOffsetFarPositive;
					y2Trigger = currentW.yTbOffsetFarNegative;
					std::cout << "FAR" << std::endl;

				}
				else if (range < 320 && range > 275) {
					xTrigger = currentW.xTbOffsetMediumPositive;
					x2Trigger = currentW.xTbOffsetMediumNegative;
					yTrigger = currentW.yTbOffsetMediumPositive;
					y2Trigger = currentW.yTbOffsetMediumNegative;
					std::cout << "Medium" << std::endl;
				}
				else {
					xTrigger = currentW.xTbOffsetClosePositive;
					x2Trigger = currentW.xTbOffsetCloseNegative;
					yTrigger = currentW.yTbOffsetClosePositive;
					y2Trigger = currentW.yTbOffsetCloseNegative;
					std::cout << "Close" << std::endl;

				}

				//std::cout << "X : " << AimX << " Y : " << AimY << std::endl;

				if (AimY > y2Trigger && AimY < yTrigger && AimX > x2Trigger && AimX < xTrigger)
				{


					click = ";1";
					send = "0:0" + click;
					std::cout << "CLICKED" << std::endl;

					WriteFile(hSerial, &send, sizeof(move), NULL, NULL);



				}
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}

			if (currentK.isAim == true && currentWeapon.isPressed == true && currentK.isTrigger == true && currentW.name != "SMG" && falsePositive == false && inRange == true) {

				//std::cout << "AIM AND TRIGGER " << std::endl;

				if (range < 285) {

					xTrigger = currentW.xTbOffsetFarPositive;
					x2Trigger = currentW.xTbOffsetFarNegative;
					yTrigger = currentW.yTbOffsetFarPositive;
					y2Trigger = currentW.yTbOffsetFarNegative;
					std::cout << "FAR" << std::endl;

				}
				else if (range < 320 && range > 285) {
					xTrigger = currentW.xTbOffsetMediumPositive;
					x2Trigger = currentW.xTbOffsetMediumNegative;
					yTrigger = currentW.yTbOffsetMediumPositive;
					y2Trigger = currentW.yTbOffsetMediumNegative;
					std::cout << "Medium" << std::endl;
				}
				else {
					xTrigger = currentW.xTbOffsetClosePositive;
					x2Trigger = currentW.xTbOffsetCloseNegative;
					yTrigger = currentW.yTbOffsetClosePositive;
					y2Trigger = currentW.yTbOffsetCloseNegative;
					std::cout << "Close" << std::endl;

				}

				if (AimY > y2Trigger && AimY < yTrigger && AimX > x2Trigger && AimX < xTrigger && currentW.name != "Sniper")
				{


					click = ";1";
					send = "0:0" + click;
					std::cout << "CLICKED" << std::endl;

					WriteFile(hSerial, &send, sizeof(move), NULL, NULL);



				}






				DirX = sensitivityX / 9;
				DirY = sensitivityY / 9;









				float AimOffsetX = (AimX * DirX) * DirX;
				float AimOffsetY = (AimY * DirY) * DirY;

				AimOffsetX = floor(AimOffsetX * 1);
				AimOffsetY = floor(AimOffsetY * 1);


				AimX = AimOffsetX;
				AimY = AimOffsetY;

				int x = AimX;
				int y = AimY;

				//std::cout << "x : " << AimX << " y : " << AimY << std::endl;



				/*std::cout << "-------------------------- - " << std::endl;
				std::cout << "aim at x : " << x << " y : " << y << std::endl;*/
				move = std::to_string(x) + ":" + std::to_string(y);



				click = ";0";
				send = move + click;

				WriteFile(hSerial, &send, sizeof(move), NULL, NULL);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));











			}


			//if currentK.isBody == true
			if (currentK.isBodyTrigger == true && currentWeapon.isPressed == true && currentK.isAim == false && currentK.isTrigger == false && falsePositive == false && inRange == true) {
				std::cout << "BODY SHOT " << std::endl;


				AimY = AimY + 100;


				DirX = sensitivityX / 2;
				DirY = sensitivityY / 2;

				float AimOffsetX = (AimX * DirX) * DirX;
				float AimOffsetY = (AimY * DirY) * DirY;

				AimOffsetX = floor(AimOffsetX * 1);
				AimOffsetY = floor(AimOffsetY * 1);


				AimX = AimOffsetX;
				AimY = AimOffsetY;

				int x = AimX;
				int y = AimY;




				move = std::to_string(x) + ":" + std::to_string(y);



				click = ";0";
				send = move + click;

				WriteFile(hSerial, &send, sizeof(move), NULL, NULL);
				Sleep(10);





			}





		}


	}
}

int fps(int p_fps) {


	//create a fps array to store the last 10 fps values to calculate the average
	static int fpsArray[50] = { 0 };

	// add p_fps to the array
	for (int i = 0; i < 49; i++)
		fpsArray[i] = fpsArray[i + 1];

	fpsArray[49] = p_fps;

	//calculate the average
	int sum = 0;
	for (int i = 0; i < 50; i++)
		sum += fpsArray[i];

	//display the average
	int average = sum / 50;
	//std::cout << "FPS: " << average << std::endl;

	return average;

}


int project()
{



	//setup variables
	cv::Mat blob, frame_RGB, frame_RGBA;
	cv::Point closest, current;
	std::vector<cv::Mat> detections;
	std::vector<std::vector<int>> results, clear_results(20, std::vector<int>(4)); //20 x 4
	std::string move;
	std::wstring comport;

	SetConsoleTitle(TEXT("SSH"));

	//read classes file
	std::vector<std::string> class_names;
	{
		std::ifstream class_file("C:\\Ecole Tower\\YOLO\\openCv\\OpenCVExample\\Weights\\Destiny\\v4\\coco-dataset.labels");
		std::string line;

		while (std::getline(class_file, line))
		{
			class_names.push_back(line);
		}
	}





	//setup screencap
	HBITMAP hBitmap;

	HDC hdcSys = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcSys);
	void* ptrBitmapPixels;

	BITMAPINFO bi;
	HDC hdc;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = FOV_x;
	bi.bmiHeader.biHeight = -FOV_y;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	hdc = GetDC(NULL);
	hBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &ptrBitmapPixels, NULL, 0);

	SelectObject(hdcMem, hBitmap);
	frame_RGBA = cv::Mat(FOV_y, FOV_x, CV_8UC4, ptrBitmapPixels, 0);


	//setup cv::dnn
	auto net = cv::dnn::readNetFromDarknet("C:\\Ecole Tower\\YOLO\\openCv\\OpenCVExample\\Weights\\Destiny\\v4\\yolov4-tiny.cfg", "C:\\Ecole Tower\\YOLO\\openCv\\OpenCVExample\\Weights\\Destiny\\v4\\yolov4-tiny_best.weights");
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	auto output_names = net.getUnconnectedOutLayersNames();


	//setup, connect to arduino
	comport = L"COM6";



	hSerial = CreateFile("\\\\.\\COM6", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	GetCommState(hSerial, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	SetCommState(hSerial, &dcbSerialParams);







	while (cv::waitKey(1) < 1)
	{
		//start FPS clock
		auto start = std::chrono::high_resolution_clock::now();
		count = 0;

		results = clear_results;

		//update the pixels / refresh frame
		BitBlt(hdcMem, 0, 0, FOV_x, FOV_y, hdcSys, screen_width / 2 - FOV_x / 2, screen_height / 2 - FOV_y / 2, SRCCOPY);
		cv::cvtColor(frame_RGBA, frame_RGB, cv::COLOR_RGBA2RGB);


		//construct blob from pixels and pass it through
		cv::dnn::blobFromImage(frame_RGB, blob, 0.00392, cv::Size(256, 256), cv::Scalar(), true, false, CV_32F);
		net.setInput(blob);
		net.forward(detections, output_names);


		//from model output draw dettections
		std::vector<int> indices[NUM_CLASSES];
		std::vector<cv::Rect> boxes[NUM_CLASSES];
		std::vector<float> scores[NUM_CLASSES];


		//extracting detections from model's output
		for (auto& output : detections)
		{
			const auto num_boxes = output.rows;
			for (int i = 0; i < num_boxes; i++)
			{

				auto x = output.at<float>(i, 0) * frame_RGB.cols;
				auto y = output.at<float>(i, 1) * frame_RGB.rows;
				auto width = output.at<float>(i, 2) * frame_RGB.cols;
				auto height = output.at<float>(i, 3) * frame_RGB.rows;
				cv::Rect rect(x - width / 2, y - height / 2, width, height);

				for (int c = 0; c < NUM_CLASSES; c++)
				{
					auto confidence = *output.ptr<float>(i, 5 + c);
					if (confidence >= CONFIDENCE_THRESHOLD)
					{

						boxes[c].push_back(rect);
						scores[c].push_back(confidence);
					}
				}
			}
		}


		for (int c = 0; c < NUM_CLASSES; c++) {
			cv::dnn::NMSBoxes(boxes[c], scores[c], 0.0, NMS_THRESHOLD, indices[c]);
		}


		for (int c = 0; c < NUM_CLASSES; c++)
		{
			for (size_t i = 0; i < indices[c].size(); ++i)
			{



				const auto color = colors[c % NUM_COLORS];

				auto idx = indices[c][i];
				const auto& rect = boxes[c][idx];
				cv::rectangle(frame_RGB, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), color, 3);

				results[count][0] = rect.x;
				results[count][1] = rect.y;
				results[count][2] = rect.width;
				results[count][3] = rect.height;
				range = rect.width;

				count = count + 1;

				std::ostringstream label_ss;
				label_ss << class_names[c] << ": " << std::fixed << std::setprecision(2) << scores[c][idx];
				auto label = label_ss.str();

				int baseline;
				auto label_bg_sz = cv::getTextSize(label.c_str(), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, 1, &baseline);
				cv::rectangle(frame_RGB, cv::Point(rect.x, rect.y - label_bg_sz.height - baseline - 10), cv::Point(rect.x + label_bg_sz.width, rect.y), color, cv::FILLED);
				cv::putText(frame_RGB, label.c_str(), cv::Point(rect.x, rect.y - baseline - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 0));



			}
		}





		//calculating closest enemy  (by distance from center)
		closest = cv::Point(FOV_x, FOV_y);

		for (int i = 0; i < count; i++)
		{
			current = cv::Point(results[i][0] + results[i][2] / 2, results[i][1] + results[i][3] / 2);

			if (abs(sqrt(pow(current.x - FOV_x / 2, 2) + pow(current.y - FOV_y / 2, 2))) < abs(sqrt(pow(closest.x - FOV_x / 2, 2) + pow(closest.y - FOV_y / 2, 2))))
			{
				closest = current;
				closest_index = i;


			}


		}

		//calculating mouse move
		move = std::to_string(results[closest_index][0] + results[closest_index][2] / 2 - FOV_x / 2) + ":" + std::to_string(results[closest_index][1] + results[closest_index][3] / 2 - FOV_y / 2);

		float x1 = results[closest_index][0];
		float x2 = results[closest_index][2];
		float y1 = results[closest_index][1];
		float y2 = results[closest_index][3];








		float xTempo = (results[closest_index][0] + results[closest_index][2] / 2 - FOV_x / 2);
		float yTempo = (results[closest_index][1] + results[closest_index][3] / 2 - FOV_y / 2);


		x = xTempo;
		y = yTempo;











		if (move != std::to_string(FOV_x / 2 * -1) + ":" + std::to_string(FOV_y / 2 * -1))
		{

			Detected = true;



		}
		else {

			Detected = false;
			x = 0;
			y = 0;
			/*x_average = 0;
			y_average = 0;*/


		}
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		int fpsInt = 1000000 / duration.count();

		int fpsAvg = fps(fpsInt);
		cv::putText(frame_RGB, std::to_string(fpsAvg), cv::Point(10, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0, 0, 255));



		if (isCv == true) {
			cv::imshow("", frame_RGB);

		}
		else {
			cv::destroyAllWindows();
		}



	}


}