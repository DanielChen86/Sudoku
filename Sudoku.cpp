//
//  main.cpp
//  Sudoku
//
//  Created by 陳柏廷 on 2020/11/10.
//


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <ctime>

using namespace std;

const int boardsize=9;
const int difficulty=6;
string read_file="/Users/chenboting/Desktop/question/question3.csv";

// show a vector
void show_vec(vector<int> vec)
{
    cout<<"{";
    for (int i=0;i<vec.size();i++)
    {
        if (i<vec.size()-1)
        {cout<<vec[i]<<",";}
        else
        {cout<<vec[i]<<"}"<<endl;}
    }
}

// show a vector
void show_vec(vector<pair<int,int>> vec)
{
    cout<<"{";
    for (int i=0;i<vec.size();i++)
    {
        if (i<vec.size()-1)
        {cout<<"{"<<vec[i].first<<","<<vec[i].second<<"}"<<",";}
        else
        {cout<<"{"<<vec[i].first<<","<<vec[i].second<<"}}"<<endl;}
    }
}

// show a vector
void show_vec(vector<vector<int>> vec)
{
    cout<<"{";
    for (int i=0;i<vec.size();i++)
    {
        cout<<"{";
        for (int j=0;j<vec[i].size();j++)
        {
            if (j<vec[i].size()-1)
            {cout<<vec[i][j]<<",";}
            else
            {cout<<vec[i][j];}
        }
        cout<<"}";
        if (i<vec.size()-1)
        {
            cout<<",";
        }
    }
    cout<<"}"<<endl;
}

// remove Pair from vec
vector<pair<int,int>> vec_remove(vector<pair<int,int>> vec,pair<int,int> Pair)
{
    vector<pair<int,int>> vec_temp;
    for (int i=0;i<vec.size();i++)
    {
        if (vec[i]!=Pair)
        {vec_temp.push_back(vec[i]);}
    }
    return vec_temp;
}

// check if Integer is in vec
bool in_vec(vector<int> vec,int Integer)
{
    int i;
    for (i=0;i<vec.size();i++)
    {
        if (vec[i]==Integer)
        {
            return true;
            break;
        }
    }
    return false;
}

int count_vec(vector<int> vec, int Integer)
{
    int result=0;
    for (int i=0;i<vec.size();i++)
    {
        if (vec[i]==Integer)
        {
            result++;
        }
    }
    return result;
}

// return the block of a given position Pair
int position_to_block(pair<int,int> Pair)
{
    if (Pair.first<0 or Pair.first>=boardsize or Pair.second<0 or Pair.second>=boardsize)
    {
        cout<<"error (position_to_block): not a legal position while getting block."<<endl;
        return boardsize;
    }
    int row=Pair.first/3;
    int col=Pair.second/3;
    if (0<=3*row+col and 3*row+col<boardsize)
    {
        return 3*row+col;
    }
    else
    {
        cout<<"error (position_to_block): not a legal position while getting block."<<endl;
        return boardsize;
    }
}

// return all positions that are in block Integer
vector<pair<int,int>> block_to_positions(int Integer)
{
    if (Integer<0 or Integer>=boardsize)
    {
        cout<<"error (block_to_positions): not a legal block while getting positions."<<endl;
        return {{boardsize,boardsize}};
    }
    pair<int,int> pair_block={Integer/3,Integer%3};
    vector<pair<int,int>> result;
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
            result.push_back({3*pair_block.first+i,3*pair_block.second+j});
        }
    }
    return result;
}

// return all positions that are in the same row as Pair, not including Pair
vector<pair<int,int>> same_row(pair<int,int> Pair)
{
    if (Pair.first<0 or Pair.first>=boardsize or Pair.second<0 or Pair.second>=boardsize)
    {
        cout<<"error (same_row): not a legal position while getting same_row."<<endl;
        return {{boardsize,boardsize}};
    }
    int k;
    vector<pair<int,int>> result;
    for (k=0;k<boardsize;k++)
    {
        if (k!=Pair.second)
        {
            result.push_back({Pair.first,k});
        }
    }
    return result;
}

