#include <vector>

template <typename T>
class CircularBuffer {
public:
    CircularBuffer(const int size) 
                            : m_capacity{size}
                            , m_buffer(size) {}
    
    bool write(const std::vector<T> &chunk) {
        if (chunk.size() > m_capacity - m_size) return false;

        for (size_t i = 0;i < chunk.size();i++) {
            m_buffer[(m_writeIndex + i) % m_capacity] = chunk[i];
        }

        m_writeIndex += chunk.size();
        m_writeIndex %= m_capacity;
        m_size += chunk.size();

        return true;
    }
    
    bool read(std::vector<T> &out, const int count) {
        if (m_size < count) return false;

        out.resize(count);

        for (size_t i = 0;i < count;i++) {
            out[i] = m_buffer[(m_readIndex + i) % m_capacity];
        }

        m_readIndex += count;
        m_readIndex %= m_capacity;
        m_size -= count;

        return true;
    }
private:
    std::vector<T> m_buffer{};
    int m_writeIndex{};
    int m_readIndex{};
    int m_capacity{};
    int m_size{};
};