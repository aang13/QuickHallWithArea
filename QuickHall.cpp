#include <bits/stdc++.h>
using namespace std;

//Defining POINT Structure
typedef struct POINT
{
    double x,y;
} Point;

//Global Variables
vector <Point> vector_of_extreme_points;
double area;
double perimeter;

//Some helping Functions
double distance_from_p1p2(Point p1, Point p2, Point outer_p)
{
    /*
        Finds the distance between p and the line joining p1,p2
         x - x1      y - y1
        -------- = ----------  => (x - x1)*(y1-y2) - (y-y1)*(x1-x2) = 0
         x1 - x2    y1 - y2
          Ax+By+C
         ---------- , in Convex Hull Problem A,B remains same. So distance is proportional to |Ax+By+C|, where (x,y) is the outer Point for test.
         A^2 + B^2
    */
    // Computes |Ax+Bx+C|
    return fabs( (p1.y - p2.y) * (outer_p.x - p1.x) - (outer_p.y - p1.y)*(p1.x - p2.x) );
}


bool is_strictly_right(Point a,Point b,Point p)
{
    /*
        Checks the sign of the Determinant forming the triangle A,P,B
        If Area(A,P,B) is positive, then P is strictly right of AB
        | a.x a.y 1 |
        | p.x p.y 1 |
        | b.x b.y 1 |
    */

    return a.x*(p.y - b.y)- p.x*(a.y - b.y) + b.x*(a.y - p.y) > 0 ;
}


double find_area_times_two(Point a, Point b, Point c)
{
    /*
        | a.x a.y 1 |
        | b.x b.y 1 |
        | c.x c.y 1 |
    */
    return fabs(a.x*(b.y - c.y)- b.x*(a.y - c.y) + c.x*(a.y - b.y));
}


void find_hull(Point a, Point b, vector <Point> &vector_s)
{
    bool debug = false;

    if (vector_s.empty())
    {
        perimeter+= sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
        return;
    }

    int vector_size = vector_s.size();

    Point max_distant_point_c;
    vector <Point> right_of_ac;
    vector <Point> right_of_cb;

    //Searching the Point which is the most distant from AB
    double dis, max_dis = -1;
    int index_of_max_dist_point;

    double min_x; //Necessary For Finding Extreme Points.

    for (int i = 0; i<vector_size; i++)
    {
        dis = distance_from_p1p2(a,b, vector_s[i]);
        if (dis - max_dis > 0.0001)
        {
            max_dis = dis;
            index_of_max_dist_point = i;
            min_x = vector_s[i].x;
        }
        else if (dis == max_dis && vector_s[i].x - min_x < 0.0001 ) //Newly Added for Finding Extreme Points Only, Necessary
        {
            index_of_max_dist_point = i;
            min_x = vector_s[i].x;
        }
    }

    max_distant_point_c = vector_s[index_of_max_dist_point];
    area += find_area_times_two(a,b,max_distant_point_c);

    if (debug)
        cout<<"Max distant Point from line joining p1("<<a.x<<","<<a.y<<") and p2("<<b.x<<","<<b.y<<") is ("<<max_distant_point_c.x<<","<<max_distant_point_c.y<<")"<<endl;


    //Building the set of points strictly right of AC
    //Building the set of points strictly right of CB
    for (int i=0; i<vector_size; i++)
    {
        if (i == index_of_max_dist_point)
            continue;

        if (is_strictly_right(a, max_distant_point_c, vector_s[i]))
            right_of_ac.push_back(vector_s[i]);

        else if (is_strictly_right(max_distant_point_c, b, vector_s[i]))
            right_of_cb.push_back(vector_s[i]);
    }



    if (debug)
    {
        cout<<"Printing all points strictly right of AC:"<<endl;
        for (int i = 0; i<right_of_ac.size(); i++)
            cout<<"("<<right_of_ac[i].x<<","<<right_of_ac[i].y<<") ";

        cout<<"\nPrinting all points strictly right of CB:"<<endl;
        for (int i = 0; i<right_of_cb.size(); i++)
            cout<<"("<<right_of_cb[i].x<<","<<right_of_cb[i].y<<") ";
        cout<<endl;
    }

    if (debug)
        cout<<"Finding hull of the line a("<<a.x<<","<<a.y<<") and c("<<max_distant_point_c.x<<","<<max_distant_point_c.y<<")"<<endl;

    find_hull(a, max_distant_point_c, right_of_ac);

    vector_of_extreme_points.push_back(max_distant_point_c);

    if (debug)
        cout<<"Finding hull of the line c("<<max_distant_point_c.x<<","<<max_distant_point_c.y<<") and b("<<a.x<<","<<a.y<<")"<<endl;

    find_hull(max_distant_point_c, b,right_of_cb);

}



