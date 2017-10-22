#include <array>
#include <atomic>
#include <chrono>
#include <vector>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <algorithm>

template<class T>
struct simple_size_t
{
	simple_size_t()
	{
		val = 0;
	}

	void add(size_t next)
	{
		val += next;
	}

	T val;
};

template<class T>
struct cache_line_size_t
{
	cache_line_size_t()
	{
		val = 0;
	}

	void add(size_t next)
	{
		val += next;
	}

	T val;
	std::array<size_t, (64 - sizeof(size_t)) / sizeof(size_t)> noop;
};

template<class T>
size_t functor(const std::vector<size_t>& data, std::vector<T>& vector, size_t i)
{
	for (const size_t item : data) {
		vector[i].add(item);
	}
	return vector[i].val;
}

template<class T>
size_t test(const std::vector<size_t>& data)
{
	using clock = std::chrono::high_resolution_clock;
	size_t size = std::thread::hardware_concurrency();
	
	std::vector<T> vector(size);

	std::atomic<size_t> counter{ 0 };

	auto time_start = clock::now();
	std::vector<std::thread> threads;
	for (size_t i = 0; i < size; i++) {
		std::thread thread([&counter, &vector, &data, i]() {
			counter += functor(data, vector, i);
		});
		threads.push_back(std::move(thread));;
	}

	for (auto& thread : threads) {
		thread.join();
	}

	auto delta_time = clock::now() - time_start;
	time_start = clock::now();
	std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(delta_time);

	std::cout << "type: " << typeid(T).name() << " delta: " << delta.count() << std::endl;

	return counter;
}

// https://www.youtube.com/watch?v=WDIkqP4JbkE&t=2582s

int main()
{
	std::vector<size_t> data(100000000);
	std::generate(data.begin(), data.end(), std::rand);

	std::cout << "generated" << std::endl;

	size_t result = 0;

	for (int i = 0; i < 3; i++) {
		result += test<simple_size_t<size_t>>(data);
		result += test<cache_line_size_t<size_t>>(data);
		result += test<simple_size_t<std::atomic<size_t>>>(data);
		result += test<cache_line_size_t<std::atomic<size_t>>>(data);
	}

	return (int)(result);
}
