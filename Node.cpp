#include "Node.hpp"

Node::Node(const str &id) : id(id) {}
Node::~Node() {}

const str &Node::getId() const { return id; }
const vector<Edge> &Node::getEdges() const { return edges; }

void Node::addEdge(Node *to, ll distance)
{
    edges.eb(to, distance);
}
