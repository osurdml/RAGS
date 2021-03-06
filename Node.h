#ifndef NODE_H_
#define NODE_H_

#include <iostream> // std::cout, std::endl
#include <float.h> // DBL_MAX
#include <limits.h> // ULONG_MAX
#include "Vertex.h"
#include "Edge.h"

using std::cout ;
using std::endl ;

typedef unsigned long int ULONG ;

enum nodeType {SOURCE, OTHER} ;

// Node class to maintain path information up to a vertex
// contains parent node, (mu, sigma) of cost-to-come
class Node
{
	public:
		Node(Vertex * vertex):
		  itsVertex(vertex), itsParent(0), itsMeanCost(0.0), itsSigCost(0.0), itsDepth(0),
		  itsHeuristic(0.0), itsMeanCTG(0.0), itsSigCTG(0.0) {}
		
		Node(Vertex * vertex, nodeType n):
		itsVertex(vertex), itsParent(0), itsHeuristic(0.0)
		{
	    switch (n)
	    {
		    case SOURCE:
			    itsMeanCost = 0.0 ;
			    itsSigCost = 0.0 ;
			    itsDepth = 0.0 ;
			    itsMeanCTG = 0.0 ;
			    break ;
		    default:
			    itsMeanCost = DBL_MAX ;
			    itsSigCost = DBL_MAX ;
			    itsDepth = ULONG_MAX ;
			    itsMeanCTG = DBL_MAX ;
			    itsSigCTG = DBL_MAX ;
	    }
    }
    
		Node(Node * parent, Edge * edge):
		itsParent(parent), itsHeuristic(0.0), itsMeanCTG(0.0), itsSigCTG(0.0)
		{
	    itsVertex = edge->GetVertex2() ;
	    itsMeanCost = itsParent->GetMeanCost() + edge->GetMeanSearch() ;
	    itsSigCost = itsParent->GetSigCost() + edge->GetSigSearch() ;
	    itsDepth = itsParent->GetDepth() + 1 ;
    }
    
		~Node(){} ;
		
		Node * GetParent() const {return itsParent ;}
		void SetParent(Node * parent) {itsParent = parent ;}
		double GetMeanCost() const {return itsMeanCost ;}
		void SetMeanCost(double cost) {itsMeanCost = cost ;}
		double GetSigCost() const {return itsSigCost ;}
		void SetSigCost(double sig) {itsSigCost = sig ;}
		Vertex * GetVertex() const {return itsVertex ;}
		void SetVertex(Vertex * vertex) {itsVertex = vertex ;}
		ULONG GetDepth() const {return itsDepth ;}
		void SetDepth(ULONG depth) {itsDepth = depth ;}
		double GetHeuristic() const {return itsHeuristic ;}
		void SetHeuristic(double h) {itsHeuristic = h ;}
		double GetMeanCTG() const {return itsMeanCTG ;}
		void SetMeanCTG(double mCTG){itsMeanCTG = mCTG ;}
		double GetSigCTG() const {return itsSigCTG ;}
		void SetSigCTG(double vCTG){itsSigCTG = vCTG ;}
		
		void DisplayPath() ;
		Node * ReverseList(Node * itsChild) ;
		void SetCTG(double totalMean, double totalSig) ;
    
	private:
		Vertex * itsVertex ;
		Node * itsParent ;
		double itsMeanCost ;
		double itsSigCost ;
		ULONG itsDepth ;
		double itsHeuristic ;
		double itsMeanCTG ;
		double itsSigCTG ;
} ;

void Node::DisplayPath()
{
  cout << "Vertex: (" << itsVertex->GetX() << "," << itsVertex->GetY() << ")\n" ;
  cout << "Mean cost-to-come: " << itsMeanCost << ", " << "standard deviation: " << itsSigCost << endl ;
  cout << "Mean cost-to-go: " << itsMeanCTG << ", " << "standard deviation: " << itsSigCTG << endl ;

  if (itsParent)
    itsParent->DisplayPath() ;
}

Node * Node::ReverseList(Node * itsChild) // Can cause memleak if returned node pointer is not deleted properly
{
  Node * itsParentR = new Node(GetVertex()) ;
  itsParentR->SetMeanCost(GetMeanCost()) ;
  itsParentR->SetSigCost(GetSigCost()) ;
  itsParentR->SetParent(itsChild) ;

  if (GetParent())
  {
    Node * itsReverse = GetParent()->ReverseList(itsParentR) ;
    return itsReverse ;
  }
  else
    return itsParentR ;
}

void Node::SetCTG(double totalMean, double totalSig)
{
  itsMeanCTG = totalMean - itsMeanCost ;
  itsSigCTG = totalSig - itsSigCost ;

  if (itsParent)
    itsParent->SetCTG(totalMean, totalSig) ;
}

#endif // NODE_H_
