#include <bits/stdc++.h>
#include <unistd.h>
#include <omp.h>

#define esp ' '
#define MAX_SIZE 200
#define MAX_COLOR 20

#define all(x) (x).begin(), (x).end()
#define mset(x, y) memset(&x, (y), sizeof(x))

using namespace std;
using ll = long long;
using ii = pair<int, int>;
using matrix = vector<vector<char>>;

int n, m, k;

// Data types
struct state {
    vector<short int> colors;
};

struct node {
    int color;
    set<ii> edges;
};

// 
// Outputs
template <typename T>
void print_table (vector<vector<T>> &table) {
    cout << "Table: " << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << setfill(' ') << setw(2) << +table[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << flush;
}

// 
// Checkers
void check_position (int i, int j, matrix &table, matrix &visited, queue<ii> &cellsToCheck, int color, int newColor) {
    if (table[i][j] == color) {
        table[i][j] = newColor;
        if (visited[i][j] == 0) {
            visited[i][j] = 1;
            cellsToCheck.push(make_pair(i, j));
        }
    }
}

// New check for vector
bool check_win(matrix &table) {
    short int color = table[1][1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (color != table[i][j]) return false;
        }
    }
    return true;
}


//
// Operations
void flood (matrix &table, int newColor) {
    // cout << "start flood " << endl;
    matrix visited(table.size());
    for (long unsigned int i = 0; i < visited.size(); i++) {
        visited[i].resize(table[0].size());
    }

    int color = table[1][1];
    queue<ii> cellsToCheck;
    check_position(1, 1, table, visited, cellsToCheck, color, newColor);
    // cellsToCheck.push(make_pair(1,1));

    while (cellsToCheck.size() != 0) {
        auto [i, j] = cellsToCheck.front();
        cellsToCheck.pop();

        // cout << i << " " << j << " " << cellsToCheck.size() << endl;
        
        check_position(i-1, j, table, visited, cellsToCheck, color, newColor);
        check_position(i, j-1, table, visited, cellsToCheck, color, newColor);
        check_position(i, j+1, table, visited, cellsToCheck, color, newColor);
        check_position(i+1, j, table, visited, cellsToCheck, color, newColor);
    }
}

set<short int> get_border_colors(matrix table) {

    // set<int> colorOnBorder;
    // for (int i = 1; i <= n; i++) {
    //     for (int j = 1; j <= m; j++) {
    //         colorOnBorder.insert(table[i][j]);
    //     }
    // }

    matrix visited(table.size());
    for (long unsigned int i = 0; i < visited.size(); i++) {
        visited[i].resize(table[0].size());
    }

    queue<ii> cellsToCheck;
    cellsToCheck.push(make_pair(1,1));
    int color = table[1][1];

    set<short int> colorOnBorder;

    while (cellsToCheck.size() != 0) {
        auto [i, j] = cellsToCheck.front();
        cellsToCheck.pop();

        if (visited[i][j] == 1) {
            continue;
        }
        
        if (table[i][j] == color) {
            visited[i][j] = 1;
            cellsToCheck.push(make_pair(i-1, j));
            cellsToCheck.push(make_pair(i, j-1));
            cellsToCheck.push(make_pair(i, j+1));
            cellsToCheck.push(make_pair(i+1, j));
        } else if (table[i][j] != 0) {
            colorOnBorder.insert(table[i][j]);
        }
        // cout << cellsToCheck.size() << endl;
    }

    return colorOnBorder;
}

// 
// Not used
vector<int> count_per_color (matrix &table) {
    vector<int> amount(k+1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            amount[table[i][j]]++;
        }
    }

    return amount;
}

// 
// Best heuristic now
int distance_to_corner (matrix &table) {
    matrix visited(table.size());
    for (long unsigned int i = 0; i < visited.size(); i++) {
        visited[i].resize(table[0].size());
    }

    queue<ii> cellsToCheck;
    cellsToCheck.push(make_pair(1,1));
    int color = table[1][1];

    int minRB = 100000000;

    while (cellsToCheck.size() != 0) {
        auto [i, j] = cellsToCheck.front();
        cellsToCheck.pop();

        if (visited[i][j] == 1) {
            continue;
        }
        visited[i][j] = 1;
        
        if (table[i][j] == color) {

            int right_bottom = abs(n - i) + abs(m - j);
            if (right_bottom < minRB) {
                minRB = right_bottom;
            }

            cellsToCheck.push(make_pair(i-1, j));
            cellsToCheck.push(make_pair(i, j-1));
            cellsToCheck.push(make_pair(i, j+1));
            cellsToCheck.push(make_pair(i+1, j));
        }
    }

    return minRB;
    // return minLB + minRB + minRT;
}

// 
// New heuristic, per chunk
void checkChunck (int i, int j, set<ii> &cellsToCheck, int color, matrix &table,  vector<vector<int>> &auxTable) {
    if (table[i][j] == color && auxTable[i][j] == -1) {
        cellsToCheck.insert(make_pair(i, j));
    }
}

