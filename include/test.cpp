/*

gpp $OPTIMIZE_OPTIONS test.cpp && d ./a.out

clear ; ./a.out

*/

#include "prng.hpp"
#include <fmt/format.h>

int main()
{
    constexpr int max_iterations = 10;

    /*
    {
        fmt::println("# blake3");
        blake3::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        blake3 prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }
    */

    /*
    {
        fmt::println("# chacha_32");
        chacha_32::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        chacha_32 prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }
    */

    {
        fmt::println("# xoroshiro64starstar");
        xoroshiro64starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoroshiro64starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }

    {
        fmt::println("# xoroshiro128plusplus");
        xoroshiro128plusplus::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoroshiro128plusplus prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoroshiro128starstar");
        xoroshiro128starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoroshiro128starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoroshiro1024plusplus");
        xoroshiro1024plusplus::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoroshiro1024plusplus prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoroshiro1024starstar");
        xoroshiro1024starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoroshiro1024starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro128plusplus");
        xoshiro128plusplus::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro128plusplus prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro128starstar");
        xoshiro128starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro128starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro256plusplus");
        xoshiro256plusplus::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro256plusplus prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro256starstar");
        xoshiro256starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro256starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro512plusplus");
        xoshiro512plusplus::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro512plusplus prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    {
        fmt::println("# xoshiro512starstar");
        xoshiro512starstar::state_type seed{};

        //seed[0] = 1;
        //seed[1] = 1;

        xoshiro512starstar prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    return 0;

    {
        fmt::println("# klimov_shamir_32");
        klimov_shamir_32::state_type seed{};

        klimov_shamir_32 prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }

    {
        fmt::println("# sfc32");
        sfc32::state_type seed{};

        sfc32 prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:08x} ({1:032b})", i, x);
        }
    }

    {
        fmt::println("# sfc64");
        sfc64::state_type seed{};

        sfc64 prng(seed);

        for (int i = 0; i < max_iterations; ++i)
        {
            const auto x = prng();
            fmt::println("{0:>02} = {1:016x} ({1:064b})", i, x);
        }
    }

    return 0;
}
