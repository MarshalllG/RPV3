//  Router_placement_data.hpp

#ifndef Router_placement_data_hh
#define Router_placement_data_hh

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct point
{
    int row, col; 
};

class RP_Input
{
    friend ostream& operator<<(ostream& os, const RP_Input& in);
public:
    RP_Input (string file_name);
    vector<vector<char> > Cells() const { return cells; }
    char Cell(unsigned r, unsigned c) const {return cells[r][c]; }
    unsigned Rows() const { return rows; }
    unsigned Columns() const { return columns; }
    unsigned Radius() const { return radius; }
    unsigned BackbonePrice() const { return b_price; }
    unsigned RouterPrice() const { return r_price; }
    unsigned Budget() const { return budget; }
    point StartBackbone() const { return start_backbone; }
    bool IsWall (int r, int c) const { return cells[r][c] == '#'; }
    bool IsDot (int r, int c) const { return cells[r][c] == '.'; }
    bool IsLine (int r, int c) const { return cells[r][c] == '-'; }
    // unsigned Target(vector<vector<char> > cells, unsigned rows, unsigned columns) const; // numero
private:
    /* vector<vector<int> > covers; */
    vector<vector<char> > cells;
    unsigned rows, columns; // dimensioni esterne
    unsigned radius; // of router range
    unsigned b_price; // of connecting one single cell to the backbone
    unsigned r_price; // of one wireless router
    unsigned budget; // maximum budget
    point start_backbone; // coordinates
    // unsigned target;
};

class RP_Output
{
    // SOVRACCARICO OPERATORI []
    friend ostream& operator<<(ostream& os, const RP_Output& out);
    // friend istream& operator>>(istream& is, BDS_Output& out);
public:
    RP_Output(const RP_Input& in);
    int RemainingBudget() const { return remaining_budget; }
    unsigned Rows() const { return in.Rows(); }
    unsigned Columns() const { return in.Columns(); }
    unsigned NumCovered() const { return covered.size(); }
    unsigned NumUncovered() const { return uncovered.size(); }
    unsigned BackboneSize() const { return backbone.size(); }
    unsigned RoutersSize() const { return routers.size(); }
    // unsigned PotentialNeighbors() const { return potential_neighbors; }
    unsigned FindRouterCoverageArea (const RP_Input& in, point router) const;
    unsigned CellDegree(point pt);
    bool IsInBackbone (point pt) const;
    bool IsInCovered (point pt) const;
    int Degree(int r, int c) const { return degree[r][c]; }
    vector<point> Backbone() const { return backbone; }
    vector<point> Routers() const { return routers; }
    vector<point> Covered() const { return covered; }
    point BackboneCell(unsigned i) const { return backbone[i]; }
    point Router(unsigned i) const { return routers[i]; }
    void InsertRouter(unsigned r, unsigned c);
    void InsertBackboneCell(unsigned r, unsigned c);
    void RouterCoverage(point router);
    int ComputeScore() const { return 1000*NumCovered()+(in.Budget()-(backbone.size()*in.BackbonePrice()+routers.size()*in.RouterPrice())); }
    // bool Covers(point router, unsigned r, unsigned c) const;
    // RemoveRouter
    // RemoveBackbone
    // unsigned Target(vector<vector<char> > cells, unsigned rows, unsigned columns) const; // numero
private:
    // Degree (MURI, ROUTER, Anelli)
    const RP_Input& in;
    vector<vector<char> > out_cells;
    vector<vector<int> > degree;
    vector<point> routers, backbone;
    vector<point> covered, uncovered;
    int remaining_budget;
    // unsigned potential_neighbors;
};


#endif /* Router_placement_data_hh */


