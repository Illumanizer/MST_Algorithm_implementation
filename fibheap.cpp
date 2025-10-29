// ===== File: fibheap.cpp =====
#include "fibheap.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <climits>
using namespace std;

// allocate a new node
static fh_node *create_node(int vertex, int key)
{
    fh_node *x = new fh_node(vertex, key);
    return x;
}

// remove x from its circular list and isolate it
static void remove_from_list(fh_node *x)
{
    if (!x)
        return;
    if (x->left == x && x->right == x)
    {
        // single node, leave isolated
    }
    else
    {
        x->left->right = x->right;
        x->right->left = x->left;
    }
    x->left = x->right = x;
}

// link child under parent
static void link_nodes(fibheap_c *H, fh_node *parent, fh_node *child)
{
    // detach child from root list
    remove_from_list(child);
    child->parent = parent;
    child->mark = false;
    if (!parent->child)
    {
        parent->child = child;
        child->left = child->right = child;
    }
    else
    {
        child->right = parent->child->right;
        child->left = parent->child;
        parent->child->right->left = child;
        parent->child->right = child;
    }
    parent->degree++;
}

// consolidate root list after extract
static void consolidate(fibheap_c *H)
{
    if (!H->minptr)
        return;
    int maxDeg = (int)(log2(max(1, H->nodes))) + 2;
    vector<fh_node *> A(maxDeg, nullptr);

    // take snapshot of root list
    vector<fh_node *> roots;
    fh_node *start = H->minptr;
    fh_node *cur = start;
    do
    {
        roots.push_back(cur);
        cur = cur->right;
    } while (cur != start);

    for (fh_node *w : roots)
    {
        fh_node *x = w;
        int d = x->degree;
        while (d >= (int)A.size())
            A.resize(A.size() * 2 + 1, nullptr);
        while (A[d] != nullptr)
        {
            fh_node *y = A[d];
            if (y->key < x->key)
            {
                fh_node *tmp = x;
                x = y;
                y = tmp;
            }
            link_nodes(H, x, y);
            A[d] = nullptr;
            d++;
            while (d >= (int)A.size())
                A.resize(A.size() * 2 + 1, nullptr);
        }
        A[d] = x;
    }

    // rebuild root list and find new min
    H->minptr = nullptr;
    for (fh_node *n : A)
    {
        if (!n)
            continue;
        n->left = n->right = n;
        n->parent = nullptr;
        if (!H->minptr)
            H->minptr = n;
        else
        {
            n->right = H->minptr->right;
            n->left = H->minptr;
            H->minptr->right->left = n;
            H->minptr->right = n;
            if (n->key < H->minptr->key)
                H->minptr = n;
        }
    }
}

// move children of z to root list
static void move_children_to_root(fibheap_c *H, fh_node *z)
{
    if (!z || !z->child)
        return;
    vector<fh_node *> children;
    fh_node *cstart = z->child;
    fh_node *cc = cstart;
    do
    {
        children.push_back(cc);
        cc = cc->right;
    } while (cc != cstart);

    for (fh_node *ch : children)
    {
        remove_from_list(ch);
        ch->parent = nullptr;
        ch->mark = false;
        if (!H->minptr)
        {
            ch->left = ch->right = ch;
            H->minptr = ch;
        }
        else
        {
            ch->right = H->minptr->right;
            ch->left = H->minptr;
            H->minptr->right->left = ch;
            H->minptr->right = ch;
            if (ch->key < H->minptr->key)
                H->minptr = ch;
        }
    }
    z->child = nullptr;
}

// create heap
fibheap_c *fh_create()
{
    fibheap_c *H = new fibheap_c();
    H->nodes = 0;
    H->minptr = nullptr;
    return H;
}

// destroy heap and free nodes
void fh_destroy(fibheap_c *H)
{
    if (!H)
        return;
    if (H->minptr)
    {
        vector<fh_node *> stack;
        vector<fh_node *> all;
        fh_node *start = H->minptr;
        fh_node *cur = start;
        do
        {
            stack.push_back(cur);
            cur = cur->right;
        } while (cur != start);

        while (!stack.empty())
        {
            fh_node *n = stack.back();
            stack.pop_back();
            if (!n)
                continue;
            if (n->degree == -1)
                continue; // visited marker
            n->degree = -1;
            all.push_back(n);
            if (n->child)
            {
                fh_node *cstart = n->child;
                fh_node *cc = cstart;
                do
                {
                    stack.push_back(cc);
                    cc = cc->right;
                } while (cc != cstart);
            }
            if (n->right && n->right != n)
                stack.push_back(n->right);
        }
        for (fh_node *p : all)
            delete p;
    }
    delete H;
}

// insert and return node pointer
fh_node *fh_insert(fibheap_c *H, int vertex, int key)
{
    if (!H)
        return nullptr;
    fh_node *x = create_node(vertex, key);
    if (!H->minptr)
    {
        H->minptr = x;
    }
    else
    {
        x->right = H->minptr->right;
        x->left = H->minptr;
        H->minptr->right->left = x;
        H->minptr->right = x;
        if (x->key < H->minptr->key)
            H->minptr = x;
    }
    H->nodes++;
    return x;
}

// extract min node and return it (caller should delete)
fh_node *fh_extract_min_node(fibheap_c *H)
{
    if (!H || !H->minptr)
        return nullptr;
    fh_node *z = H->minptr;
    move_children_to_root(H, z);

    // remove z from root list
    if (z->left == z && z->right == z)
    {
        H->minptr = nullptr;
    }
    else
    {
        z->left->right = z->right;
        z->right->left = z->left;
        H->minptr = z->right; // temporary
    }

    // isolate z
    z->left = z->right = z;
    H->nodes--;
    if (H->minptr)
        consolidate(H);
    return z;
}

// cut x from parent and move to root list
static void cut_node_to_root(fibheap_c *H, fh_node *x, fh_node *par)
{
    if (!x || !par)
        return;
    if (x->right == x)
    {
        par->child = nullptr;
    }
    else
    {
        if (par->child == x)
            par->child = x->right;
        x->right->left = x->left;
        x->left->right = x->right;
    }
    par->degree--;
    x->parent = nullptr;
    x->mark = false;

    // add x to root list
    if (!H->minptr)
    {
        x->left = x->right = x;
        H->minptr = x;
    }
    else
    {
        x->right = H->minptr->right;
        x->left = H->minptr;
        H->minptr->right->left = x;
        H->minptr->right = x;
        if (x->key < H->minptr->key)
            H->minptr = x;
    }
}

// cascading cut
static void cascading_cut(fibheap_c *H, fh_node *y)
{
    fh_node *z = y->parent;
    if (!z)
        return;
    if (!y->mark)
    {
        y->mark = true;
    }
    else
    {
        cut_node_to_root(H, y, z);
        cascading_cut(H, z);
    }
}

// decrease key of node x
void fh_decrease_key(fibheap_c *H, fh_node *x, int newKey)
{
    if (!H || !x)
        return;
    if (newKey >= x->key)
        return;
    x->key = newKey;
    fh_node *y = x->parent;
    if (y && x->key < y->key)
    {
        cut_node_to_root(H, x, y);
        cascading_cut(H, y);
    }
    if (!H->minptr || x->key < H->minptr->key)
        H->minptr = x;
}

// check empty
bool fh_empty(const fibheap_c *H)
{
    return !H || H->minptr == nullptr;
}
