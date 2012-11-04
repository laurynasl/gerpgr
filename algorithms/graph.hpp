#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <QVector>

class Edge {
    public:
        int i; /// first vertex
        int j; /// second vertex
        int weight;
        
        Edge(int _i, int _j, int _weight) : i(_i), j(_j), weight(_weight) {};
        Edge(const Edge& other) : i(other.i), j(other.j), weight(other.weight) {};
        bool operator < (const Edge& other) const {
            return weight < other.weight;
        }
};

/** \brief Graph. for getting minimum spanning tree
 * 
 */
/*
   1 <= i < _count
   0 <= j < i
*/
class Graph {
    protected:
        int _count;
        QVector<QVector<int> > _weights;
    public:
        Graph();
        Graph(const Graph& other);
        int edgeWeight(int i, int j) const; /// \returns -1, if disconnected
        void connect(int i, int j, int aweight);
        void createDisconnectedGraph(int count);
        Graph getMST() const;
};

#endif
