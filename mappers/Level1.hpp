#ifndef LEVEL1_HPP
#define LEVEL1_HPP

namespace yajp {
namespace mappers {

/// Turns the parser mapping into something easier
class Level1 {
private:
    std::string lastPropertyName = "";
    enum Depth {Array, Object};
    std::vector<Depth> stack;
public:
    template <typename T>
    void foundSimpleValue(T);
    void foundNull();
    void startArray() { stack.push_back(Array); }
    void endArray() {
        auto last = stack::end();
        if (last == stack.end()) {
            throw logic_error();
        }
    }
    void startObj();
    void endObj();
    void propertyName(string&& name);
};

}
}

#endif // LEVEL1_HPP
