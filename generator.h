#pragma once
#include<experimental/coroutine>
#include<cassert>
#include<limits>
class generator_imp;

namespace std::experimental {
	template<>
	struct coroutine_traits<generator_imp, int> {
		struct promise_type {
			int value = 0;
			using coro_handle = std::experimental::coroutine_handle<promise_type>;
			generator_imp get_return_object();
			std::experimental::suspend_always initial_suspend() noexcept;
			std::experimental::suspend_always final_suspend() noexcept;
			void return_void() noexcept;
			std::experimental::suspend_always yield_value(int value) noexcept;
			void unhandled_exception();
		};
	};
}

class generator_imp {
public:
	using coro_handle = std::experimental::coroutine_handle<
		std::experimental::coroutine_traits<generator_imp, int>::promise_type>;
	generator_imp(coro_handle handle);
	generator_imp(const generator_imp&) = delete;
	generator_imp(generator_imp&& other)noexcept;
	int get_value() const noexcept;
	void resume();
	bool done()noexcept;
	~generator_imp() noexcept;

private:
	coro_handle m_handle;

};

class generator_iterator {
public:
	using iterator_category = std::input_iterator_tag;
	using value_type = int;
	using difference_type = std::ptrdiff_t;
	using pointer = int*;
	using reference = int&;

	generator_iterator(generator_imp&& handle, int initial_value)noexcept;
	generator_iterator(const generator_iterator&) = delete;
	generator_iterator(generator_iterator&& other) = default;

	generator_iterator& operator=(const generator_iterator&) = delete;
	generator_iterator& operator=(generator_iterator&&) = delete;

	bool operator==(const generator_iterator& other) const noexcept;

	bool operator!=(const generator_iterator& other)const noexcept;

	generator_iterator& operator++();

	int operator*() const noexcept;

private:
	generator_imp m_generator;
	int m_current_value;
};

class generator {
public:
	generator() = default;
	generator_iterator begin() const;
	generator_iterator end() const;
private:
	static generator_imp generateNumberFunction(int starter) noexcept;
};
