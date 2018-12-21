#ifndef BITS_COMMON_QUEUE
#define BITS_COMMON_QUEUE

// Source: https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads

#include <deque>
#include <mutex>
#include <condition_variable>

#include "Packet.h"

namespace gw {
  class Queue {
  public:
    void push(const Packet &packet) {
      std::unique_lock<std::mutex> lock(m_mutex);

      m_queue.push_back(packet);

      m_condition.notify_one();
    }

    bool poll(Packet &packet) {
      std::unique_lock<std::mutex> lock(m_mutex);

      if (m_queue.empty()) {
        return false;
      }

      packet = m_queue.front();
      m_queue.pop_front();

      return true;
    }

    void waitPoll(Packet &packet) {
      std::unique_lock<std::mutex> lock(m_mutex);

      while(m_queue.size() == 0) { // Handle spurious wake-ups
        m_condition.wait(lock);
      }

      packet = m_queue.front();
      m_queue.pop_front();
    }

  private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::deque<Packet> m_queue;
  };
}

#endif // BITS_COMMON_SEMAPHORE
