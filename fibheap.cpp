#include "fibheap.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

FibHeap::FibHeap(int maxVertices)
    : nodesCount(0), minptr(nullptr), nodesById(maxVertices + 1, nullptr) {}

FibHeap::~FibHeap()
{
    clear();
}

bool FibHeap::empty() const
{
    return minptr == nullptr;
}

ll FibHeap::min_key() const
{
    return minptr ? minptr->key : numeric_limits<ll>::max();
}

void FibHeap::insert(int vertex, ll key)
{
    // if vertex id outside current nodesById, resize
    if (vertex >= (int)nodesById.size())
        nodesById.resize(vertex + 1, nullptr);

    // if an entry for this vertex already exists, treat as decrease_key instead
    if (nodesById[vertex])
    {
        if (key < nodesById[vertex]->key)
            decrease_key(vertex, key);
        return;
    }

    FibNode *x = new FibNode(vertex, key);
    // insert into root list
    if (minptr == nullptr)
    {
        minptr = x;
    }
    else
    {
        // insert to right of min
        x->right = minptr->right;
        x->left = minptr;
        minptr->right->left = x;
        minptr->right = x;
        if (x->key < minptr->key)
            minptr = x;
    }
    nodesById[vertex] = x;
    nodesCount++;
}

bool FibHeap::contains(int vertex) const
{
    return vertex < (int)nodesById.size() && nodesById[vertex] != nullptr;
}

void FibHeap::removeFromList(FibNode *x)
{
    if (!x)
        return;
    if (x->left == x && x->right == x)
    {
        // single node — isolate it
    }
    else
    {
        x->left->right = x->right;
        x->right->left = x->left;
    }
    x->left = x->right = x;
}

void FibHeap::link(FibNode *parentNode, FibNode *childNode)
{
    // remove childNode from its list
    removeFromList(childNode);

    // make childNode a child of parentNode
    childNode->parent = parentNode;
    childNode->mark = false;

    if (parentNode->child == nullptr)
    {
        parentNode->child = childNode;
        childNode->left = childNode->right = childNode;
    }
    else
    {
        // insert into parent's child list
        childNode->right = parentNode->child->right;
        childNode->left = parentNode->child;
        parentNode->child->right->left = childNode;
        parentNode->child->right = childNode;
    }
    parentNode->degree++;
}

void FibHeap::consolidate()
{
    if (!minptr)
        return;
    int maxDeg = (int)(log2(max(1, nodesCount))) + 2;
    vector<FibNode *> A(maxDeg, nullptr);

    // collect root list into vector to iterate safely
    vector<FibNode *> roots;
    FibNode *start = minptr;
    FibNode *cur = start;
    do
    {
        roots.push_back(cur);
        cur = cur->right;
    } while (cur != start);

    for (FibNode *w : roots)
    {
        FibNode *x = w;
        int d = x->degree;
        while (d >= (int)A.size())
            A.resize(A.size() * 2 + 1, nullptr);
        while (A[d] != nullptr)
        {
            FibNode *y = A[d];
            if (y->key < x->key)
                swap(x, y);
            // y becomes child of x
            link(x, y);
            A[d] = nullptr;
            d++;
            while (d >= (int)A.size())
                A.resize(A.size() * 2 + 1, nullptr);
        }
        A[d] = x;
    }

    // rebuild root list and find new min
    minptr = nullptr;
    for (FibNode *node : A)
    {
        if (node)
        {
            // isolate node
            node->left = node->right = node;
            node->parent = nullptr;
            if (minptr == nullptr)
                minptr = node;
            else
            {
                // insert node into root list
                node->right = minptr->right;
                node->left = minptr;
                minptr->right->left = node;
                minptr->right = node;
                if (node->key < minptr->key)
                    minptr = node;
            }
        }
    }
}

