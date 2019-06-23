#pragma once
#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum MOUSE_EVENT_TYPE
		{
			LEFT_PRESS,
			LEFT_RELEASED,

			RIGHT_PRESS,
			RIGHT_RELEASED,

			WHEEL_UP,
			WHEEL_DOWN,
			MOVE,

			ENTER,
			LEAVE,

			INVALID
		};

	private:
		MOUSE_EVENT_TYPE type;

		bool left_is_pressed;
		bool right_is_pressed;

		int x, y;

	public:
		Event() : type(MOUSE_EVENT_TYPE::INVALID), left_is_pressed(false), right_is_pressed(false), x(0), y(0) {}
		Event(MOUSE_EVENT_TYPE _type, const Mouse& parent) : type(_type), left_is_pressed(parent.left_is_pressed), right_is_pressed(parent.right_is_pressed), x(parent.x), y(parent.y) {}

		MOUSE_EVENT_TYPE GetType() const { return type; }
		bool IsValid() const { return type != MOUSE_EVENT_TYPE::INVALID; }
		std::pair<int, int> GetPos() { return{ x, y }; }
		int GetPosX() const { return x; }
		int GetPosY() const { return y; }

		bool LeftIsPressed() const { return left_is_pressed; }
		bool RightIsPressed() const { return right_is_pressed; }

	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetMousePos() const;
	int GetPosX() const { return x; }
	int GetPosY() const { return y; }

	bool LeftIsPressed() const { return left_is_pressed; }
	bool RightIsPressed() const { return right_is_pressed; }
	bool IsInWindow() const { return is_in_window; }

	Mouse::Event ReadEvent();
	bool IsEmpty() const;

	void ClearEvents();

private:
	void OnMouseMove(int x, int y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);

	void TrimBuffer();


private:
	static constexpr unsigned int buffer_sizze = 16u;
	int x, y;
	bool left_is_pressed = false;
	bool right_is_pressed = false;
	bool is_in_window = false;

	std::queue<Event> mouse_event_buffer;
};