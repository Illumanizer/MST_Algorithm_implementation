#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <climits>
#include "graph.h"
#include "prim.h"
#include "kruskal.h"
using namespace std;

// helper function to sort edges
static void sort_edges(vector<Edge> &edges)
{
    for (int i = 0; i < edges.size(); ++i)
    {
        if (edges[i].u > edges[i].v)
            swap(edges[i].u, edges[i].v);
    }

    for (int i = 1; i < edges.size(); ++i)
    {
        Edge key = edges[i];
        int j = i - 1;
        while (j >= 0 && (edges[j].u > key.u || (edges[j].u == key.u && edges[j].v > key.v)))
        {
            edges[j + 1] = edges[j];
            --j;
        }
        edges[j + 1] = key;
    }
}

int main(int argc, char **argv)
{
    string inputName = (argc >= 2 ? argv[1] : "input.txt");
    string outputName = (argc >= 3 ? argv[2] : "output.txt");

    ifstream fin(inputName);
    if (!fin)
    {
        cerr << "Error: Cannot open input file.\n";
        return 1;
    }

    vector<vector<int>> mat;
    string line;
    while (getline(fin, line))
    {
        if (line.find_first_not_of(" \t\r\n") == string::npos)
            continue;
        istringstream iss(line);
        vector<int> row;
        int x;
        while (iss >> x)
            row.push_back(x);
        if (!row.empty())
            mat.push_back(row);
    }
    fin.close();

    if (mat.empty())
    {
        cerr << "Error: Input file empty or invalid.\n";
        return 1;
    }

    int n = mat.size();
    for (int i = 0; i < n; ++i)
    {
        if (mat[i].size() != n)
        {
            cerr << "Error: Non-square matrix.\n";
            return 1;
        }
    }

    Graph G;
    try
    {
        G = Graph::from_cost_matrix(mat);
    }
    catch (...)
    {
        cerr << "Error: Graph creation failed.\n";
        return 1;
    }

    auto start = chrono::steady_clock::now();
    auto prim_res = prim_mst(G);
    auto end = chrono::steady_clock::now();
    double prim_time = chrono::duration<double, milli>(end - start).count();

    start = chrono::steady_clock::now();
    auto kruskal_res = kruskal_mst(G);
    end = chrono::steady_clock::now();
    double kruskal_time = chrono::duration<double, milli>(end - start).count();

    sort_edges(prim_res.first);
    sort_edges(kruskal_res.first);

    ofstream fout(outputName);
    if (!fout)
        cerr << "Warning: Could not open output file.\n";

    auto printBoth = [&](string s)
    {
        cout << s;
        if (fout)
            fout << s;
    };

    printBoth("Prim’s MST (total cost: " + to_string(prim_res.second) +
              "; runtime: " + to_string(prim_time) + " ms)\n");
    for (auto &e : prim_res.first)
        printBoth("(" + to_string(e.u) + "," + to_string(e.v) + ")\n");

    printBoth("Kruskal’s MST (total cost: " + to_string(kruskal_res.second) +
              "; runtime: " + to_string(kruskal_time) + " ms)\n");
    for (auto &e : kruskal_res.first)
        printBoth("(" + to_string(e.u) + "," + to_string(e.v) + ")\n");

    fout.close();
    return 0;
}