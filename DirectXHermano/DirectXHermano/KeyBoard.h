#pragma once

#include <queue>
#include <bitset> //Like vector but optimized for bool values 

class KeyBoard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum EVENT_TYPE
		{
			PRESS,
			RELEASE,
			INVALID
		};

	private:
		EVENT_TYPE type;
		unsigned char key_code;

	public:
		Event() : type(EVENT_TYPE::INVALID), key_code(0u) {}
		Event(EVENT_TYPE _type, unsigned char _code) : type(_type), key_code(_code) {}

		bool IsPressed() const { return type == EVENT_TYPE::PRESS;  }
		bool IsReleased() const { return type == EVENT_TYPE::RELEASE; }
		bool IsValid() const { return type != EVENT_TYPE::INVALID; }

		unsigned char GetCode() const { return key_code; }
	};

public:
	KeyBoard() = default;
	KeyBoard(const KeyBoard&) = delete;
	KeyBoard& operator=(const KeyBoard&) = delete;

	//Keys
	bool KeyIsPressed(unsigned char keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	void ClearKeyEvents();

	//Char aka for text input
	char ReadChar();
	bool CharIsEmpty() const;
	void ClearChar();

	void ClearAll();

	//key Repeat state
	void SetRepeatState(bool state) { auto_repeat = state; }
	bool IsRepeatEnabled() const { return auto_repeat; }

private:
	//Things that window will modify
	void OnKeyPressed(unsigned char key_code);
	void OnKeyReleased(unsigned char key_code);
	void OnChar(char character);
	void ClearState();

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer); //If the size of the buffer is over it will remove items until it fits

private:
	//KeyBoard State
	static constexpr unsigned int num_keys = 256u;
	std::bitset<num_keys> key_states;

	//Events queues
	static constexpr unsigned int buffer_size = 16u;
	std::queue<Event> key_events_buffer;
	std::queue<char> char_events_buffer;

	bool auto_repeat = false;
};



