#ifndef ENGINE_H
#define ENGINE_H

class Engine {
public:
    inline void setEngines(Engine** engine_arr) {
        this->engine_arr = engine_arr;
    }
private:
protected:
    Engine() {}
    ~Engine() {}

    Engine** engine_arr;
};

#endif