void quick_hull(int ind_of_leftmost_bottomP, int ind_of_rightmost_topP, vector<Point> &vector_of_given_points)
{
    int total_points = vector_of_given_points.size();

    Point a = vector_of_given_points[ind_of_leftmost_bottomP];
    Point b = vector_of_given_points[ind_of_rightmost_topP];

    vector <Point> vector_right_ab;
    vector <Point> vector_right_ba;


    for (int i=0; i<total_points; i++)
    {
        if (i == ind_of_leftmost_bottomP || i == ind_of_rightmost_topP)
            continue;

        if (is_strictly_right(a,b,vector_of_given_points[i]))
            vector_right_ab.push_back(vector_of_given_points[i]);

        else if (is_strictly_right(b,a,vector_of_given_points[i]))
            vector_right_ba.push_back(vector_of_given_points[i]);
    }


    vector_of_extreme_points.push_back(a);

    find_hull( a, b, vector_right_ab );

    vector_of_extreme_points.push_back(b);

    find_hull( b, a, vector_right_ba );

}


int main()
{
    //freopen("new_input_1.txt","r",stdin);
    //freopen("1405073_output_analysis.txt","w",stdout);

    bool debug_in_main = false;

    //Prepares input for Graphic Display in OpenGL
    bool gl_input = true;


    int total_points, ind_of_leftmost_bottomP, ind_of_rightmost_topP;
    double min_x_coordinate, max_x_coordinate;

    //cout<< "Enter number of total points." <<endl;
    cin>>total_points;
    vector <Point> vector_of_given_points(total_points);

    //cout<< "Enter x y pairs in each line. " <<endl;
    for (int i=0; i<total_points; i++)
    {
        cin>>vector_of_given_points[i].x>>vector_of_given_points[i].y;

        if (i)
        {
            if (min_x_coordinate - vector_of_given_points[i].x > 0.0001 ||
                    ( min_x_coordinate == vector_of_given_points[i].x &&  vector_of_given_points[i].y-vector_of_given_points[ind_of_leftmost_bottomP].y < 0.00001)
               )
            {
                min_x_coordinate = vector_of_given_points[i].x;
                ind_of_leftmost_bottomP = i;
            }

            if (max_x_coordinate - vector_of_given_points[i].x < 0.0001 ||
                    ( max_x_coordinate == vector_of_given_points[i].x &&  vector_of_given_points[i].y-vector_of_given_points[ind_of_rightmost_topP].y > 0.00001)
               )
            {
                max_x_coordinate = vector_of_given_points[i].x;
                ind_of_rightmost_topP = i;
            }
        }
        else
        {
            ind_of_leftmost_bottomP = ind_of_rightmost_topP = 0;
            min_x_coordinate = max_x_coordinate = vector_of_given_points[0].x;
        }


    }


    //Check leftmost_bottom and right_most_top
    if (debug_in_main)
    {
        cout<<"Leftmost Bottom: "<<vector_of_given_points[ind_of_leftmost_bottomP].x<<" "<<vector_of_given_points[ind_of_leftmost_bottomP].y<<endl;
        cout<<"Rightmost Top: "<<vector_of_given_points[ind_of_rightmost_topP].x<<" "<<vector_of_given_points[ind_of_rightmost_topP].y<<endl;
    }


    quick_hull(ind_of_leftmost_bottomP,ind_of_rightmost_topP,vector_of_given_points);


    cout<<vector_of_extreme_points.size()<<endl;
    for (vector<Point> :: iterator it_for_extreme_points = vector_of_extreme_points.begin(); it_for_extreme_points!=vector_of_extreme_points.end(); it_for_extreme_points++)
    {
        cout<<(*it_for_extreme_points).x<<" "<<(*it_for_extreme_points).y<<endl;
    }

    cout<<"\nArea: "<<area/2<<endl;
    cout<<"Perimeter: "<<perimeter<<endl;

    /*
        Prepares Input for Graphics Output
    */
    if (gl_input)
    {
        ofstream make_gl_file("1405073_gl_input.txt");

        make_gl_file<<total_points<<endl;
        for (int i=0; i<total_points; i++)
            make_gl_file<<vector_of_given_points[i].x<<" "<<vector_of_given_points[i].y<<endl;

        make_gl_file<<vector_of_extreme_points.size()<<endl;
        for (vector<Point> :: iterator it_for_extreme_points = vector_of_extreme_points.begin(); it_for_extreme_points!=vector_of_extreme_points.end(); it_for_extreme_points++)
        {
            make_gl_file<<(*it_for_extreme_points).x<<" "<<(*it_for_extreme_points).y<<endl;
        }
        make_gl_file.close();

        system("opengl_executable.exe");
    }



    return 0;
}
