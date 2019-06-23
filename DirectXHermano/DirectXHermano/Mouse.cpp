#include "Mouse.h"

std::pair<int, int> Mouse::GetMousePos() const
{
	return std::pair<int, int>(x, y);
}

Mouse::Event Mouse::ReadEvent()
{
	if (!mouse_event_buffer.empty())
	{
		Mouse::Event ret = mouse_event_buffer.front();
		mouse_event_buffer.pop();
		return ret;
	}

	return Mouse::Event();
}

bool Mouse::IsEmpty() const
{
	return mouse_event_buffer.empty();
}

void Mouse::ClearEvents()
{
	while (!IsEmpty())
	{
		mouse_event_buffer.pop();
	}
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::MOVE, (*this)));
	TrimBuffer();

}

void Mouse::OnMouseEnter()
{
	is_in_window = true;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::ENTER, (*this)));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	is_in_window = false;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::LEAVE, (*this)));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
	left_is_pressed = true;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::LEFT_PRESS, (*this)));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
	left_is_pressed = false;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::LEFT_RELEASED, (*this)));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
	right_is_pressed = true;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::RIGHT_PRESS, (*this)));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
	right_is_pressed = false;

	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::RIGHT_RELEASED, (*this)));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::WHEEL_UP, (*this)));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	mouse_event_buffer.push(Mouse::Event(Mouse::Event::MOUSE_EVENT_TYPE::WHEEL_DOWN, (*this)));
	TrimBuffer();
}

void Mouse::OnWeheelDelta(int x, int y, int delta)
{
	mouse_wheel_delta += delta;
	while (mouse_wheel_delta >= WHEEL_DELTA)
	{
		mouse_wheel_delta -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}

	while (mouse_wheel_delta <= -WHEEL_DELTA)
	{
		mouse_wheel_delta += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer()
{
	while (mouse_event_buffer.size() > buffer_sizze)
		mouse_event_buffer.pop();
}
