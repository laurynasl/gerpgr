#ifndef TESTABLE_HPP_
#define TESTABLE_HPP_

class Testable {
    public:
        // returns max of its test result and of its children
        // optionally prints warnings
        virtual int test(bool verbose) const = 0;
        // 0 - all good
        // 1 - warning
        // 2 - error
        // 3 - critical error
};

#endif
