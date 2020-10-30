namespace Graph_lib {

    class Shape {};
    class Line : public Shape {};
    class Poly_line : public Shape {};
    class Text : public Shape {};

    Shape operator+(const Shape&, const Shape&);
    Graph_reader open(const char*);

} // namespace Graph_lib

namespace Text_lib {

    class Glyph {};
    class Word {};
    class Line {};
    class Text {}

    File* open(const char*);
    Word operator+(const Line&, const Line&);

} // Text_lib