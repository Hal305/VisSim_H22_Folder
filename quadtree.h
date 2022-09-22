#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <list>
#include <vector>

namespace gsml
{
typedef std::pair<double, double> Point2D;

template <class ID, class T>
class QuadTree
{
private:
    /*Points that define object corners
     * In the same order as subtree pointers*/
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;
    /*Pointers to subtrees are needed for a
     * dynamic quadtree implementation.
     * They can also be stored in arrays.*/
    QuadTree* m_sw {nullptr};
    QuadTree* m_se {nullptr};
    QuadTree* m_ne {nullptr};
    QuadTree* m_nw {nullptr};

    std::list<std::pair<ID,T>> mObjects;

    bool isLeaf() const;

    //Help variable for storing Ts
    std::vector<T> mAllObjects;

    //Preorder traversal
    void traverseAll(std::vector<T>& allObjects);

public:
    QuadTree();
    QuadTree(const Point2D &v1, const Point2D &v2,
             const Point2D &v3, const Point2D & v4);
    void init(const Point2D &v1, const Point2D &v2,
              const Point2D &v3, const Point2D & v4);
    void subDivide(int n);
    void print() const;
    void printAll() const;
    QuadTree* insert(const Point2D& point, const ID& id, const T& object);
    QuadTree* find(const Point2D& p);
    const QuadTree* find (const Point2D& p) const;
    T& find(const ID& id);
    bool playerCollision(std::pair<float, float> playerPosition, std::pair<float, float> TrophyPosition);
    T& operator[](ID id);

    //Not real iterators, but a container for Ts
    typename std::vector<T>::iterator begin()
    {
        traverseAll(mAllObjects);
        return mAllObjects.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return mAllObjects.end();
    }
};

#endif // QUADTREE_H

template<class ID, class T>
QuadTree<ID, T>::QuadTree()
    : m_sw{nullptr}, m_se{nullptr}, m_ne{nullptr}, m_nw{nullptr}
{

}

template<class ID, class T>
QuadTree<ID, T>::QuadTree(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
    : m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4}, m_sw{nullptr}, m_se{nullptr}, m_ne{nullptr}, m_nw{nullptr}
{

}

template<class ID, class T>
void QuadTree<ID, T>::init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D &v4)
{
    m_a = v1; m_b = v2; m_c = v3; m_d = v4;
}

template<class ID, class T>
bool QuadTree<ID, T>::isLeaf() const
{
    return m_sw == nullptr && m_se == nullptr && m_ne == nullptr && m_nw == nullptr;
}


template<class ID, class T>
void QuadTree<ID, T>::traverseAll(std::vector<T> &allObjects)
{
    for(auto it : mObjects)
            allObjects.push_back(it.second);
    if(m_ne)
        m_ne->traverseAll(allObjects);
    if(m_nw)
        m_nw->traverseAll(allObjects);
    if(m_sw)
        m_sw->traverseAll(allObjects);
    if(m_se)
        m_se->traverseAll(allObjects);
}

template<class ID, class T>
void QuadTree<ID, T>::subDivide(int n)
{
    if (n>0){
        Point2D v1 = Point2D{(m_a.first+m_b.first)/2, (m_a.second+m_b.second)/2};
        Point2D v2 = Point2D{(m_b.first+m_c.first)/2, (m_b.second+m_c.second)/2};
        Point2D v3 = Point2D{(m_c.first+m_d.first)/2, (m_c.second+m_d.second)/2};
        Point2D v4 = Point2D{(m_d.first+m_a.first)/2, (m_d.second+m_a.second)/2};
        Point2D m = Point2D{(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};

        m_sw = new QuadTree(m_a, v1, m, v4);
        m_sw->subDivide(n-1);
        m_se = new QuadTree(v1, m_b, v2, m);
        m_se->subDivide(n-1);
        m_ne = new QuadTree(m, v2, m_c, v3);
        m_ne->subDivide(n-1);
        m_nw = new QuadTree(v4, m, v3, m_d);
        m_nw->subDivide(n-1);
    }
}

template<class ID, class T>
void QuadTree<ID, T>::print() const
{
    Point2D c = Point2D{(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};

    std::cout << "Center = (" << c.first << ", " << c.second << ")" << std::endl;
    for(auto it : mObjects)
        std::cout << it.first << ", " << it.second->getName() << std::endl;
}

template<class ID, class T>
void QuadTree<ID, T>::printAll() const
{
    print();
    if(m_ne) m_ne->printAll();
    if(m_nw) m_nw->printAll();
    if(m_sw) m_sw->printAll();
    if(m_se) m_se->printAll();
}

template<class ID, class T>
QuadTree<ID, T> *QuadTree<ID, T>::insert(const Point2D &point, const ID &id, const T &object)
{
    if(isLeaf())
    {
        mObjects.push_back(std::pair<ID,T>{id, object});
        return this;
    }
    else
    {
        Point2D m = {(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};
        if(point.second < m.second)
        {
            if(point.first < m.first)
                m_sw->insert(point, id, object);
            else
                m_se->insert(point, id, object);
        }
        else
        {
            if(point.first < m.first)
                m_nw->insert(point, id, object);
            else
                m_ne->insert(point, id, object);
        }
    }
}

template<class ID, class T>
QuadTree<ID, T> *QuadTree<ID, T>::find(const Point2D &p)
{
    if(isLeaf())
    {
        return this;
    }
    else
    {
        Point2D m = {(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};
        if(p.second < m.second)
        {
            if(p.first < m.first)
                m_sw->find(p);
            else
                m_se->find(p);
        }
        else
        {
            if(p.first < m.first)
                m_nw->find(p);
            else
                m_ne->find(p);
        }
    }
}

template<class ID, class T>
const QuadTree<ID, T> *QuadTree<ID, T>::find(const Point2D &p) const
{
    if(isLeaf())
    {
        return this;
    }
    else
    {
        Point2D m = {(m_a.first+m_c.first)/2, (m_a.second+m_c.second)/2};
        if(p.second < m.second)
        {
            if(p.first < m.first)
                m_sw->find(p);
            else
                m_se->find(p);
        }
        else
        {
            if(p.first < m.first)
                m_nw->find(p);
            else
                m_ne->find(p);
        }
    }
}

template<class ID, class T>
T &QuadTree<ID, T>::find(const ID &id)
{
    if(isLeaf() && mObjects.size()>0)
    {
        for(auto it:mObjects)
        {
            if(it->first == id)
                return it->second;
        }
    }
    else if (m_ne)
        m_ne->find(id);
    else if (m_nw)
        m_nw->find(id);
    else if (m_sw)
        m_sw->find(id);
    else if (m_se)
        m_se->find(id);
}

template<class ID, class T>
        bool QuadTree<ID, T>::playerCollision(std::pair<float, float> playerPosition,
                                              std::pair<float, float> objectPosition)
{
            float range = 0.5f;
            if((playerPosition.first <objectPosition.first + range
                && playerPosition.first>objectPosition.first - range)
                    && (playerPosition.second <objectPosition.second + range
                        && playerPosition.second>objectPosition.second - range))
            {
                return true;
            }
            else
            {
                return false;
            }

}

template<class ID, class T>
T &QuadTree<ID, T>::operator[](ID id)
{
    if(isLeaf())
    {
        for(auto it:mObjects)
            if(it->first == id)
                return it->second;
    }
}

}

