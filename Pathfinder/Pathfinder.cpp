/*
 * File: Pathfinder.cpp
 * --------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Pathfinder application
 * for Assignment #5.
 */
 
// [TODO: complete this implementation]

#define DEBUG

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "path.h"
#include "pqueue.h"
#include "foreach.h"
#include "console.h"
#include "filelib.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gpathfinder.h"
#include "graphtypes.h"
#include "gwindow.h"
using namespace std;

/* Constants */

const string DEFAULT_MAP = "USA.txt";
const string DEFAULT_IMAGE = "USA.png";
const string QUIT = "Quit";
const string MAP = "Map";
const string DIJKSTRA = "Dijkstra";
const string KRUSKAL = "Kruskal";
/* Function prototypes */

void eventLoop(const PathfinderGraph& graph);

/* Starter version of the main program */

void readFile(PathfinderGraph &graph)
{
    string fileName, line;
    ifstream infile;
    fileName = promptUserForFile(infile,"Enter the filename: ");
    //First line is bg name
    getline(infile, line);
    drawPathfinderMap(line);
    getline(infile, line);
    if(line != "NODES")
    {
        cout << "Error" << endl;
        return;
    }
    while( getline(infile, line) && line != "ARCS")
    {
        //reading NODES
        istringstream iss(line);
        string name;
        int x,y;
        iss >> name >> x >> y;
        Node *n = new Node();
        n->name = name;
        n->coor = GPoint(x, y);
        graph.addNode(n);
    }

    //read all the arcs
    while (getline(infile, line))
    {
        istringstream iss(line);
        string nameA, nameB;
        int dist;
        iss >> nameA >> nameB >> dist;


        Arc *arc = new Arc(graph.getNode(nameA),
                           graph.getNode(nameB),
                           dist);
        Arc *arcR = new Arc(graph.getNode(nameB),
                            graph.getNode(nameA),
                            dist);
        graph.addArc(arc);
        graph.getNode(nameA)->arcs.add(arc);
        graph.addArc(arcR);
        graph.getNode(nameB)->arcs.add(arcR);
    }

    //for test
#ifdef DEBUG
    Set<Node*> s = graph.getNodeSet();
    for(auto i = s.begin(); i!=s.end(); i++)
    {
        cout << (*i)->name << endl;
        for(auto j = (*i)->arcs.begin(); j!= (*i)->arcs.end(); j++)
            cout << (*j)->finish->name << endl;

        cout << "=============" << endl;
    }
#endif
}

void displayGraph(const PathfinderGraph& graph)
{
    //draw all the nodes
    Set<Node*> s = graph.getNodeSet();
    for(auto i = s.begin(); i!= s.end(); ++i)
    {
        drawPathfinderNode((*i)->coor, "gray", (*i)->name);
    }
    //draw all the arcs
    Set<Arc*> a = graph.getArcSet();
    for(auto i = a.begin(); i!=a.end(); ++i)
    {
        //little problem   the line will draw twic
        drawPathfinderArc((*i)->start->coor, (*i)->finish->coor, "gray");
    }
}

bool isSamePoint(const GPoint& a, const GPoint& b)
{
    double sum2 = (a.getX()-b.getX())*(a.getX()-b.getX()) + (a.getY()-b.getY())*(a.getY()-b.getY());
    return sum2 <= 100;
}

bool getNodeByCoor(const PathfinderGraph &graph ,const GPoint& point, string& name)
{
    cout << "x=" << point.getX() << "y=" << point.getY() << endl;
    Set<Node*> s = graph.getNodeSet();
    for(auto i = s.begin(); i!= s.end(); ++i)
    {
        if(isSamePoint((*i)->coor, point))
        {
            cout << "same point" << (*i)->name << endl;
            name = (*i)->name;
            return true;
        }
    }
    return false;
}

double getPathCost(const Vector<Arc *> &path) {
    double cost = 0;
    foreach (Arc* arc in path){
        cost += arc->cost;
    }
    return cost;
}