void checkBorderAux(int i, int j, int index, vector<node> &graph, vector<vector<int>> &auxTable) {
    if (auxTable[i][j] != -1 && auxTable[i][j] != index) {
        graph[index].edges.insert(make_pair(index, auxTable[i][j]));
        graph[auxTable[i][j]].edges.insert(make_pair(auxTable[i][j], index));
    }
}

void DFS(vector<node> &graph, long unsigned int src, int prev_len, int *max_len, vector <bool> &visited) {

    // cout << "source: " << src << '\n';
    // Mark the src node visited
    visited[src] = 1;
 
    // curr_len is for length of cable from src
    // city to its adjacent city
    int curr_len = 0;
 
    // Adjacent is pair type which stores
    // destination city and cable length
    // pair<int, int> adjacent;
 
    // Traverse all adjacent
    for (set<ii>:: iterator it = graph[src].edges.begin(); it != graph[src].edges.end();/* blank */) {
        //cout << "source testing: " << it->first << "\n";
        if (!visited[it->second]) {
            //cout << "source test: " << visited[it->first] << '\n';
            // Total length of cable from src city
            // to its adjacent
            curr_len = prev_len + it->first;
 
            // Call DFS for adjacent city
            DFS(graph, it->second, curr_len, max_len,  visited);
        }

        // If total cable length till now greater than
        // previous length then update it
        if ((*max_len) < curr_len) {
            *max_len = curr_len;
        }
 
        // make curr_len = 0 for next adjacent
        curr_len = 0;

        ++it;
    }

    // for (long unsigned int i=0; i< graph[src].edges.size(); i++) {
    //     // Adjacent element
    //     adjacent = graph[src].edges[i];
 
    //     // If node or city is not visited
    //     if (!visited[adjacent.first]) {
    //         // Total length of cable from src city
    //         // to its adjacent
    //         curr_len = prev_len + adjacent.second;
 
    //         // Call DFS for adjacent city
    //         DFS(graph, adjacent.first, curr_len, max_len,  visited);
    //     }
 
    //     // If total cable length till now greater than
    //     // previous length then update it
    //     if ((*max_len) < curr_len)
    //         *max_len = curr_len;
 
    //     // make curr_len = 0 for next adjacent
    //     curr_len = 0;
    // }
}

int bfs(int u, vector<node> &graph) {
    //  mark all distance with -1
    int dis[graph.size()];
    memset(dis, -1, sizeof(dis));
 
    queue<long unsigned int> q;
    q.push(u);
 
    //  distance of u from u will be 0
    dis[u] = 0;

    int maxDis = 0;
 
    while (!q.empty()) {
        int t = q.front();
        q.pop();

        //  loop for all adjacent nodes of node-t
        for (set<ii>:: iterator it = graph[t].edges.begin(); it != graph[t].edges.end();/* blank */) {
            int v = it->second;

            // push node into queue only if
            // it is not visited already
            if (dis[v] == -1) {
                q.push(v);
 
                // make distance of v, one more
                // than distance of t
                dis[v] = dis[t] + 1;

                if (dis[v] > maxDis) {
                    maxDis = dis[v];
                }
            }

            ++it;
        }
    }

    return maxDis;
}

int countChunk(vector <node> &graph) {
    set<int> colors;

    for (long unsigned int i = 0; i < graph.size(); i++) {
        colors.insert(graph[i].color);
    }

    return (int) ceil(graph.size()*1.0/colors.size());
}

int maxDistance(vector <node> &graph) {
    return bfs(0, graph);
}

// create the graph of contiguous colors in the table
vector<node> createGraph (matrix &table) {
    int index = 0;

    vector<vector<int>> auxTable;
    auxTable.resize(n + 2, vector<int>(m + 2,-1));
    set<int> colors;

    set<ii> cellsToCheck;

    vector<node> graph;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {

            if (auxTable[i][j] != -1) {
                continue;
            }

            node aux;
            graph.push_back(aux);

            checkBorderAux(i, j - 1, index, graph, auxTable);
            checkBorderAux(i, j + 1, index, graph, auxTable);
            checkBorderAux(i - 1, j, index, graph, auxTable);
            checkBorderAux(i + 1, j, index, graph, auxTable);

            int color = table[i][j];
            colors.insert(auxTable[i][j]);

            graph[index].color = auxTable[i][j];
            
            cellsToCheck.insert(make_pair(i,j));

            while (cellsToCheck.size() != 0) {
                auto [i, j] = *cellsToCheck.begin();
                cellsToCheck.erase(cellsToCheck.begin());
                
                auxTable[i][j] = index;

                checkChunck(i-1, j, cellsToCheck, color, table, auxTable);
                checkChunck(i, j-1, cellsToCheck, color, table, auxTable);
                checkChunck(i, j+1, cellsToCheck, color, table, auxTable);
                checkChunck(i+1, j, cellsToCheck, color, table, auxTable);
            }

            index++;
        }
    }

    return graph;
}

