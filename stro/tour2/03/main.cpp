// c++ -Wall -std=c++2a -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. main.cpp Vector.cpp

import Vector;
import <stdexcept>;
import <iostream>;

int main() {
    try {
        Vector v1{-3};
    } catch (std::length_error& err) {
        std::cerr << err.what() << '\n';
    } catch (std::bad_alloc& err) {
        std::cerr << err.what() << '\n';
        throw;  // rethrow
    }

    try {
        Vector v2{4};
        v2[4]++;
    } catch (std::out_of_range& err) {
        std::cerr << err.what() << '\n';
    }
}