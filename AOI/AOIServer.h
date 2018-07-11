//
// Created by pc4 on 2018/7/10.
//

#ifndef SOLO_AOISERVER_H
#define SOLO_AOISERVER_H

class AOINode
{
public:
    AOINode(float x, float y)
        : x_(x), y_(y)
    {

    }

    float x() { return x_; }
    float y() { return y_; }

    void x(float x) { x_ = x; }
    void y(float y) { y_ = y; }
private:
    float x_ = 0.0;
    float y_ = 0.0;
};


class AOIServer
{
public:
    AOIServer() = default;
    virtual ~AOIServer() {}

    virtual void insert(AOINode*) = 0;

    virtual void update(AOINode*) = 0;

    virtual void remove(AOINode*) = 0;
};

#endif //SOLO_AOISERVER_H
