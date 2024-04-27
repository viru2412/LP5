#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph {
    int V;  // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Add an edge to the graph
    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    // Sequential Depth-First Search
    void DFS(int startVertex) {
        vector<bool> visited(V, false);
        DFSUtil(startVertex, visited);
    }

    // Sequential DFS utility function
    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }
    
    // Parallel Depth-First Search
    
    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        parallelDFSUtil(startVertex, visited);
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }
    
    // Sequential Breadth-First Search
    void BFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";
            
            for (int i = 0; i < adj[v].size(); ++i) {
                int n = adj[v][i];
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }

    // Parallel Breadth-First Search
    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[v].size(); ++i) {
                int n = adj[v][i];
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};

int main() {
    // Create a graph
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    
    /*
        0 -------->1
        |         / \
        |        /   \
        |       /     \
        v       v       v
        2 ----> 3       4
        |      |
        |      |
        v      v
        5      6
    */
    
    //time for sequrential dfs
    double start; 
    double end; 
    start = omp_get_wtime(); 
    cout << "Depth-First Search (DFS): ";
    g.DFS(0);
    cout << endl;
    end = omp_get_wtime(); 
    double timeDFS=end-start;
    cout<< "Time taken by sequential DFS  "<< timeDFS<<endl;
    
    //time for parallel dfs
    start = omp_get_wtime(); 
    cout << "Parallel Depth-First Search (DFS): ";
    g.parallelDFS(0);
    cout << endl;
    end = omp_get_wtime(); 
    double timeParallelDFS=end-start;
    cout<< "Time taken by Parallel DFS  "<< timeParallelDFS<<endl;


    // time for sequential BFS
    start = omp_get_wtime(); 
    cout << "Breadth-First Search (DFS): ";
    g.BFS(0);
    cout << endl;
    end = omp_get_wtime(); 
    double timeBFS=end-start;
    cout<< "Time taken by sequential DFS  "<< timeBFS<<endl;
    
    //time for parallel BFS
    start = omp_get_wtime(); 
    cout << "Parallel Breadth-First Search (DFS): ";
    g.parallelBFS(0);
    cout << endl;
    end = omp_get_wtime(); 
    double timeParallelBFS=end-start;
    cout<< "Time taken by Parallel BFS  "<< timeParallelBFS<<endl;


    cout<<"BFS Speedup : " << timeBFS/timeParallelBFS<<endl;
    cout<<"DFS Speedup : " << timeDFS/timeParallelDFS;

    return 0;
}