// return all positions that are in the same col as Pair, not including Pair
vector<pair<int,int>> same_col(pair<int,int> Pair)
{
    if (Pair.first<0 or Pair.first>=boardsize or Pair.second<0 or Pair.second>=boardsize)
    {
        cout<<"error (same_col): not a legal position while getting same_col."<<endl;
        return {{boardsize,boardsize}};
    }
    int k;
    vector<pair<int,int>> result;
    for (k=0;k<boardsize;k++)
    {
        if (k!=Pair.first)
        {
            result.push_back({k,Pair.second});
        }
    }
    return result;
}

// return all positions that are in the same block as Pair, not including Pair
vector<pair<int,int>> same_block(pair<int,int> Pair)
{
    if (Pair.first<0 or Pair.first>=boardsize or Pair.second<0 or Pair.second>=boardsize)
    {
        cout<<"error (same_block): not a legal position while getting same_block."<<endl;
        return {{boardsize,boardsize}};
    }
    vector<pair<int,int>> vec_temp=block_to_positions(position_to_block(Pair));
    vector<pair<int,int>> result;
    vector<pair<int,int>>::iterator it=vec_temp.begin();
    while (it!=vec_temp.end())
    {
        if (*it!=Pair)
        {
            result.push_back(*it);
        }
        it++;
    }
    return result;
}

// turn {{1,2},{3,4,5}} into {{1,3},{1,4},{1,5},{2,3},{2,4},{2,5}}
vector<vector<int>> set_product_two(vector<int> vec1,vector<int> vec2)
{
    vector<vector<int>> returning;
    for (int i=0;i<vec1.size();i++)
    {
        for (int j=0;j<vec2.size();j++)
        {
            returning.push_back({vec1[i],vec2[j]});
        }
    }
    return returning;
}

// turn ( {{1,3},{1,4},{2,3},{2,4}} , {5,6} ) into {{1,3,5},{1,3,6},...}
vector<vector<int>> set_product_two(vector<vector<int>> vec1,vector<int> vec2)
{
    vector<vector<int>> result;
    vector<int> vec_temp;
    int i,j;
    for (i=0;i<vec1.size();i++)
    {
        for (j=0;j<vec2.size();j++)
        {
            vec_temp=vec1[i];
            vec_temp.push_back(vec2[j]);
            result.push_back(vec_temp);
            vec_temp.clear();
        }
    }
    return result;
}

// turn {{1,2},{3,4,5},{6,7}} into {{1,3,6},{1,3,7},{1,4,6},...}
vector<vector<int>> set_product_many(vector<vector<int>> vec)
{
    if (vec.size()<1)
    {
        cout<<"error (set_product_many): not enough input."<<endl;
        vector<vector<int>> result={{0}};
        return result;
    }
    else if (vec.size()==1)
    {
        vector<vector<int>> result;
        for (int j=0;j<vec[0].size();j++)
        {
            result.push_back({vec[0][j]});
        }
        return result;
    }
    else
    {
        vector<vector<int>> result=set_product_two(vec[0],vec[1]);
        for (int i=2;i<vec.size();i++)
        {
            result=set_product_two(result,vec[i]);
        }
        return result;
    }
}

class chessboard
{
private:
    
    map<pair<int,int>,int> AllChess;
    map<pair<int,int>,vector<int>> AllMap;
    vector<pair<int,int>> EmptyPositions;
    int solvable;   // -1: initial value / 0: solvable / 1: no solution / 2: no unique solution

public:
    
    void initiate(vector<vector<int>> vec)
    {
        solvable=-1;
        vector<int> vec_initalize={1,2,3,4,5,6,7,8,9};
        pair<int,int> pair_temp;
        int i,j;
        for (i=0;i<boardsize;i++)
        {
            for (j=0;j<boardsize;j++)
            {
                pair_temp={i,j};
                AllChess[pair_temp]=vec[i][j];
                if (vec[i][j]==0)
                {
                    AllMap[pair_temp]=vec_initalize;
                    EmptyPositions.push_back(pair_temp);
                }
                else if ((1<=vec[i][j]) and (vec[i][j]<=boardsize))
                {
                    AllMap[pair_temp].push_back(vec[i][j]);
                }
                else{cout<<"error (chessboard.initiate): input csv is not a legal one."<<endl;}
            }
        }
    }
    
