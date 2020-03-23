#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool isPress() const noexcept
		{
			return type == Type::Press;
		}
		bool isRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool isInvalid() const noexcept
		{
			return type != Type::Invalid;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;

	bool KeyPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyEmpty() const noexcept;
	void ClearKey() noexcept;

	char ReadChar() noexcept;
	bool CharEmpty() const noexcept;
	void ClearChar() noexcept;
	void Clear() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutoRepeatEnabled() const noexcept;
private:
	void onKeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void onChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event>  keybuffer;
	std::queue<char>  charbuffer;
};