#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include "Cube.h"
#include "robin_hood.h"

using namespace std;

const vector<vector<int>> movePrune = {{3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 
                                {0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 
                                {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 
                                {0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17}, {0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17}, 
                                {0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17}, {0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17}, {0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17}, 
                                {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}};

vector<vector<vector<int>>> pruneTable;

vector<vector<vector<int>>> generatePruneTable() {
    Cube c;
    robin_hood::unordered_set<Cube, CubeHash> seen;
    seen.insert(Cube());
    vector<vector<vector<int>>> res;
    
    for (int m1 = 0; m1 < 19; m1++) {
        res.push_back(vector<vector<int>>());
        
        for (int m2 = 0; m2 < 19; m2++) {
            res[m1].push_back(vector<int>());
            
            for (int m3 = 0; m3 < 18; m3++) {
                if (m1 != 18) {
                    res[m1][m2].push_back(m3);
                }
            }
        }
    }
    
    for (int i = 0; i < 18; i++) {
        c.makeMove(i);
        seen.insert(Cube(c.faces));
        
        for (int move : movePrune[i]) {
            c.makeMove(move);
            seen.insert(Cube(c.faces));
            c.unmakeMove(move);
        }
        
        c.unmakeMove(i);
    }
    
    for (int m1 = 0; m1 < 18; m1++) {
        c.makeMove(m1);
        seen.insert(Cube(c.faces));
        
        for (int m2 = 0; m2 < 18; m2++) {
            c.makeMove(m2);
            seen.insert(Cube(c.faces));
            for (int m3 = 0; m3 < 18; m3++) {
                c.makeMove(m3);
                
                if (seen.count(c) > 0) {
                    auto it = find(res[m1][m2].begin(), res[m1][m2].end(), m3);
                    if (it != res[m1][m2].end()) {
                        res[m1][m2].erase(it);
                    }
                } else {
                    seen.insert(Cube(c.faces));
                }
                
                c.unmakeMove(m3);
            }
            
            c.unmakeMove(m2);
        }
        
        c.unmakeMove(m1);
    }
    
    for (int m2 = 0; m2 < 19; m2++) {
        for (int m3 = 0; m3 < movePrune[m2].size(); m3++) {
            res[18][m2].push_back(movePrune[m2][m3]);
        }
    }
    
    return res;
}

int countdfs(int depth, int currentDepth, Cube c, robin_hood::unordered_set<Cube, CubeHash>& seen, int prevMove, int prevPrev){
        if (depth == currentDepth){
           return (int)seen.insert(c).second;
        }
        int res = 0;
        for (int move : pruneTable[prevPrev][prevMove]){
            c.makeMove(move);
            res += countdfs(depth, currentDepth+1, c, seen, move, prevMove);
            c.unmakeMove(move);
        }
        return res;
    }

int countdfs(int depth){
        Cube c;
        robin_hood::unordered_set<Cube, CubeHash> seen;
        for (int i = 0; i < depth; i++){
            countdfs(i, 0, c, seen, 18, 18);
        }
       
        return countdfs(depth, 0, c, seen, 18, 18);
    }

void perft(){
    auto start = chrono::high_resolution_clock::now();
    countdfs(6);
    auto end = chrono::high_resolution_clock::now();    
    auto duration = duration_cast<chrono::nanoseconds>(end - start);
    double e = duration.count();
    cout << fixed << "Iterative deepening dfs with hash table and pruning: " << 8240087.0/e*1000000000 << " nodes per sec" << endl;
    Cube c;
    start = chrono::high_resolution_clock::now();
    for (long long i = 0; i < 1000000; i++){
        c.makeMove(i%18);
    }
    end = chrono::high_resolution_clock::now();  
    volatile bool b = c==Cube();
    duration = end - start;
    e = duration.count();
    cout << fixed << "makeMove(): " << (1E15)/e << " moves per sec" << endl;
}

int main() {
    pruneTable = generatePruneTable();
    cout << (countdfs(5) == 574908 ? "generation good" : "generation error") << endl;
    perft();
    return 0;
}
