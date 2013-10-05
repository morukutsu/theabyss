// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_INPUT
#define MK_INPUT

#include "lowSystem.h"
#include <string>

#define BUT_NONE          -1
#define BUT_PUSHED         0
#define BUT_HELD           1
#define BUT_RELEASED       2

#define CNT_KEYBOARD	   0
#define CNT_XBOX     	   1
#define CNT_GAMEPAD        2

#define MAX_BUTTONS		   18
#define MAX_STICKS		   2
#define MAX_PLAYERS        4

// Input : platform specific controller input handling
namespace mk 
{
	struct ButtonNames {
		enum ButtonName {
			X, Y, A, B, Back, Start, LB, LT, RB, RT, LeftStick, RightStick, Up, Down, Left, Right, LeftClick, RightClick
		};
	};
	typedef ButtonNames::ButtonName ButtonName;

	struct StickNames {
		enum StickName {
			Left, Right
		};
	};
	typedef StickNames::StickName StickName;

	struct Button
	{
		bool held, pressed, released;
	};

	struct Stick 
	{
		float magnitude;
		float angle;
	};

	struct Pointer
	{
		float x, y;
	};

	enum {
		EMU_KEY_JOY_UP, EMU_KEY_JOY_DOWN, EMU_KEY_JOY_LEFT, EMU_KEY_JOY_RIGHT
	};

	// Input pour un controleur
	struct Input
    {
		// Pad buttons
		Button buttons[MAX_BUTTONS];

		// Sticks
		Stick sticks[MAX_STICKS];

		// Pointer
		Pointer pointer;

		// Emulation de l'user input du joystick au clavier
		sf::Keyboard::Key emuKeyJoy[4];

		// Si une touche a été appuyée à cette frame
		bool btnPressed;
	};

	// Map pour stocker la correspondance touche / string
	template <typename T>
	class StringBimap
	{
		static_assert(!std::is_same<T, std::string>::value, "Type different from std::string expected");

		public:
			explicit StringBimap(unsigned int valueCount, unsigned int specialValueCount = 0)
			: mStringVector(valueCount + specialValueCount)
			, mTMap()
			, mSpecialValueCount(specialValueCount)
			{
			}

			void insert(T t, const std::string& string)
			{
				mStringVector[t + mSpecialValueCount] = string;
				mTMap[string] = t;
			}

			const std::string& at(T t) const
			{
				std::size_t index = static_cast<std::size_t>(t + mSpecialValueCount);
				// Gérer ce cas ...
				/*if (index >= mStringVector.size() + mSpecialValueCount)
					return "";
				else*/


					return mStringVector[index];
			}

			T at(const std::string& string) const
			{
				auto itr = mTMap.find(string);
				
				// Gestion erreurs
				return itr->second;
			}

		private:
			std::vector<std::string>	mStringVector;
			std::map<std::string, T>	mTMap;
			unsigned int				mSpecialValueCount;
	};

	class InputManager
	{
		public:
			static void Init();
			static void Update();
			static void UpdateMousePosition();
			static Input* GetInput(int player, int device);
			static bool   GetKeyboardKeyPressed(std::string str)
			{
				sf::Keyboard::Key key = toKeyboardKey(str);
				return sf::Keyboard::isKeyPressed(key);
			}

			// String to key conversion
			static std::string toString(sf::Keyboard::Key key)
			{
				return mKeybMap->at(key);
			}

			static sf::Keyboard::Key toKeyboardKey(const std::string& string)
			{
				return mKeybMap->at(string);
			}

			static void setControlKeyboardMapping(int player, ButtonName name, sf::Keyboard::Key key)
			{
				mKeybMapping[player][name] = key;
			}

			static void setEmuKey(int player, int key, sf::Keyboard::Key keybkey) 
			{
				_in[player].emuKeyJoy[key] = keybkey;
			}

		private:
			static Input _in[MAX_PLAYERS];
			static void majPressedReleased(bool oldheld, bool held, bool * pressed, bool * released)
			{
				if(!oldheld && held)
					*pressed = true;
				if(oldheld && !held)
					*released = true;
			}

			static void ResetInput(Input* input) {
				for(int k = 0; k < MAX_BUTTONS; k++) {
					input->buttons[k].held = false;
					input->buttons[k].pressed = false;
					input->buttons[k].released = false;
				}
			}

			static void InitMaps();
			static StringBimap<sf::Keyboard::Key> *mKeybMap;

			static sf::Keyboard::Key mKeybMapping[MAX_PLAYERS][MAX_BUTTONS];
	};
};

#endif