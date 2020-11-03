class complex {
    double re, im;
public:
    double real() const { return re; }
    void real(double r) { re = r; }
    complex& operator+=(const complex z) {
        re+=z.re;
        im+=z.im;
        return *this;
    }
};

complex operator+(complex a, complex b) { return a+=b; }
