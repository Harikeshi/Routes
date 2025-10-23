#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

// std::packaged_task — это обёртка над вызываемым объектом (функцией, лямбдой, функтором),
// которая позволяет асинхронно выполнить задачу и получить её результат через std::future.
// Это один из ключевых компонентов в построении собственного пула потоков или графа задач.
// std::future — это механизм в C++ для получения результата асинхронной операции,
// которая может завершиться позже.
// std::condition_variable — это примитив синхронизации в C++,
// который позволяет одному потоку заблокироваться и ждать,
// пока другой поток не уведомит его о каком-то событии.
// Он появился в C++11 и используется совместно с std::mutex или std::unique_lock.
namespace Operations {
class ThreadPool
{
public:
    ThreadPool(size_t n)
        : stop(false)
    {
        for (size_t i = 0; i < n; ++i)
        {
            // Добавляем новый поток в workers
            workers.emplace_back([this] {
                // Бесконечный цикл: поток ждёт задачи.
                while (true)
                {
                    std::function<void()> task; // task — контейнер для задачи.
                    {
                        // Захватываем мьютекс.
                        std::unique_lock<std::mutex> lock(this->mtx);
                        // Ждём, пока появится задача или сигнал остановки.
                        cv.wait(lock,
                                [this] {
                                    return stop || !tasks.empty();
                                });
                        // Если пул остановлен и задач нет — выходим из потока.
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        // Извлекаем задачу из очереди и выполняем.
                        tasks.pop();
                    }
                    task(); // Выполняем задачу.
                }
            });
        }
    }

    /*!
     * Обобщённый метод: принимает любую функцию и аргументы.
     * @param f функция.
     * @param args - список аргументов функции.
     * @return Возвращает std::future с результатом.
     */
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        using Ret = decltype(f(args...));
        // Оборачиваем функцию в packaged_task, чтобы получить future.
        auto task = std::make_shared<std::packaged_task<Ret()>>(
            // bind фиксирует аргументы.
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        // Получаем future из задачи.
        std::future<Ret> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace([task] {
                (*task)();
            });
        }
        cv.notify_one();
        return res;
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto& w : workers)
            w.join();
    }

private:
    std::vector<std::thread> workers;        // вектор потоков
    std::queue<std::function<void()>> tasks; // очередь задач
    std::mutex mtx;                          // защита очереди
    std::condition_variable cv;              // синхронизация
    bool stop;                               //! флаг завершения
};
} // namespace Operations

/*
// Общий пример с 100000 задач.
ThreadPool pool(std::thread::hardware_concurrency() * 2); // x2

    std::vector<std::future<int>> results;
    results.reserve(100000);

    // Запускаем 100000 задач
    for (int i = 0; i < 100000; ++i)
    {
        results.push_back(pool.submit(myFunction_, i, i % 10));
    }

    // Собираем результаты
    long long sum = 0;
    for (auto& f : results)
    {
        sum += f.get();
    }

    std::cout << "Сумма результатов: " << sum << "\n";

    const int N = 100000;
    std::vector<int> results_(N); // заранее выделяем память

    // Отправляем задачи батчами
    for (int i = 0; i < N; ++i)
    {
        pool.submit([i, &results_] {
            results_[i] = myFunction_(i, i % 10);
        });
    }

    // Деструктор пула дождётся завершения всех задач
    // Теперь можно агрегировать
    sum = 0;
    for (int v : results_)
        sum += v;

    std::cout << "Сумма: " << sum << "\n";

// примеры использования
std::vector<std::vector<int>> A(100, std::vector<int>(100));
int rows = A.size();
int cols = A[0].size();

std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

// 1. ThreadPool + лямбды
for (int i = 0; i < rows; ++i)
{
for (int j = 0; j < cols; ++j)
{
pool.submit([&, i, j] {
result[i][j] = myFunction(A[i][j]); //! функция
});
}
}

// 2. Batch-подход (разбиение по строкам/блокам)
for (int i = 0; i < rows; ++i)
{
pool.submit([&, i] {
for (int j = 0; j < cols; ++j)
{
result[i][j] = myFunction(A[i][j]);
}
});
}

// 3. for_each
for (int i = 0; i < rows; ++i)
{
std::transform(std::execution::par, A[i].begin(), A[i].end(), result[i].begin(), [](int x) { return myFunction(x); });
}

// Общий доступ, если надо иметь доступ к future больше 1 раза

auto task = std::make_shared<std::packaged_task<int()>>(computeGlobalValue);
std::shared_future<int> sharedResult = task->get_future().share();

// Отправляем задачу в пул
pool.submit([task]{ (*task)(); });

// Многократное чтение результата
pool.submit([sharedResult] {
std::cout << "Поток 1: " << sharedResult.get() << "\n";
});

pool.submit([sharedResult] {
std::cout << "Поток 2: " << sharedResult.get() << "\n";
});

pool.submit([sharedResult] {
std::cout << "Поток 3: " << sharedResult.get() << "\n";
});
*/