// 
// Algorithm
struct custom_compare {
    template<typename T, typename U>
    bool operator()(T const& lhs, U const& rhs) const {
        if (lhs.first > rhs.first) return true;
        return false;
    }
};

vector<short int> greedy_search (state &initial_state, matrix &initial_table) {
    priority_queue<pair<tuple<int, int, int>, state>, vector<pair<tuple<int, int, int>, state>>, custom_compare> pq;
    
    state actual_state = initial_state;
    matrix actual_table = initial_table;

    double startTime = omp_get_wtime();
    double endTime = omp_get_wtime();
    int counter = 0;

    long unsigned int maxStates = n*m;
    while (!check_win(actual_table)) {
        pq = priority_queue<pair<tuple<int, int, int>, state>, vector<pair<tuple<int, int, int>, state>>, custom_compare>();
        //priority_queue<pair<int, state>, vector<pair<int, state>>, custom_compare> pq;
        // if (pq.size() > maxStates) {
        //     break;
        // }
        // if (endTime - startTime > 30) {
        //     break;
        // }

        set<short int> colors = get_border_colors(actual_table);
        for (short int color : colors) {
            state next_state = actual_state;
            matrix next_table = actual_table;
            
            next_state.colors.push_back(color);
            flood(next_table, color);

            // Put heuristic here
            vector <node> graph = createGraph(next_table);
            int heuristic = next_state.colors.size() + maxDistance(graph);
            int heuristic2 = next_state.colors.size() + countChunk(graph);
            int heuristic3 = next_state.colors.size() + distance_to_corner(next_table);

            //int heuristic = mixed(next_table);
            //cout << "colors: " << next_state.colors.size() << '\n';
            pq.push(make_pair(make_tuple(heuristic, heuristic3, heuristic2), next_state));
        }

        // Next state
        actual_state = pq.top().second;
        actual_table = initial_table;
        pq.pop();

        for (auto color : actual_state.colors) {
            flood(actual_table, color);
        }

        counter++;
        
        if (counter % 100 == 0) {
            cout << "Solution: " << actual_state.colors.size() << '\n';
            endTime = omp_get_wtime();
            cout << "Elapsed time (s): " << setfill(' ') << setw(3) << setprecision(4) << fixed << (endTime - startTime) << " Counter: " << counter << endl;

            cout << "PQ size: " << pq.size() << '\n';
            cout << "Solution: " << actual_state.colors.size() << '\n';
            for (auto value : actual_state.colors) 
                cout << value << " ";
            cout << endl;
         }
        
        endTime = omp_get_wtime();
    }

    // priority_queue<pair<int, state>, vector<pair<int, state>>, custom_compare> pq2;
    // while (!check_win(actual_table)) {
    //     pq2 = priority_queue<pair<int, state>, vector<pair<int, state>>, custom_compare>();
    //     set<short int> colors = get_border_colors(actual_table);
    //     for (short int color : colors) {
    //         state next_state = actual_state;
    //         matrix next_table = actual_table;
            
    //         next_state.colors.push_back(color);
    //         flood(next_table, color);

    //         // Put heuristic here

    //         vector <node> graph = createGraph(next_table);
    //         //int heuristic = countChunk(graph) + distance_to_corner(next_table);
    //         int heuristic = maxDistance(graph);
            
    //         //int heuristic = mixed(next_table);
    //         //cout << "colors: " << next_state.colors.size() << '\n';
    //         pq2.push(make_pair(heuristic, next_state));
    //     }

    //     // Next state
    //     actual_state = pq2.top().second;
    //     actual_table = initial_table;
    //     pq2.pop();

    //     for (auto color : actual_state.colors) {
    //         flood(actual_table, color);
    //     }

    //     counter++;
    //     if (counter % 100 == 0) {
    //         endTime = omp_get_wtime();
    //         cout << "Elapsed time (s): " << setfill(' ') << setw(3) << setprecision(4) << fixed << (endTime - startTime) << " Counter: " << counter << endl;

    //         cout << "Solution: " << actual_state.colors.size() << '\n';
    //         for (auto value : actual_state.colors) 
    //             cout << value << " ";
    //         cout << endl;
    //     }
    // }

    // cout << "Computed states: " << counter << endl;
    return actual_state.colors;
}

// 
// main
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m >> k;

    matrix table(n+2);
    table[0].resize(m+2, 0);
    table[n+1].resize(m+2, 0);

    for (int i = 1; i <= n; i++) {
        table[i].push_back(0);
        for (int j = 1; j <= m; j++) {
            short int aux;
            cin >> aux;
            table[i].push_back(aux);
        }
        table[i].push_back(0);
    }

    // cout << ">> Solving problem" << endl;
    // cout << "Dimensions: " << n << " " << m << endl;
    // print_table(table);

    state initial_state;

    vector<short int> result = greedy_search(initial_state, table);

    //cout << "Solution: " << endl;
    cout << result.size() << endl;
    for (auto val : result) {
        cout << val << " ";
    }
    cout << endl;

    // int teste = countChunk(table, 0);

    // cout << "Longest Path: " << teste << "\n\n";
    return 0;
}