// RP_Greedy.cc 

#include "RP_Greedy.hh"
#include "Random.hh"
#include <algorithm>
#include <numeric>

using namespace std;

unsigned ManhattanDistance (point p1, point p2)
{
    return (abs(int(p1.row - p2.row)) + abs(int(p1.col - p2.col)));
}

point PlaceRandomizedRouter(const RP_Input& in, RP_Output& out)
{
    unsigned r, c;
    point router_to_place, selected;
    unsigned max_degree = 0;

    for (int i = 0; i < 1000000; i++)
    {
        do
        {
            r = Random(0, in.Rows()-1);
            c = Random(0, in.Columns()-1);
            selected.row = r;
            selected.col = c;
        } while (!in.IsDot(r, c) || (in.RouterPrice() + (ManhattanDistance(selected, ClosestBackbonePoint(out, in, selected)) * in.BackbonePrice()) >= out.RemainingBudget()));
        
        // if (out.CellDegree(selected) > max_degree)
        // {
        //     max_degree = out.CellDegree(selected);
        //     router_to_place.row = selected.row;
        //     router_to_place.col = selected.col;
        // }
        // cout << "i: " << i << endl;
    }

    out.InsertRouter(selected.row, selected.col);
    // cout << "Placed router in cell " << out.Router(out.RoutersSize()-1).row << " " << out.Router(out.RoutersSize()-1).col << endl;

    out.RouterCoverage(selected);

    return selected;
}


void ConnectRouterWithBackbone(RP_Output& out, const RP_Input& in, point router)
{
    point temp, selected_bb_cell;
    unsigned new_distance;

    // select the backbone point
    selected_bb_cell = ClosestBackbonePoint(out, in, router);
    // cout << "selectedbbcell: " << selected_bb_cell.row << " " << selected_bb_cell.col << endl;

    unsigned current_distance = ManhattanDistance(selected_bb_cell, router);

    while (current_distance > 0)
    {
        temp = MakeRandomMove(selected_bb_cell);
        new_distance = ManhattanDistance(temp, router);
        if (new_distance < current_distance)
        {
            current_distance = new_distance;
            out.InsertBackboneCell(temp.row, temp.col);
            selected_bb_cell = temp;
        }
    }
}


// move from p to one point of his neighborhood
point MakeRandomMove(point p) 
{  
    point p2;

    unsigned r_diff = Random(-1, 1);
    unsigned c_diff = Random (-1, 1);
    p2.row = p.row + r_diff;
    p2.col = p.col + c_diff;

    return p2;
}



point ClosestBackbonePoint (const RP_Output& out, const RP_Input& in, point router)
{
    unsigned minimum_distance = ManhattanDistance(in.StartBackbone(), router);
    point closest = in.StartBackbone();
    unsigned current_distance;
    

    for (unsigned i = 0; i < out.BackboneSize(); i++)
    {
        current_distance = ManhattanDistance(out.BackboneCell(i), router);
        if (current_distance < minimum_distance)
        {
            closest = out.BackboneCell(i);
            minimum_distance = current_distance;
        }
    }
    
    return closest;
}


// void UpdateCoverage(RP_Output& out, const RP_Input& in, point router)
// {
//     point to_evaluate;
//     vector<point> evaluated;
//     unsigned evaluated_neighbors = 0;
//     int r_move, c_move;

//     // cout << "Potential neighbors: " << out.PotentialNeighbors() << endl;
//     // cout << "Radius: " << in.Radius() << endl;
//     // cout << "Router: " << router.row << " " << router.col << endl;

//     do
//     {
//         r_move = Random(-1*int(in.Radius()), in.Radius());
//         // cout << "r_move: " << r_move << endl;
//         c_move = Random(-1*int(in.Radius()), in.Radius());
//         // cout << "c_move: " << c_move << endl;

//         to_evaluate.row = router.row + r_move;
//         to_evaluate.col = router.col + c_move;

//         // cout << "Evaluating point: " << to_evaluate.row << " " << to_evaluate.col << endl;

//         // FOR 

//         // if is still evaluated, break
//         if (PointIsInVec(to_evaluate, evaluated))
//         {
//             // cout << "The point is in vec, continue" << endl;
//             continue;
//         }
        
//         // else add to evaluated
//         evaluated.push_back(to_evaluate);
//         evaluated_neighbors++;
//         // cout << "Evaluated_neighbors: " << evaluated_neighbors << endl;

//         // if
//         out.Cover(to_evaluate.row, to_evaluate.col);
//             // cout << "Covered: " << to_evaluate.row << " " << to_evaluate.col << endl;
//         // else 
//             // cout << "Not covered: " << to_evaluate.row << " " << to_evaluate.col << endl;

//     } while (evaluated_neighbors != out.PotentialNeighbors());
// }



void GreedyRP(const RP_Input& in, RP_Output& out)
{ 
    unsigned max_routers = in.Budget() / in.RouterPrice();
    int score;
    unsigned tentativi;
    unsigned num_r = 0;
    point router;

    do
    {    
        score = out.ComputeScore();
        if (out.RemainingBudget() >= int(in.RouterPrice()))
        {
            router = PlaceRandomizedRouter(in, out);

            // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;

            ConnectRouterWithBackbone(out, in, router);
            // cout << "New Backbone:" << endl;
            // PrintVec(out.Backbone());
            // // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;
        }
        tentativi++;
    } while(tentativi < 1000000);
    score = out.ComputeScore();

    // cout << "Covered: " << endl;
    // PrintVec(out.Covered());

    cout << out;
    cout << "Final Score: " << score << endl;

    cout << "Remaining budget: " << out.RemainingBudget() << endl;
}


bool PointIsInVec (point pt, vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
        if (pt.row == vec[i].row && pt.col == vec[i].col)
            return 1;

    return 0;
}


void PrintVec (vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
    {
        cout << vec[i].row << " " << vec[i].col;
        if (i != vec.size()-1)
            cout << " -> ";
    }
        
    cout << endl;
}