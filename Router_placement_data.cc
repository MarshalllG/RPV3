//  Router_placement_data.cpp

#include "Router_placement_data.hh"


RP_Input::RP_Input(string file_name)
{  
    ifstream is(file_name);
    if (!is) 
    {
        cerr << "Cannot open file " <<  file_name << endl;
	    exit(1);
    }

    is >> rows >> columns >> radius
       >> b_price >> r_price >> budget
       >> start_backbone.row >> start_backbone.col;

    cells.resize(rows, vector<char>(columns));

    for (unsigned r = 0; r < rows; r++)
    {
        for (unsigned c = 0; c < columns; c++)
        {
            char ch;
            is >> ch;
            cells[r][c] = ch;
        }
    }
}


// unsigned RP_Input::Target(vector<vector<char> > cells, unsigned rows, unsigned columns) const // numero
// {
//     unsigned count = 0;

//     for (unsigned r = 0; r < rows; r++)
//     {
//         for (unsigned c = 0; c < columns; c++)
//         {
//             if (cells[r][c] == '.')
//                 count++;
//         }
//     }
//     return count;
// }


ostream& operator<<(ostream& os, const RP_Input& in)
{
    for (unsigned r = 0; r < in.rows; r++)
    {
        for (unsigned c = 0; c < in.columns; c++)
        {
            os << in.cells[r][c];
        }
        cout << endl;
    }
    return os;
}


RP_Output::RP_Output(const RP_Input& my_in) : in(my_in) // ?????
{
    out_cells = in.Cells(); // for for
    backbone.push_back(in.StartBackbone()); 
    out_cells[in.StartBackbone().row][in.StartBackbone().col] = 'b';
    remaining_budget = in.Budget()-in.BackbonePrice();
    // potential_neighbors = pow((2*in.Radius()+1), 2);
    // total_score = 0;
    // point temp;

    // for (int r = 0; r < in.Rows(); r++)
    // {
    //     cout << "r: " << r << endl;

    //     for (int c = 0; c < in.Columns(); c++)
    //     {
    //         cout << "c: " << c << endl;

    //         if (!in.IsWall(r,c))
    //         {
    //             temp.row = r;
    //             temp.col = c;
    //             cout << "ciao" << endl; 

    //             degree[r][c] = FindRouterCoverageArea(in, temp);
    //             cout << "ciao" << endl; 

    //         }
    //     }
    // }
}


void RP_Output::InsertRouter(unsigned r, unsigned c)
{
    point router;

    if (!in.IsWall(r, c))
    {
        // update cell
        out_cells[r][c] = 'r';

        router.row = r;
        router.col = c;
    
        routers.push_back(router);

        // update budget
        remaining_budget -= in.RouterPrice();
    }
}


void RP_Output::InsertBackboneCell(unsigned r, unsigned c)
{
    point bb_cell;
    bb_cell.row = r;
    bb_cell.col = c;

    if (!IsInBackbone(bb_cell))
        backbone.push_back(bb_cell);

    remaining_budget -= in.BackbonePrice();
}


void RP_Output::RouterCoverage(point router)
{
    point temp;
    unsigned new_radius;
        
    new_radius = FindRouterCoverageArea(in, router);

    for (int r=router.row-new_radius; r <= router.row+new_radius; r++)
    {
        for (int c=router.col-new_radius; c <= router.col+new_radius; c++)
        {
            temp.row = r;
            temp.col = c;
            if (in.IsDot(r,c) && !IsInCovered(temp))
            {
                covered.push_back(temp);
                if (out_cells[temp.row][temp.col] != 'r')
                    out_cells[r][c] = '1';
            }
        }
    }
}

unsigned RP_Output::CellDegree(point pt)
{
    point temp;
    unsigned new_radius;
    unsigned count = 0;
        
    new_radius = FindRouterCoverageArea(in, pt);

    for (int r=pt.row-new_radius; r <= pt.row+new_radius; r++)
    {
        for (int c=pt.col-new_radius; c <= pt.col+new_radius; c++)
        {
            temp.row = r;
            temp.col = c;
            if (out_cells[r][c] == '.' && !IsInCovered(temp))
            {
                count++;
            }
        }
    }
    return count;
}

// int RP_Output::CellDegree(point pt) 
// {
//     int count = 0;
//     unsigned new_radius;
        
//     new_radius = FindRouterCoverageArea(in, pt);
//     cout << "new radius: " << new_radius << endl;

//     for (int r=pt.row-new_radius; r <= pt.row+new_radius; r++)
//     {
//         for (int c=pt.col-new_radius; c <= pt.col+new_radius; c++)
//         {
//             cout << "r: " << r << endl;
//             cout << "c: " << c << endl;

//             if (out_cells[r][c] == '.')
//             {
//                 cout << "count: " << count << endl;
//                 count++;
//             }
//         }
//     }
//     return count;
// }

unsigned RP_Output::FindRouterCoverageArea (const RP_Input& in, point router) const
{
    unsigned step = 1;

    do
    {
        for (int r=router.row-step; r <= router.row+step; r++)
        {
            for (int c=router.col-step; c <= router.col+step; c++)
            {
                if (r >= 0 && r < in.Rows() && c >= 0 && c < in.Columns())
                {
                    if (in.IsWall(r,c))
                        return step-1;
                }
                else
                    return step-1;
            }
        }
        step++;
    } while (step <= in.Radius());

    return in.Radius();
}

// void RP_Output::RemoveRouter(point router)
// {
//     out_cells[router.row][router.col] = in.cells[router.row][router.col];
//     routers.delete()
//     out.remaining_budget += in.RouterPrice();
// }

 
// void RP_Output::RemoveBackboneCell (point bb_cell)
// {
//     backbone.delete()
//     out.remaining_budget += in.BackbonePrice();
// }


ostream& operator<<(ostream& os, const RP_Output& out)
{
    for (unsigned r = 0; r < out.Rows(); r++)
    {
        for (unsigned c = 0; c < out.Columns(); c++)
        {
            os << out.out_cells[r][c];
        }
        cout << endl;
    }
    return os;
}


bool RP_Output::IsInBackbone (point pt) const
{
    for (unsigned i = 0; i < backbone.size(); i++)
        if (pt.row == backbone[i].row && pt.col == backbone[i].col)
            return 1;

    return 0;
}


bool RP_Output::IsInCovered (point pt) const
{
    for (unsigned i = 0; i < covered.size(); i++)
        if (pt.row == covered[i].row && pt.col == covered[i].col)
            return 1;

    return 0;
}