void FibHeap::moveChildrenToRootList(FibNode *z)
{
    if (!z || !z->child)
        return;
    // snapshot child list
    vector<FibNode *> children;
    FibNode *cstart = z->child;
    FibNode *ccur = cstart;
    do
    {
        children.push_back(ccur);
        ccur = ccur->right;
    } while (ccur != cstart);

    for (FibNode *ch : children)
    {
        // detach from child list
        removeFromList(ch);
        ch->parent = nullptr;
        ch->mark = false;

        // add to root list
        if (minptr == nullptr)
        {
            ch->left = ch->right = ch;
            minptr = ch;
        }
        else
        {
            ch->right = minptr->right;
            ch->left = minptr;
            minptr->right->left = ch;
            minptr->right = ch;
            if (ch->key < minptr->key)
                minptr = ch;
        }
    }
    z->child = nullptr;
}

int FibHeap::extract_min()
{
    if (!minptr)
        return -1;
    FibNode *z = minptr;

    // move children of z to root list
    moveChildrenToRootList(z);

    // remove z from root list
    if (z->left == z && z->right == z)
    {
        minptr = nullptr;
    }
    else
    {
        z->left->right = z->right;
        z->right->left = z->left;
        minptr = z->right; // temporary
    }

    // remove mapping
    if (z->vertex >= 0 && z->vertex < (int)nodesById.size())
        nodesById[z->vertex] = nullptr;

    int retVertex = z->vertex;
    // free z
    delete z;
    nodesCount--;

    if (minptr)
        consolidate();
    return retVertex;
}

void FibHeap::decrease_key(int vertex, ll newKey)
{
    if (vertex >= (int)nodesById.size() || nodesById[vertex] == nullptr)
        return;
    FibNode *x = nodesById[vertex];
    if (newKey >= x->key)
        return;
    x->key = newKey;
    FibNode *y = x->parent;
    if (y && x->key < y->key)
    {
        // cut x from parent and add to root list
        // remove x from parent's child list
        removeFromList(x);
        if (y->child == x)
        {
            // if x was the only child, y->child becomes null or another child
            if (x->right != x)
                y->child = x->right;
            else
                y->child = nullptr;
        }
        y->degree--;
        x->parent = nullptr;
        x->mark = false;

        // insert x into root list
        x->right = minptr->right;
        x->left = minptr;
        minptr->right->left = x;
        minptr->right = x;
    }
    // cascading cut is omitted here for brevity (but can be implemented)
    // The following implements a simple cascading cut:
    while (y)
    {
        if (!y->mark)
        {
            y->mark = true;
            break;
        }
        else
        {
            FibNode *py = y->parent;
            // cut y
            removeFromList(y);
            if (py && py->child == y)
            {
                if (y->right != y)
                    py->child = y->right;
                else
                    py->child = nullptr;
            }
            if (py)
                py->degree--;
            y->parent = nullptr;
            y->mark = false;
            // insert y to root list
            y->right = minptr->right;
            y->left = minptr;
            minptr->right->left = y;
            minptr->right = y;
            y = py;
        }
    }

    if (minptr == nullptr || x->key < minptr->key)
        minptr = x;
}

void FibHeap::freeAllNodes()
{
    if (!minptr)
        return;
    // collect all nodes with DFS-like traversal from root list
    vector<FibNode *> stack;
    vector<FibNode *> all;
    FibNode *start = minptr;
    FibNode *cur = start;
    do
    {
        stack.push_back(cur);
        cur = cur->right;
    } while (cur != start);

    while (!stack.empty())
    {
        FibNode *node = stack.back();
        stack.pop_back();
        if (!node)
            continue;
        if (node->degree == -1)
            continue; // visited marker
        node->degree = -1;
        all.push_back(node);
        // push children
        if (node->child)
        {
            FibNode *cstart = node->child;
            FibNode *ccur = cstart;
            do
            {
                stack.push_back(ccur);
                ccur = ccur->right;
            } while (ccur != cstart);
        }
        // push right sibling
        if (node->right && node->right != node)
            stack.push_back(node->right);
    }

    for (FibNode *n : all)
        delete n;
    minptr = nullptr;
}

void FibHeap::clear()
{
    // free memory and clear nodesById
    freeAllNodes();
    nodesById.assign(nodesById.size(), nullptr);
    nodesCount = 0;
}