Path dijkstra2(const PathfinderGraph &graph, Node *start, Node *finish) {
    cout << "dijkstra2 " << "start dijkstra... From " << start->name << " To " << finish->name  << endl;
    Path p;
    PriorityQueue< Path > queue;
    Map<string, double> fixed;

    while (start != finish) {
        if(!fixed.containsKey(start->name)) {
            fixed.put(start->name, p.dist_);
            foreach(Arc* arc in start->arcs){
                if(!fixed.containsKey(arc->finish->name)) {
                    p.addArc(arc);
                    queue.enqueue(p, p.dist_);
                    p.removeBackArc();
                }
            }
        }
        if (queue.isEmpty()) {
            p.clear();
            return p;
        }
        p = queue.dequeue();
        start = p.getFront();
    }
    return p;
}

Vector<Arc *> dijkstra(const PathfinderGraph &graph, Node* start, Node* finish)
{
    cout << "start dijkstra... From " << start->name << " To " << finish->name  << endl;
    Vector<Arc *> path;
    PriorityQueue< Vector<Arc *> > queue;
    Map<string, double> fixed;
    while (start != finish) {
        if(!fixed.containsKey(start->name)) {
            fixed.put(start->name, getPathCost(path));
            foreach(Arc* arc in start->arcs){
                if(!fixed.containsKey(arc->finish->name)) {
                    path.add(arc);
                    queue.enqueue(path, getPathCost(path));
                    path.remove(path.size() - 1);
                }
            }
        }
        if (queue.isEmpty()) {
            path.clear();
            return path;
        }
        path = queue.dequeue();
        start = path[path.size() - 1]->finish;
    }
    return path;
}

void drawPath(const Vector<Arc *> &path){
    foreach(Arc *arc in path){
        drawPathfinderArc(arc->start->coor, arc->finish->coor, "red");
    }
}

int main() {
   GWindow gw(WINDOW_WIDTH, WINDOW_HEIGHT);
   initPathfinderGraphics(gw);
   gw.addToRegion(new GButton(QUIT), "SOUTH");
   gw.addToRegion(new GButton("Map"), "SOUTH");
   gw.addToRegion(new GButton("Dijkstra"), "SOUTH");
   gw.addToRegion(new GButton("Kruskal"), "SOUTH");

   //drawPathfinderMap(DEFAULT_IMAGE);
   //ask user input file name
   //read all nodes and arcs
   PathfinderGraph myGraph;
   readFile(myGraph);
   eventLoop(myGraph);
   return 0;
}

/*
 * Function: eventLoop
 * Usage: eventLoop();
 * -------------------
 * Waits for events to occur and then executes the appopriate action.
 * Note that this function never returns.  The program exits only when
 * a command executes the exitGraphics function.
 */

void eventLoop(const PathfinderGraph &graph) {
   while (true) {
      GActionEvent e = waitForEvent(ACTION_EVENT);
      string cmd = e.getActionCommand();
      if (cmd == "Quit")
          exitGraphics();
      else if (cmd == "Map")
          displayGraph(graph);
      else if (cmd == "Dijkstra")
      {
          cout << "Please click two node as start and finish" << endl;
          string start, finish;
          bool flag = false;
          do{
              GPoint startP = getMouseClick();
              flag = getNodeByCoor(graph,startP, start);
              if (flag == true)
                  break;
              cout << "cannot get the Node enter again: " << endl;
          }while(true);



          cout << "get the start city is " << start << endl;

          GPoint finishP = getMouseClick();
          do{
              flag = getNodeByCoor(graph,finishP, finish);
              if(flag == true)
                      break;
              cout << "cannot get the Node enter again: " << endl;
          }while(true);

          cout << "get the finish city is " << finish << endl;
          Path path = dijkstra2(graph, graph.getNode(start), graph.getNode(finish));
          path.toString();
          //drawPath(path);
      }
      else if (cmd == "Kruskal")
          continue;
   }
}
