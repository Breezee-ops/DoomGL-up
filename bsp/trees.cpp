#include "trees.h"

void trees::populateNormals(std::vector<Walls>& w) {
	for (auto& wall : w) {
		int dx = wall.p2.first - wall.p1.first;
		int dy = wall.p2.second - wall.p1.second;
		wall.normal = { -dy, dx };
	}
}

inline float dot(std::pair<float, float>a, std::pair<float, float>b) {
	return ((a.first* b.first) + (a.second * b.second));
}

inline float cross_z (std::pair<float, float>a, std::pair<float, float>b, std::pair<float, float>m) {
	float zcomp = ((b.first - a.first) * (m.second - a.second) - (b.second - a.second) * (m.first - a.first));
	return zcomp;
}

void trees::printwalls(Walls a) {
	std::cout << "(" << a.p1.first << "," << a.p1.second << "), (" << a.p2.first << "," << a.p2.second << ")" << std::endl;
}
// 0 front 1 back 2 intersection first __ second
int trees::WallComp(Walls first, Walls second) {

	//// make the first line a vector, find if the two points on the second line cross product to have signs
	//std::pair<float, float> vec1 = { first.p1.first - first.p2.first, first.p1.second - first.p2.second };
	//std::pair<float, float> vec21 = { first.p1.first - second.p2.first, first.p1.second - second.p2.second };
	//std::pair<float, float> vec22 = { first.p1.first - second.p1.first, first.p1.second - second.p1.second };
	//std::cout << "z1: " << cross_z(vec1, second.p1) << "z2: " << cross_z(vec1, second.p2) << std::endl;
	if (cross_z(first.p1, first.p2, second.p1) >= 0 && cross_z(first.p1, first.p2, second.p2) >= 0) {
		return 1;
	}
	if (cross_z(first.p1, first.p2, second.p1) < 0 && cross_z(first.p1, first.p2, second.p2) < 0) {
		return 0;
	}
	return 2;
}

inline void trees::intersectionHandle(Walls curwall, Walls dividingwall, std::vector<Walls>& frontwall, std::vector<Walls>& backwall) {
	std::cout << "intersecting walls: ";
	printwalls(curwall);
	printwalls(dividingwall);

	// visualising using desmos is goated
	std::pair<float, float> inter;

	// these first couple of if statements are no good
	
	float a1 = (curwall.p2.second - curwall.p1.second) / (curwall.p2.first - curwall.p1.first);
	float a2 = (dividingwall.p2.second - dividingwall.p1.second) / (dividingwall.p2.first - dividingwall.p1.first);
	float b1 = curwall.p1.second - (a1 * curwall.p1.first);
	float b2 = dividingwall.p1.second - (a2 * dividingwall.p1.first);
	if (curwall.p2.first - curwall.p1.first == 0) {
		inter = { curwall.p1.first, a2 * curwall.p1.first + b2 };
	}
	else if(dividingwall.p2.first - dividingwall.p1.first == 0){
		inter = { dividingwall.p1.first, a1 * dividingwall.p1.first + b1 };
	}
	else{
		float x = (b2 - b1) / (a1 - a2);

		inter = { x,(a1 * x + b1) };
	}


	Walls w1(dividingwall);
	Walls w2(dividingwall);
	w1.p2 = inter;
	w2.p1 = inter;

	if (WallComp(w1, dividingwall) == 1 && WallComp(w2, dividingwall) == 0) {
		
		frontwall.push_back(w1);
		backwall.push_back(w2);
	}
	else if (WallComp(w1, dividingwall) == 0 && WallComp(w2, dividingwall) == 1) {
		
		frontwall.push_back(w2);
		backwall.push_back(w1);
	}
	else if (WallComp(w1, dividingwall) == 1 && WallComp(w2, dividingwall) == 1) {
		
		backwall.push_back(w2);
		backwall.push_back(w1);
	}
	else if (WallComp(w1, dividingwall) == 0 && WallComp(w2, dividingwall) == 0) {
		std::cout << "case hit" << std::endl;
		frontwall.push_back(w2);
		frontwall.push_back(w1);
	}

	else {
		std::cout << "curwall: ";
		printwalls(curwall);
		std::cout << "div: ";
		printwalls(dividingwall);
		std::cout << "houston, we have a problem" << std::endl;
	}
}


