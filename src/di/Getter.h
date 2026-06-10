#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace di {
  template <typename T,                                //
            typename Deleter = std::default_delete<T>> //
  class Getter
  {
    std::shared_mutex   *mutex_;
    std::function<T *()> getter_;
    std::atomic<T *>     cache_{nullptr};

    [[no_unique_address]] Deleter deleter_{};

  public:
    Getter(std::shared_mutex *mutex, std::function<T *()> getter)
        : mutex_(mutex)
        , getter_(std::move(getter))
        , deleter_(Deleter{})
    {}

    Getter(std::shared_mutex *mutex, std::function<T *()> getter, Deleter deleter)
        : mutex_(mutex)
        , getter_(std::move(getter))
        , deleter_(std::move(deleter))
    {}

    ~Getter()
    {
      T *pointer = cache_.load(std::memory_order_seq_cst);
      deleter_(pointer);
    }

    T *get()
    {
      T *ref = cache_.load(std::memory_order_seq_cst);

      if (ref == nullptr) {
        std::unique_lock lock(*mutex_);

        ref = cache_.load(std::memory_order_seq_cst);

        if (ref == nullptr) {
          ref = getter_();
          cache_.store(ref);
        }
      }

      return ref;
    }

    inline T *operator->()
    {
      return get();
    }

    inline T &operator*()
    {
      return *get();
    }
  };
} // namespace getter
