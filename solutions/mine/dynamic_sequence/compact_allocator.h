const unsigned POOL_SIZE = 48000000;

// assisted by gpt
template<typename T>
class MemoryPoolManager {
public:
    MemoryPoolManager(): pool(), active() { }

    ~MemoryPoolManager() { if (active) std::free(pool); }

    T* allocate() {
        if (!active) {
            active = true;
            pool = static_cast<T*>(std::malloc(POOL_SIZE * sizeof(T)));
            current = pool;
        }
        return current++;
    }

    T* pool;
    T* current;
    bool active;
};

template<typename T>
struct CompactAllocator
{
    static MemoryPoolManager<T> pool_manager;
    using value_type = T;
    constexpr CompactAllocator() noexcept { }
    template<class U>
    constexpr CompactAllocator(const CompactAllocator<U>&) noexcept { }

    T* allocate(std::size_t) {
        return (pool_manager.allocate());
    }

    void deallocate(T*, std::size_t) noexcept {
        // pool_manager.deallocate(x);
    }
};
template<typename T>
MemoryPoolManager<T> CompactAllocator<T>::pool_manager;