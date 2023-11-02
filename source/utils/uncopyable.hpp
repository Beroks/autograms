#pragma once

class uncopyable
{
    public:

        uncopyable()
        {

        }

    private:

        uncopyable( const uncopyable & ) = delete;

    private:

        uncopyable &operator =( const uncopyable & ) = delete;
};
