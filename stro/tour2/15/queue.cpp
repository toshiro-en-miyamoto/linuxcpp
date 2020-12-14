struct message
{
    using value_type = char;
    char value;
};

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const message& m)
{
    return std::cout << m.value;
}

inline std::istream& operator>>(std::istream& is, message& m)
{
    return is >> m.value;
}

#include <queue>
#include <condition_variable>
#include <mutex>

std::queue<message> mq;
std::condition_variable mcond;
std::mutex mmutex;

void consumer(std::stop_token stop_token)
{
    while (true)
    {
        // acquire mmutex
        std::unique_lock lck{mmutex};

        // release lck and wait
        // re-acquire lck upon wakeup
        mcond.wait(lck);

        if (stop_token.stop_requested())
        {
            return;
        }
        else if (!mq.empty())
        {
            auto m = mq.front();
            mq.pop();
            std::cout << '>' << m << '\n' << std::flush;
        }

        // release the lock
        lck.unlock();
    }
}

void producer()
{
    while (true)
    {
        // push the message
        message msg;
        std::cin >> msg;
        if (std::cin.good())
        {
            // protect operations
            std::scoped_lock lck {mmutex};
            mq.push(msg);
            mcond.notify_one();
        }
        else
        {
            return;
        }
        
    }
}

#include <thread>

int main()
{
    std::jthread thread_consumer{ consumer };
    std::jthread thread_producer{ producer };
    thread_producer.join();
    thread_consumer.request_stop();
    mcond.notify_one();
    thread_consumer.join();
}
