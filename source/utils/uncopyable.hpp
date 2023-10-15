#pragma once

class uncopyable
{
    public:

        uncopyable()
        {

        }

    private:

        uncopyable( const uncopyable & )
        {

        }

    private:

        uncopyable &operator =( const uncopyable & )
        {
            return *this;
        }
};
