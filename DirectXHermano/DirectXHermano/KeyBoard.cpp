#include "KeyBoard.h"

bool KeyBoard::KeyIsPressed(unsigned char keycode) const
{
	return key_states[keycode];
}

KeyBoard::Event KeyBoard::ReadKey()
{
	if (key_events_buffer.size() > 0u)
	{
		KeyBoard::Event key_event = key_events_buffer.front();
		key_events_buffer.pop();
		return key_event;
	}

	return KeyBoard::Event();
}

bool KeyBoard::KeyIsEmpty() const
{
	return key_events_buffer.empty();
}

void KeyBoard::ClearKeyEvents()
{
	while (!KeyIsEmpty())
		key_events_buffer.pop();
}

char KeyBoard::ReadChar()
{
	if (char_events_buffer.size() > 0)
	{
		unsigned char key = char_events_buffer.front();
		char_events_buffer.pop();
		return key;
	}

	return 0;
}

bool KeyBoard::CharIsEmpty() const
{
	return char_events_buffer.empty();
}

void KeyBoard::ClearChar()
{
	while (!char_events_buffer.empty())
		char_events_buffer.pop();
}

void KeyBoard::ClearAll()
{
	ClearKeyEvents();
	ClearChar();
}

void KeyBoard::OnKeyPressed(unsigned char key_code)
{
	key_states[key_code] = true;
	Event new_event(KeyBoard::Event::EVENT_TYPE::PRESS, key_code);
	key_events_buffer.push(new_event);

	TrimBuffer(key_events_buffer);
}

void KeyBoard::OnKeyReleased(unsigned char key_code)
{
	key_states[key_code] = false;
	Event new_event(KeyBoard::Event::EVENT_TYPE::RELEASE, key_code);
	key_events_buffer.push(new_event);

	TrimBuffer(key_events_buffer);
}

void KeyBoard::OnChar(char character)
{
	char_events_buffer.push(character);
	TrimBuffer(char_events_buffer);
}

void KeyBoard::ClearState()
{
	key_states.reset();
}

template<typename T>
void KeyBoard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > buffer_size)
		buffer.pop();
}