trees::treenode* trees::rec(std::vector<Walls> walls) {
	std::cout << "why is this being called? " << std::endl;
	if (walls.empty()) {  return nullptr; }
	Walls dividingwall = walls[0];
	std::vector<Walls> frontwalls, backwalls;

	if (walls.size() == 1) {
		treenode* curroot1 = new treenode(dividingwall);
		return curroot1;
	}
	for (int i = 1; i < walls.size(); i++) {
		if (WallComp(dividingwall, walls[i]) == 1) frontwalls.push_back(walls[i]);
		if (WallComp(dividingwall, walls[i]) == 0) backwalls.push_back(walls[i]);
		if (WallComp(dividingwall, walls[i]) == 2) {
			intersectionHandle(dividingwall, walls[i], frontwalls, backwalls);
		}
	}
	treenode* curroot = new treenode(dividingwall);
	curroot->front = rec(frontwalls);
	curroot->back = rec(backwalls);
	return curroot;
}

trees::trees(std::vector<Walls> walls) {
	map = walls;
	root = rec(walls);
}

//int pointdep(int x1, int x2, int y1, int y2) {
//	// calculates depth of middle point to player (origin)
//	int midx = (x1 + x2) / 2;
//	int midy = (y1 + y2) / 2;
//
//	return (midx * midx) + (midy * midy);
//}

void trees::traverse(trees::treenode* root, std::pair<float, float> v, std::vector<Walls>& maptree) {

	// inorder
	//if (root == NULL) return;

	//if(root->front != nullptr)traverse(root->front);
	//std::cout << root->ssec.p1.first << ", " << root->ssec.p1.second << std::endl;
	//std::cout << root->ssec.p2.first << ", " << root->ssec.p2.second << std::endl;
	//if (root->back != nullptr)traverse(root->back);
	// 
	// 
	// actual
	if (root->front == nullptr && root->back == nullptr) {
		maptree.push_back(root->ssec);
		//std::cout << "(" << root->ssec.p1.first << "," << root->ssec.p1.second << "), (" << root->ssec.p2.first << "," << root->ssec.p2.second << ")" << std::endl;
		return;
	}
	//float distf = FLT_MAX;
	//float distb = FLT_MAX;
	//if (root->front != nullptr) {
	//	std::pair<float, float> mf = { (root->front->ssec.p2.first - root->front->ssec.p1.first) / 2, (root->front->ssec.p2.second - root->front->ssec.p1.second) / 2 };
	//	distf = (mf.first - v.first) * (mf.first - v.first) + (mf.second - v.second) * (mf.second - v.second);
	//}
	//if (root->back != nullptr) {
	//	std::pair<float, float> mb = { (root->back->ssec.p2.first - root->back->ssec.p1.first) / 2, (root->back->ssec.p2.second - root->back->ssec.p1.second) / 2 };
	//	distb = (mb.first - v.first) * (mb.first - v.first) + (mb.second - v.second) * (mb.second - v.second);
	//}
	//int m = (root->ssec.p2.second - root->ssec.p1.second) / (root->ssec.p2.first - root->ssec.p1.first);
	//int res1 = v.second - root->ssec.p2.second - (m * v.first) + (m * root->ssec.p2.first);
	if (cross_z(root->ssec.p1, root->ssec.p2, v) > 0) {
		if(root->back != nullptr) traverse(root->back, v, maptree);
		maptree.push_back(root->ssec);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->front != nullptr) traverse(root->front, v, maptree);
	}

	if (cross_z(root->ssec.p1, root->ssec.p2, v) < 0) {
		if (root->front != nullptr) traverse(root->front, v, maptree);
		maptree.push_back(root->ssec);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->back != nullptr) traverse(root->back, v, maptree);
	}

	//TODO: same plane handling

	if (cross_z(root->ssec.p1, root->ssec.p2, v) == 0) {
		if (root->front != nullptr) traverse(root->front, v, maptree);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->back != nullptr) traverse(root->back, v, maptree);
	}
}

void trees::print(std::pair<float, float> v, std::vector<Walls>& maptree) {
	traverse(root, v, maptree);
}

void trees::printBT(const treenode* node)
{
	if (node != nullptr)
	{
		printwalls(node->ssec);
		if (node->front != nullptr) {
			std::cout << "front: " << std::endl;
			printBT(node->front);
		}
		else {
			std::cout << "leaf" << std::endl;
		}
		if (node->back != nullptr) {
			std::cout << "back: " << std::endl;
			printBT(node->back);
		}
		else {
			std::cout << "leaf" << std::endl;
		}
	}
}

void trees::printBT_()
{
	printBT(root);
}

