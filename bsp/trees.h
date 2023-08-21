#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include "structinc.h"

class trees {
private:
	struct treenode {
		Walls ssec;
		treenode* front, * back;

		treenode(Walls w) : ssec(w), front(NULL), back(NULL) {};
	};


public:
	/* standard bsp traversal I stole this from wikipedia, but the compariosns 
	* are flipped so that the accumulated vector of walls
	* from the traversal are flipped so walls can be drawn back to front
	*/
	void traverse(trees::treenode* root, std::pair<float, float> v, std::vector<Walls>& maptree);

	/* int func to compare if lines are infront or behind and if
	* intersection split line at the intersection and add as walls
	* 0 front 1 back 2 intersection first __ second
	*/
	static int WallComp(Walls, Walls);

	/* function to handle finding the intersection point and
	* splitting the wall. after split send front wall to a front wall vector
	* and the back wall to a backwall vector
	* these are function variables that we take as parameters
	*/
	inline void intersectionHandle(Walls curwall, Walls dividingwall, std::vector<Walls>& frontwall, std::vector<Walls>& backwall);

	/* so we create a recurser to go down the wall points
	* create a vector along that direction and cycle through
	* the points
	* intersection means split
	* for front and behind we call dist to midpoint and see
	* infront or behind
	* build tree
	* recurse
	*/
	treenode* rec(std::vector<Walls> walls);

	// helper
	static void printwalls(Walls a);

	trees(std::vector<Walls> walls);

	/* helper function that calls the recursive traversal on the root
	*/
	void print(std::pair<float, float> v, std::vector<Walls>& maptree);

	void printBT(const treenode* node);
	void printBT_();
private:
	treenode* root;
	std::vector<Walls> map;

};