    void initiate(string file_position)
    {
        // read csv file
        vector<vector<int>> AllNums;
        vector<int> vec_temp;
        fstream file;
        file.open(file_position);
        string line;
        string data;
        while (getline(file,line,'\n'))  // read until "\n"
        {
            istringstream templine(line); // string -> stream
            while (getline(templine,data,',')) // read until ","
            {
                vec_temp.push_back(atoi(data.c_str()));
            }  //string -> int
            AllNums.push_back(vec_temp);
            vec_temp.clear();
        }
        file.close();
        initiate(AllNums);
    }
    
    void initiate(chessboard Game0)
    {
        solvable=-1;
        AllChess=Game0.getAllChess();
        AllMap=Game0.getAllMap();
        EmptyPositions=Game0.getEmptyPositions();
    }
    
    map<pair<int,int>,int> getAllChess()
    {
        return AllChess;
    }
    
    map<pair<int,int>,vector<int>> getAllMap()
    {
        return AllMap;
    }
    
    vector<pair<int,int>> getEmptyPositions()
    {
        return EmptyPositions;
    }
    
    int getsolvable()
    {
        return solvable;
    }
    
    void showchess()
    {
        cout<<"empty positions= "<<EmptyPositions.size()<<endl;
        for (int i=0;i<boardsize;i++)
        {
            for (int j=0;j<boardsize;j++)
            {
                cout<<AllChess[{i,j}];
                if (j<boardsize-1)
                {
                    cout<<",";
                }
            }
            cout<<endl;
        }
        cout<<"-------------------------"<<endl;
    }
    
    // return true if there is Integer in position Pair's row/col/block
    bool exist(pair<int,int> Pair,int Integer)
    {
        vector<pair<int,int>>::iterator it_vec;
        vector<pair<int,int>> vec_temp;
        
        vec_temp=same_row(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (AllChess[*it_vec]==Integer)
            {
                return true;
            }
            it_vec++;
        }
        
        vec_temp=same_col(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (AllChess[*it_vec]==Integer)
            {
                return true;
            }
            it_vec++;
        }
        
        vec_temp=same_block(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (AllChess[*it_vec]==Integer)
            {
                return true;
            }
            it_vec++;
        }
        return false;
    }
    
    // reset AllMap
    void scan()
    {
        vector<int> vec_temp;
        vector<int>::iterator it_vec;
        map<pair<int,int>,vector<int>>::iterator it_map=AllMap.begin();
        while (it_map!=AllMap.end())
        {
            if (AllChess[it_map->first]==0)
            {
                it_vec=(it_map->second).begin();
                while (it_vec!=(it_map->second).end())
                {
                    if (!exist(it_map->first,*it_vec))
                    {
                        vec_temp.push_back(*it_vec);
                    }
                    it_vec++;
                }
                it_map->second=vec_temp;
                vec_temp.clear();
            }
            else if (AllChess[it_map->first]>=1 and AllChess[it_map->first]<=boardsize)
            {
                if ((it_map->second).size()==1 and exist(it_map->first,it_map->second[0]))
                {
                    solvable=1;
                }
            }
            else
            {
                cout<<"error (chessboard.scan): invalid AllChess."<<endl;
            }
            it_map++;
        }
    }
    
    // put Integer to Pair
    void move(pair<int,int> Pair,int Integer)
    {
        if (AllChess[Pair]==0 and in_vec(AllMap[Pair],Integer))
        {
            AllChess[Pair]=Integer;
            EmptyPositions=vec_remove(EmptyPositions,Pair);
            scan();
        }
    }
    
