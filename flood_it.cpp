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

int countChunk (matrix &table) {
    int index = 0;

    vector<vector<int>> auxTable;
    auxTable.resize(n + 2, vector<int>(m + 2,-1));
    set<int> colors;

    set<ii> cellsToCheck;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {

            if (auxTable[i][j] != -1) {
                continue;
            }

            colors.insert(auxTable[i][j]);

            int color = table[i][j];
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

    //return index;
    return ceil(index*1.0/colors.size());
}

int mixed(matrix &table) {
    //cout << "Chunk Size: " << teste << '\n';
    //return teste;
    return (int) (countChunk(table) + distance_to_corner(table));
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

vector<short int> a_star (state &initial_state, matrix &initial_table) {
    priority_queue<pair<int, state>, vector<pair<int, state>>, custom_compare> pq;
    
    state actual_state = initial_state;
    matrix actual_table = initial_table;

    double startTime = omp_get_wtime();
    double endTime = omp_get_wtime();
    int counter = 0;
    
    while (!check_win(actual_table)) {

        // cout << endl;
        // // print_table(actual_table);
        // cout << "Solution: ";
        // for (auto value : actual_state.colors) 
        //     cout << value << " ";
        // cout << endl;
        // cout << "Queue size: " << pq.size() << endl;
        
        set<short int> colors = get_border_colors(actual_table);

        for (short int color : colors) {
            state next_state(actual_state);
            matrix next_table(actual_table);
            
            next_state.colors.push_back(color);
            flood(next_table, color);
            
            // Put heuristic here
            //int heuristic = next_state.colors.size() + distance_to_corner(next_table);

            //cout << "Heuristic: " << heuristic << " Colors: " << next_state.colors.size() << '\n';
            // if (heuristic < (int )next_state.colors.size()) {
            //     heuristic = next_state.colors.size();
            // }
            int heuristic = next_state.colors.size() + mixed(next_table);

            pq.push(make_pair(heuristic, next_state));
        }

        // Next state
        actual_state = pq.top().second;
        actual_table = initial_table;

        pq.pop();

        for (auto color : actual_state.colors) {
            flood(actual_table, color);
        }

        counter++;
        // if (counter % 100 == 0) {
        //     endTime = omp_get_wtime();
        //     cout << "Elapsed time (s): " << setfill(' ') << setw(3) << setprecision(4) << fixed << (endTime - startTime) << " Counter: " << counter << endl;

        //     cout << "Solution: " << actual_state.colors.size() << '\n';
        //     for (auto value : actual_state.colors) 
        //         cout << value << " ";
        //     cout << endl;
        // }

    }

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

    vector<short int> result = a_star(initial_state, table);

    // cout << "Solution: " << endl;
    cout << result.size() << endl;
    for (auto val : result) {
        cout << val << " ";
    }
    cout << endl;
    return 0;
}