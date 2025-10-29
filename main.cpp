#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include "graph.h"
#include "prim.h"
#include "kruskal.h"

using namespace std;

static void sort_edges(vector<Edge> &edges)
{ // ensure each edge stored as (min,max)
    for (auto &e : edges)
    {
        if (e.u > e.v)
        {
            int t = e.u;
            e.u = e.v;
            e.v = t;
        }
    } // insertion sort by (u, then v)
    for (int i = 1; i < edges.size(); ++i)
    {
        Edge key = edges[i];
        int j = (int)i - 1; // compare edges[j] > key ?
        while (j >= 0)
        {
            bool greater = false;
            if (edges[j].u > key.u)
                greater = true;
            else if (edges[j].u == key.u && edges[j].v > key.v)
                greater = true;
            if (!greater)
                break;
            edges[j + 1] = edges[j];
            --j;
        }
        edges[j + 1] = key;
    }
}

int main(int argc, char **argv)
{
    // ---------- File Handling ----------
    string filename = (argc >= 2 ? argv[1] : "input.txt");
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Cannot open input file '" << filename << "'.\n";
        return 1;
    }

    // ---------- Matrix Reading ----------
    vector<vector<long long>> mat;
    string line;
    while (getline(file, line))
    {
        if (line.find_first_not_of(" \t\r\n") == string::npos)
            continue;

        istringstream iss(line);
        vector<long long> row;
        long long x;
        while (iss >> x)
            row.push_back(x);
        if (!row.empty())
            mat.push_back(row);
    }

    if (mat.empty())
    {
        cerr << "Error: Input file is empty or invalid.\n";
        return 1;
    }

    int n = mat.size();
    for (int i = 0; i < n; ++i)
    {
        if ((int)mat[i].size() != n)
        {
            cerr << "Error: Row " << i + 1 << " has " << mat[i].size()
                 << " columns, but expected " << n << ".\n";
            return 1;
        }
    }

    // ---------- Graph Creation ----------
    Graph G;
    try
    {
        G = Graph::from_cost_matrix(mat);
    }
    catch (const exception &e)
    {
        cerr << "Error constructing graph: " << e.what() << "\n";
        return 1;
    }

    // ---------- Run Algorithms ----------
    auto start = chrono::steady_clock::now();
    auto prim_res = prim_mst(G);
    auto end = chrono::steady_clock::now();
    double prim_ms = chrono::duration<double, milli>(end - start).count();

    start = chrono::steady_clock::now();
    auto kruskal_res = kruskal_mst(G);
    end = chrono::steady_clock::now();
    double kruskal_ms = chrono::duration<double, milli>(end - start).count();

    sort_edges(prim_res.first);
    sort_edges(kruskal_res.first);

    // ---------- Output to file ----------
    ofstream fout("output.txt");
    if (!fout)
    {
        cerr << "Warning: Could not open output.txt for writing.\n";
    }

    // helper lambda to print to both console & file
    auto print_both = [&](const string &s)
    {
        cout << s;
        if (fout)
            fout << s;
    };

    // ---------- Output ----------
    cout << fixed << setprecision(3);
    fout << fixed << setprecision(3);

    print_both("Prim’s algorithm MST (total cost: " + to_string(prim_res.second) +
               "; runtime: " + to_string(prim_ms) + " ms)\n");
    for (auto &e : prim_res.first)
        print_both("(" + to_string(e.u) + "," + to_string(e.v) + ")\n");

    print_both("\nKruskal’s algorithm MST (total cost: " + to_string(kruskal_res.second) +
               "; runtime: " + to_string(kruskal_ms) + " ms)\n");
    for (auto &e : kruskal_res.first)
        print_both("(" + to_string(e.u) + "," + to_string(e.v) + ")\n");

    fout.close();
    return 0;
}
