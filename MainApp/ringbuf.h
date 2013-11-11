#ifndef RINGBUF_H
#define RINGBUF_H

template <typename T, int size>
class CRingBuf {
public:
    CRingBuf() {
        ring_top = ring_bot = 0;
    }

    ~CRingBuf() {
    }

    int SpaceLeft() const {
        return ((size-1)-Count());
    }

    int Count() const {
        if (ring_bot > ring_top) {
            return (size - (ring_bot - ring_top));
        } else {
            return (ring_top - ring_bot);
        }
    }

    bool Full() const {
        return (Count() == size - 1);
    }

    bool Has(const T& val) const {
        if (ring_bot == ring_top) {
            return false;
        }
        int pos = ring_bot;
        if (pos < ring_top) {
            while (pos < ring_top) {
                if (ringbuf_data[pos] == val) {
                    return true;
                }
                pos++;
            }
        } else {
            while (pos < size) {
                if (ringbuf_data[pos] == val) {
                    return true;
                }
                pos++;
            }
            pos = 0;
            while (pos < ring_top) {
                if (ringbuf_data[pos] == val) {
                    return true;
                }
                pos++;
            }
        }
        return false;
    }

    T& Get() {
        int outpos = ring_bot;
        if (ring_bot != ring_top) {
            if (++ring_bot == size) {
                ring_bot = 0;
            }
        }
        return ringbuf_data[outpos];
    }

    void Put(const T& in) {
        ringbuf_data[ring_top++] = in;
        if (ring_top == size) {
            ring_top = 0;
        }
    }

private:
    T ringbuf_data[size]; 
    int ring_top;
    int ring_bot;
};



#endif

