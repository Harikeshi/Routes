#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(size_t threads)
        : stop(false)
    {
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this] {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                                             [this] {
                                                 return this->stop || !this->tasks.empty();
                                             });

                        if (this->stop && this->tasks.empty())
                            return;

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() {
                (*task)();
            });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#include <iostream>
#include <list>

bool incorrectPath(const std::vector<Point2D>& path)
{
    if (path.size() < 2)
    {
        return true;
    }

    for (size_t i = 0; i != path.size() - 1; ++i)
    {
        if (path[i] == path[i + 1])
        {
            return true;
        }
    }

    return false;
}

static int count = 0;
static size_t all_points = 0;

#include <chrono>

inline std::pair<size_t, long> test(const std::string& path, const Schemes::Search::InRegion::Input& input, size_t number)
{
    auto start = std::chrono::high_resolution_clock::now();
    ThreadPool pool(number);

    Schemes::Search::InRegion::_Zigzag algorithm(input);

    std::list<std::future<void /*std::string*/>> result;
    result.resize(number);
    // std::vector<std::future<void>> result;
    // result.reserve(number);

    //    std::thread progress([number](int i){std::cout << "\rПрогресс: " << i / (double) number * 100 << std::flush;}, );

    for (size_t i = 0; i != number; ++i)
    {
        std::cout << "Прогресс: " << i / (double)number * 100 << "% \r";
        std::cout.flush();
        result.emplace_back(pool.enqueue([i, number](Schemes::Search::InRegion::_Zigzag algorithm) {
            auto points = algorithm.calculate().points;
            //            std::cout << points.size() << std::endl;

            all_points += points.size();
            for (size_t j = 0; j != points.size() - 1; ++j)
            {
                if (points[j] == points[j + 1])
                {
                    ++count;
                }
            }
            //            std::cout << all_points << std::endl;
            //            return std::to_string(i);
            //            return points;
        },
                                         algorithm));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    //    std::cout << "Тестовый пример выполнен. Общее число точек: " << all_points << std::endl;
    //    //    for (auto& r : result)
    //    //        r.get();
    //    std::cout << "Повторение точек было в " << count << " тестах из " << number << std::endl;
    //    std::cout << "Время: " << duration.count() / 1000. << " сек." << std::endl;

    return std::make_pair(count, duration.count());
}
