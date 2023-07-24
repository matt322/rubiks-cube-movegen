#include <iostream>
#include <string>
#include <bit>
#include <vector>
#include <unordered_map>
using namespace std;

const char bitToColor[6] = {'W', 'O', 'G', 'R', 'B', 'Y'};
const int faceAdj[6][4] = {{4, 3, 2, 1}, {0, 2, 5, 4}, {0, 3, 5, 1}, {0, 4, 5, 2}, {0, 1, 5, 3}, {2, 3, 4, 1}};
const int neighborEdge[6][4] = {{0, 0, 0, 0}, {3, 3, 3, 1}, {2, 3, 0, 1}, {1, 3, 1, 1}, {0, 3, 2, 1}, {2, 2, 2, 2}};
const unordered_map<string, int> movetoint = {{"U", 0}, {"U2", 1}, {"U'", 2}, {"L", 3}, {"L2", 4}, {"L'", 5},{"F", 6}, {"F2", 7}, {"F'", 8},{"R", 9}, {"R2", 10}, {"R'", 11},{"B", 12}, {"B2", 13}, {"B'", 14},{"D", 15}, {"D2", 16}, {"D'", 17}};


class Cube {
    public:
        unsigned int faces[6];
        
        Cube() : faces {0,286331153,572662306,858993459,1145324612,1431655765}{}
        
        Cube(const unsigned int* f){
            copy(f + 0, f + 6, faces);
        }

        bool operator==(const Cube& other) const {
            return other.faces[0]==this->faces[0]&&other.faces[1]==this->faces[1]&&other.faces[2]==this->faces[2]&&other.faces[3]==this->faces[3]&&other.faces[4]==this->faces[4]&&other.faces[5]==this->faces[5];
        }

        string faceString(int face){
            string res = "        ";
            for (int i = 0; i < 8; i++){
                res[7-i] = bitToColor[(face & (15 << i*4)) >> i*4];
            }
            return res;
        }

        string toString(){
            return faceString(faces[0])+" "+faceString(faces[1])+" "+faceString(faces[2])+" "+faceString(faces[3])+" "+faceString(faces[4])+" "+faceString(faces[5]);
        }
        void makeMove(string move){
            makeMove(movetoint.at(move));
        }

        void makeMove(int move) {
            int angle = move % 3 + 1;
            int face = move / 3;
            faces[face] = rotr(faces[face], (angle)*8);
            if (angle==1){
                rotateAdjRight(face);
                return;
            } if(angle==2){
                rotateAdjFlip(face);
                return;
            } 
            rotateAdjLeft(face);  
        }

        void unmakeMove(int move){
            makeMove(move - (2 * (move % 3)) + 2);
        }
        

    private:
        void setEdge(int edge, int faceNum, unsigned int n){
            faces[faceNum] = faces[faceNum] & rotr(0b00000000000011111111111111111111U, edge*8) | rotr(n, edge*8);
        }

        int getEdge(int edge, unsigned int face){
            return rotl(face, 8*edge)&0b11111111111100000000000000000000;
        }

        void rotateAdjRight(int faceNum){ 
            int n0 = neighborEdge[faceNum][0];
            int n1 = neighborEdge[faceNum][1];
            int n2 = neighborEdge[faceNum][2];
            int n3 = neighborEdge[faceNum][3];
            int f0 = faceAdj[faceNum][0];
            int f1 = faceAdj[faceNum][1];
            int f2 = faceAdj[faceNum][2];
            int f3 = faceAdj[faceNum][3];
            int upEdge = getEdge(n0, faces[f0]);
            setEdge(n0, f0, getEdge(n3, faces[f3]));
            setEdge(n3, f3, getEdge(n2, faces[f2]));
            setEdge(n2, f2, getEdge(n1, faces[f1]));
            setEdge(n1, f1, upEdge);
        }

        void rotateAdjLeft(int faceNum){ 
            int n0 = neighborEdge[faceNum][0];
            int n1 = neighborEdge[faceNum][3];
            int n2 = neighborEdge[faceNum][2];
            int n3 = neighborEdge[faceNum][1];
            int f0 = faceAdj[faceNum][0];
            int f1 = faceAdj[faceNum][3];
            int f2 = faceAdj[faceNum][2];
            int f3 = faceAdj[faceNum][1];
            int upEdge = getEdge(n0, faces[f0]);
            setEdge(n0, f0, getEdge(n3, faces[f3]));
            setEdge(n3, f3, getEdge(n2, faces[f2]));
            setEdge(n2, f2, getEdge(n1, faces[f1]));
            setEdge(n1, f1, upEdge);
        }

        void rotateAdjFlip(int faceNum){ 
            int n0 = neighborEdge[faceNum][0];
            int n1 = neighborEdge[faceNum][2];
            int n2 = neighborEdge[faceNum][3];
            int n3 = neighborEdge[faceNum][1];
            int f0 = faceAdj[faceNum][0];
            int f1 = faceAdj[faceNum][2];
            int f2 = faceAdj[faceNum][3];
            int f3 = faceAdj[faceNum][1];
            int upEdge = getEdge(n0, faces[f0]);
            setEdge(n0, f0, getEdge(n1, faces[f1]));
            setEdge(n1, f1, upEdge);
            upEdge = getEdge(n2, faces[f2]);
            setEdge(n2, f2, getEdge(n3, faces[f3]));
            setEdge(n3, f3, upEdge);
        }
};

struct CubeHash {
    size_t operator()(const Cube& cube) const {
        const size_t fnvOffsetBasis = 2166136261U;
        const size_t fnvPrime = 16777619U;
        size_t hash = fnvOffsetBasis;

        for (unsigned int c : cube.faces) {
            hash ^= static_cast<size_t>(c);
            hash *= fnvPrime;
        }

        return hash;
    }
};