    // fill in chess if a position has only one possible move
    void fill_AllChess()
    {
        map< pair<int,int> , vector<int> >::iterator it_map=AllMap.begin();
        while (it_map!=AllMap.end())
        {
            if (AllChess[it_map->first]==0)
            {
                if ((it_map->second).size()==1)
                {
                    move(it_map->first,it_map->second[0]);
                }
            }
            it_map++;
        }
    }
    
    // return true if Integer is a possible value for position Pair and Integer is impossible for position Pair's row/col/block
    bool only(pair<int,int> Pair,int Integer)
    {
        if (!in_vec(AllMap[Pair],Integer))
        {
            cout<<"error (chessboard.only): Integer is not a possible number for Pair."<<endl;
            return false;
        }
        bool result_row=true;
        bool result_col=true;
        bool result_block=true;
        
        vector<pair<int,int>> vec_temp;
        vector<pair<int,int>>::iterator it_vec;
        
        vec_temp=same_row(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (in_vec(AllMap[*(it_vec)],Integer))
            {
                result_row=false;
                break;
            }
            it_vec++;
        }
        
        vec_temp=same_col(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (in_vec(AllMap[*(it_vec)],Integer))
            {
                result_col=false;
                break;
            }
            it_vec++;
        }
        
        vec_temp=same_block(Pair);
        it_vec=vec_temp.begin();
        while (it_vec!=vec_temp.end())
        {
            if (in_vec(AllMap[*(it_vec)],Integer))
            {
                result_block=false;
                break;
            }
            it_vec++;
        }
        return (result_row or result_col or result_block);
    }
    
    // fill in chess on the position which is the only one in its row/col/block that can be filled with an integer
    void fill_AllMap()
    {
        vector<int> vec_temp;
        vector<int>::iterator it_vec;
        map< pair<int,int> , vector<int> >::iterator it_map=AllMap.begin();
        while (it_map!=AllMap.end())
        {
            if (AllChess[it_map->first]==0)
            {
                vec_temp=it_map->second;
                it_vec=vec_temp.begin();
                while (it_vec!=vec_temp.end())
                {
                    if (only(it_map->first,*(it_vec)))
                    {
                        move(it_map->first,*(it_vec));
                    }
                    it_vec++;
                }
            }
            it_map++;
        }
    }
    
    // return the smallest size of value of AllMap
    int smallest_AllMap()
    {
        long result=boardsize;
        map<pair<int,int>,vector<int>>::iterator it_map=AllMap.begin();
        while (it_map!=AllMap.end())
        {
            if (((it_map->second).size()<result) and (AllChess[it_map->first]==0))
            {
                result=(it_map->second).size();
            }
            it_map++;
        }
        return (int) result;
    }
    
    // return number positions with smallest length of values in AllMap as a vector
    vector<pair<int,int>> select_AllMap(int number)
    {
        int smallest=smallest_AllMap();
        vector<pair<int,int>> result;
        map<pair<int,int>,int>::iterator it_map;
        while (result.size()<number and result.size()<EmptyPositions.size())
        {
            it_map=AllChess.begin();
            while (result.size()<number and it_map!=AllChess.end() and result.size()<EmptyPositions.size())
            {
                if (it_map->second==0 and AllMap[it_map->first].size()<=smallest)
                {
                    result.push_back(it_map->first);
                }
                it_map++;
            }
            smallest++;
        }
        return result;
    }
    
    void solve()
    {
        scan();
        long temp_zeros=EmptyPositions.size();
        while (EmptyPositions.size()!=0 and solvable==-1)
        {
            fill_AllChess();
            fill_AllMap();
            if (EmptyPositions.size()==0)
            {
                solvable=0;
                break;
            }
            if (smallest_AllMap()<1)
            {
                solvable=1;
                break;
            }
            if (EmptyPositions.size()==temp_zeros)
            {
                solvable=2;
                break;
            }
            temp_zeros=EmptyPositions.size();
        }
    }
    
    bool operator==(const chessboard& that)
    {
        return (this->AllChess==that.AllChess);
    }
};

