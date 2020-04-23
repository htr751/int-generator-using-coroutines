#include"generator.h"

generator_imp std::experimental::coroutine_traits<generator_imp, int>::promise_type::get_return_object() {
	return coro_handle::from_promise(*this);
}

std::experimental::suspend_always std::experimental::coroutine_traits<generator_imp, int>::promise_type::initial_suspend()noexcept {
	return std::experimental::suspend_always{};
}

std::experimental::suspend_always std::experimental::coroutine_traits<generator_imp, int>::promise_type::final_suspend()noexcept {
	return std::experimental::suspend_always{};
}

void std::experimental::coroutine_traits<generator_imp, int>::promise_type::return_void()noexcept {

}

std::experimental::suspend_always std::experimental::coroutine_traits<generator_imp, int>::promise_type::yield_value(int val)noexcept {
	this->value = val;
	return std::experimental::suspend_always{};
}

void std::experimental::coroutine_traits<generator_imp, int>::promise_type::unhandled_exception() {
	std::exception_ptr current_exception = std::current_exception();
	if (current_exception)
		std::rethrow_exception(current_exception);
}

generator_imp::generator_imp(generator_imp&& other) noexcept{
	this->m_handle = other.m_handle;
	other.m_handle = nullptr;
}

generator_imp::generator_imp(coro_handle handle) :m_handle(handle) { assert(m_handle);}
int generator_imp::get_value() const noexcept {
	return this->m_handle.promise().value;
}

void generator_imp::resume() {
	this->m_handle();
}

bool generator_imp::done() noexcept{
	return this->m_handle.done();
}


generator_imp::~generator_imp() { 
	if(this->m_handle)
		this->m_handle.destroy(); 
}

generator_iterator::generator_iterator(generator_imp&& handle, int initial_value)noexcept :m_generator(std::move(handle)),
m_current_value(initial_value) {}

bool generator_iterator::operator==(const generator_iterator& other) const noexcept {
	return this->m_current_value == other.m_current_value;
}

bool generator_iterator::operator!=(const generator_iterator& other)const noexcept{
	return !(*this == other);
}

generator_iterator& generator_iterator::operator++() {
	if (!this->m_generator.done())
		this->m_generator.resume();
	this->m_current_value = this->m_generator.get_value();
	return *this;
}


int generator_iterator::operator*() const noexcept {
	return this->m_current_value;
}

generator_iterator generator::begin() const {
	auto generator = generator::generateNumberFunction(0);
	generator.resume();
	return generator_iterator{ std::move(generator), 0 };
}

generator_iterator generator::end() const {
	return generator_iterator{ generator::generateNumberFunction(std::numeric_limits<int>::max()), std::numeric_limits<int>::max() };
}

generator_imp generator::generateNumberFunction(int starter) noexcept {
	int i = starter;
	while (true) {
		co_yield i;
		i++;
	}
}
