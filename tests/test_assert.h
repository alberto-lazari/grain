#pragma once

/**
 * Assertion that is run even in Release builds.
 *
 * Do-while is needed to scope the assert and safely put a ; after the macro.
 */
#define TEST_ASSERT(expr) \
    do \
    { \
        if (!(expr)) \
        { \
            std::cerr << "Assertion failed: " #expr ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while(false)