// check if Game0 is in vec
bool in_vec(vector<chessboard> vec,chessboard Game0)
{
    int i;
    for (i=0;i<vec.size();i++)
    {
        if (vec[i]==Game0)
        {
            return true;
        }
    }
    return false;
}

vector<chessboard> AllSolutions;
vector<int> AllSolvable;

// store all positions chosen by select_AllMap
vector<pair<int,int>> AllPositionsChosen;

// all tries of positions in AllPositionChosen
vector<vector<int>> AllTries;

void initiate_difficult(int difficulty_num,chessboard Game0,vector<pair<int,int>>* positions,vector<vector<int>>* products)
{
    cout<<"initiate difficult..."<<endl;
    
    // select difficulty_num elements in Game0.AllMap, say {(2,0),(4,1)}, size=difficulty_num
    *positions=Game0.select_AllMap(difficulty_num);
    
    // vec_for_set is the possible numbers that can be filled in Pairs in positions, ex: {{5,9},{1,6}}
    vector<vector<int>> vec_for_set;
    vector<pair<int,int>>::iterator it_vec=positions->begin();
    while (it_vec!=positions->end())
    {
        vec_for_set.push_back(Game0.getAllMap()[*it_vec]);
        it_vec++;
    }
    *products=set_product_many(vec_for_set);
}

int main()
{
    clock_t starting,ending;
    starting=clock();
    chessboard Game;
    Game.initiate(read_file);
    Game.showchess();
    Game.solve();
    if (Game.getsolvable()==0)
    {
        Game.showchess();
    }
    else if (Game.getsolvable()==1)
    {
        cout<<"no solution"<<endl;
    }
    else if (Game.getsolvable()==2)
    {
        initiate_difficult(difficulty, Game, &AllPositionsChosen, &AllTries);
        cout<<"start try and error ("<<AllTries.size()<<" possiblities)..."<<endl;
        
        int solutions=0;
        for (int i=0;i<AllTries.size();i++)
        {
            chessboard Game_temp;
            Game_temp.initiate(Game);
            for (int j=0;j<AllTries[i].size();j++)
            {
                Game_temp.move(AllPositionsChosen[j],AllTries[i][j]);
            }
            Game_temp.solve();
            AllSolvable.push_back(Game_temp.getsolvable());
            if (Game_temp.getsolvable()==0)
            {
                if (!in_vec(AllSolutions,Game_temp))
                {
                    AllSolutions.push_back(Game_temp);
                    solutions++;
                }
            }
        }
        cout<<"(solved, no solution, unsolved)= ("<<count_vec(AllSolvable,0)<<","<<count_vec(AllSolvable,1)<<","<<count_vec(AllSolvable,2)<<")"<<endl;
        if (count_vec(AllSolvable,0)>0 and count_vec(AllSolvable,2)==0)
        {
            cout<<solutions<<" solution(s) in total:"<<endl;
            cout<<"-------------------------"<<endl;
            for (int i=0;i<AllSolutions.size();i++)
            {
                AllSolutions[i].showchess();
            }
        }
        else if (count_vec(AllSolvable,1)==AllSolvable.size())
        {
            cout<<"no solution"<<endl;
        }
        else if (count_vec(AllSolvable,2)>0)
        {
            cout<<"Difficult problem, try bigger difficulty."<<endl;
            if (AllSolutions.size()>0)
            {
                cout<<"These are "<<AllSolutions.size()<<" possible solutions, while there may be more."<<endl;
                cout<<"-------------------------"<<endl;
                for (int i=0;i<AllSolutions.size();i++)
                {
                    AllSolutions[i].showchess();
                }
            }
        }
        else
        {
            cout<<"error: number of solutions incorrect."<<endl;
        }
    }
    else
    {
        cout<<"error: getsolvable incorrect."<<endl;
    }
    
    ending=clock();
    cout<<"Time consumed for solving Sudoku: "<<double(ending-starting)/CLOCKS_PER_SEC<<" (sec)"<<endl;
    
    return 0;
}


