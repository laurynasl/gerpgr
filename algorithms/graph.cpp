#include "graph.hpp"
#include <QtAlgorithms>
#include <iostream>

using namespace std;

Graph:: Graph() {
    _count = 0;
}

Graph:: Graph(const Graph& other) {
    _count = other._count;
    _weights = other._weights;
}

/** \brief Weight of an edge between vertices i and j
 * 
 *  \returns weight. if return value < 0, vertices are disconnected
 */
int Graph:: edgeWeight(int i, int j) const {
    if (i >= _count) {
        return -2;
    }
    else if (i < j) {
        return edgeWeight(j, i);
    }
    else if (i == j) {
        return -3;
    }
    else {
        return _weights[i][j];
    }
}

void Graph:: connect(int i, int j, int aweight) {
    if (i >= _count) {
    }
    else if (i < j) {
        connect(j, i, aweight);
    }
    else if (i == j) {
    }
    else {
        _weights[i][j] = aweight;
    }
}

void Graph:: createDisconnectedGraph(int count) {
    _count = count;
    _weights.clear();
    _weights.resize(_count);
    for (int i=1; i<_count; i++) {
        _weights[i].resize(i);
        for (int j=0; j<i; j++) {
            _weights[i][j] = -1; //disconnected state
        }
    }
}

/** \brief Creates Minimum Spanning Tree using Kruskal's algorithm
 */
Graph Graph:: getMST() const {
    //vertices are connected if theirs colors are equal
    //new graph
    Graph graph;
    graph.createDisconnectedGraph(_count);
    //colors of vertices
    QVector<int> colors(_count, 0);
    
    //useful for new color assignment for a new subgraph
    int colorsCount = 0;
    //first we need to sort all edges by weight (ascending)
    QList<Edge> sortedEdges;
    for (int i=1; i<_count; i++) {
        for (int j=0; j<i; j++) {
            int weight = edgeWeight(i, j);
            if (weight >= 0) {
                Edge edge(i, j, weight);
                sortedEdges.append(edge);
            }
        }
    }
    qSort(sortedEdges);
    /**then I simply pick a cheapest edge and try to connect vertices of that
     * edge. If vertices both had no color, they are assigned new color.
     * If only one vertex has no color, then it is assigned other vertex's color.
     * If their colors were different, one of the colors is changed into other
     * for whole graph. Otherwise, if their colors were same, connection is
     * aborted. Repeat this until we have no edges left or all vertices are
     * connected.
    */
    int unconnected = _count - 1;
    while ((unconnected > 0) && (sortedEdges.size() > 0)) {
        Edge edge = sortedEdges.takeFirst();
        
        int color1 = colors[edge.i];
        int color2 = colors[edge.j];
        if (color1 == 0) {
            if (color2 == 0) {
                colorsCount++;
                colors[edge.i] = colorsCount;
                colors[edge.j] = colorsCount;
                unconnected--;
                graph.connect(edge.i, edge.j, edgeWeight(edge.i, edge.j));
            }
            else {
                colors[edge.i] = colors[edge.j];
                unconnected--;
                graph.connect(edge.i, edge.j, edgeWeight(edge.i, edge.j));
            }
        }
        else {
            if (color2 == 0) {
                colors[edge.j] = colors[edge.i];
                unconnected--;
                graph.connect(edge.i, edge.j, edgeWeight(edge.i, edge.j));
            }
            else {
                if (color1 == color2) {
                    //do nothing - these colors are equal
                }
                else {
                    for (int i=0; i<=_count; i++) {
                        if (colors[i] == color2) {
                            colors[i] = color1;
                        }
                    }
                    unconnected--;
                    graph.connect(edge.i, edge.j, edgeWeight(edge.i, edge.j));
                }
            }
        }
    }
    return graph;
}
