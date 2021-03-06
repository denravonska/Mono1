
#pragma once

#include <algorithm>
#include <vector>

namespace mono
{
    template <typename T>
    inline bool remove(std::vector<T>& collection, const T& object)
    {
        const auto it = std::find(collection.begin(), collection.end(), object);
        if(it != collection.end())
        {
            collection.erase(it);
            return true;
        }

        return false;
    }

    template <typename T, typename F>
    inline bool remove_if(std::vector<T>& collection, const F& remove_if_func)
    {
        const auto it = std::remove_if(collection.begin(), collection.end(), remove_if_func);
        if(it != collection.end())
        {
            collection.erase(it, collection.end());
            return true;
        }
        
        return false;
    }

    class FPSCounter
    {
    public:

        void operator ++ (int i);
        unsigned int Fps() const;
        unsigned int Frames() const;

    private:

        unsigned int mTime = 0;
        unsigned int mFrames = 0;
        unsigned int mTotalFrames = 0;
        unsigned int mFps = 0;
    };
}
