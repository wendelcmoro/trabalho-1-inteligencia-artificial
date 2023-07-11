#include <bits/stdc++.h>
#include <unistd.h>

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

// Check if reached goal state
bool check_win(matrix &table) {
    short int color = table[1][1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (color != table[i][j]) return false;
        }
    }
    return true;
}

// aux for flood function
void check_position (int i, int j, matrix &table, matrix &visited, queue<ii> &cellsToCheck, int color, int newColor) {
    if (table[i][j] == color) {
        table[i][j] = newColor;
        if (visited[i][j] == 0) {
            visited[i][j] = 1;
            cellsToCheck.push(make_pair(i, j));
        }
    }
}

// run a flood using the specified color
void flood (matrix &table, int newColor) {
    matrix visited(table.size());
    for (long unsigned int i = 0; i < visited.size(); i++) {
        visited[i].resize(table[0].size());
    }

    int color = table[1][1];
    queue<ii> cellsToCheck;
    check_position(1, 1, table, visited, cellsToCheck, color, newColor);

    while (cellsToCheck.size() != 0) {
        auto [i, j] = cellsToCheck.front();
        cellsToCheck.pop();
        
        check_position(i-1, j, table, visited, cellsToCheck, color, newColor);
        check_position(i, j-1, table, visited, cellsToCheck, color, newColor);
        check_position(i, j+1, table, visited, cellsToCheck, color, newColor);
        check_position(i+1, j, table, visited, cellsToCheck, color, newColor);
    }
}

// return a set with colors from the top left border
set<short int> get_border_colors(matrix table) {

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
    }

    return colorOnBorder;
}

// Breadth-first Search
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

// Heuristic - Regions/colors in state table
int countChunk(vector <node> &graph) {
    set<int> colors;

    for (long unsigned int i = 0; i < graph.size(); i++) {
        colors.insert(graph[i].color);
    }

    return (int) ceil(graph.size()*1.0/colors.size());
}

// Heuristic - Call for bfs to return the min greatest distance from node 0 to all others in the graph
int maxDistance(vector <node> &graph) {
    return bfs(0, graph);
}

// Heuristic - determine to distance to border
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
}

// Used to help count nodes in createGraph function
void checkChunck (int i, int j, set<ii> &cellsToCheck, int color, matrix &table,  vector<vector<int>> &auxTable) {
    if (table[i][j] == color && auxTable[i][j] == -1) {
        cellsToCheck.insert(make_pair(i, j));
    }
}

// Used to help create edges for a node in createGraph function
void checkBorderAux(int i, int j, int index, vector<node> &graph, vector<vector<int>> &auxTable) {
    if (auxTable[i][j] != -1 && auxTable[i][j] != index) {
        graph[index].edges.insert(make_pair(index, auxTable[i][j]));
        graph[auxTable[i][j]].edges.insert(make_pair(auxTable[i][j], index));
    }
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

// Custom compare for priority-queue
struct custom_compare {
    template<typename T, typename U>
    bool operator()(T const& lhs, U const& rhs) const {
        if (lhs.first > rhs.first) return true;
        return false;
    }
};

// greedy search using three different heuristics
vector<short int> greedy_search (state &initial_state, matrix &initial_table) {
    priority_queue<pair<tuple<int, int, int>, state>, vector<pair<tuple<int, int, int>, state>>, custom_compare> pq;
    
    state actual_state = initial_state;
    matrix actual_table = initial_table;

    while (!check_win(actual_table)) {
        set<short int> colors = get_border_colors(actual_table);
        for (short int color : colors) {
            state next_state = actual_state;
            matrix next_table = actual_table;
            
            next_state.colors.push_back(color);
            flood(next_table, color);

            // get Heuristics values
            vector <node> graph = createGraph(next_table);
            int heuristic = next_state.colors.size() + maxDistance(graph);
            int heuristic2 = next_state.colors.size() + countChunk(graph);
            int heuristic3 = next_state.colors.size() + distance_to_corner(next_table);

            pq.push(make_pair(make_tuple(heuristic, heuristic3, heuristic2), next_state));
        }

        // Next state
        actual_state = pq.top().second;
        actual_table = initial_table;
        pq.pop();

        for (auto color : actual_state.colors) {
            flood(actual_table, color);
        }

        pq = priority_queue<pair<tuple<int, int, int>, state>, vector<pair<tuple<int, int, int>, state>>, custom_compare>();
    }

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

    // call for the greedy_search
    state initial_state;
    vector<short int> result = greedy_search(initial_state, table);

    // print solution
    cout << result.size() << endl;
    for (auto val : result) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}