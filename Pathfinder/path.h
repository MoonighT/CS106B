/*
 * File: path.h
 * ------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the interface for a Path class that encapsulates
 * the idea of a multistep path.
 */

/////////////////////////////////////////////////////////////////////
// Your task in this part of the assignment is to design a Path    //
// class that supports the implementation of Dijkstra's algorithm. //
// As noted in the assignment handout, your class must meet the    //
// following conditions:                                           //
//                                                                 //
// 1. The class must not export any public instance variables.     //
//    All data required to store the path must be private to the   //
//    class.                                                       //
//                                                                 //
// 2. The method that returns the total cost of the path must      //
//    run in constant time.                                        //
//                                                                 //
// 3. The class must export a toString method that returns a       //
//    string composed of the nodes on the path separated by        //
//    arrows formed by the two-character sequence ->.              //
//                                                                 //
// 4. Any heap storage allocated by this class must be freed       //
//    when the object is deleted.                                  //
//                                                                 //
// Beyond those restrictions, you are free to design and implement //
// this class as you see fit.                                      //
/////////////////////////////////////////////////////////////////////

#ifndef _path_h
#define _path_h

//#include "foreach.h"
#include "graphtypes.h"

class Path {

// [TODO: complete the class definition]
public:
    Path() {
        path_.clear();
        dist_ = 0.0;
    }

    inline void toString(){
        for(int i=0; i < path_.size(); ++i){
            Arc *arc = path_[i];
            cout << arc->start->name << "=>" << arc->finish->name << " ";
        }
        cout << endl;

    }

    void addArc(Arc *arc) {
        path_.push_back(arc);
        dist_ += arc->cost;
    }

    void removeBackArc() {
        if(path_.size() > 0){
            dist_ -= path_[path_.size() - 1]->cost;
            path_.remove(path_.size() - 1);
        }
    }

    void clear() {
        dist_ = 0;
        path_.clear();
    }

    Node* getFront() {
        return path_[path_.size() - 1]->finish;
    }

    double dist_;
private:
    Vector<Arc *> path_;
};

#endif
