#include "trees.h"

void trees::populateNormals(std::vector<Walls>& w) {
	for (auto& wall : w) {
		int dx = wall.p2.first - wall.p1.first;
		int dy = wall.p2.second - wall.p1.second;
		wall.normal = { -dy, dx };
	}
}

inline int dot(std::pair<int, int>a, std::pair<int, int>b) {
	return ((a.first* b.first) + (a.second * b.second));
}

void trees::printwalls(Walls a) {
	std::cout << "(" << a.p1.first << "," << a.p1.second << "), (" << a.p2.first << "," << a.p2.second << ")" << std::endl;
}
int isleft(std::pair<float, float> a, std::pair<float, float> b, std::pair<float, float> c) {
	if ((b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first) == 0) return 2;
	if ((b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first) > 0) return 0;
	if ((b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first) < 0) return 1;
}
// 0 front 1 back 2 intersection first __ second
int trees::WallComp(Walls first, Walls second) {
	/*if ((first.p2.first - first.p1.first) == 0) {
		float m = (second.p2.second - second.p1.second) / (second.p2.first - second.p1.first);
		float res1 = first.p1.second - second.p2.second - (m * first.p1.first) + (m * second.p2.first);
		float res2 = first.p2.second - second.p2.second - (m * first.p2.first) + (m * second.p2.first);
		if ((res1 >= 0 && res2 >= 0)) {
			return 1;
		}
		if (res1 < 0 && res2 < 0) {
			return 0;
		}
		std::cout << "res inside" << res1 << ", " << res2 << std::endl;
		return 2;
	}*/
	// need to handle when slope tends to infinity
	if (first.p2.first - first.p1.first == 0) {
		if (isleft(first.p1, first.p2, second.p1) == 0 || isleft(first.p1, first.p2, second.p2) == 0) {
			return 1;
		}
		return 0;

	}
	float m = (first.p2.second - first.p1.second) / (first.p2.first - first.p1.first);
	float res1 = second.p1.second - first.p2.second - (m * second.p1.first) + (m * first.p2.first);
	float res2 = second.p2.second - first.p2.second - (m * second.p2.first) + (m * first.p2.first);
	if ((res1 >= 0 && res2 >= 0)) {
		return 0;
	}
	if (res1 < 0 && res2 < 0) {
		return 1;
	}
	if (res1 < 0 && res2 == 0 || res1 == 0 && res2 < 0) {
		return 1;
	}
	std::cout << "res " << res1 << ", " << res2 << std::endl;
	return 2;
}

inline void trees::intersectionHandle(Walls curwall, Walls dividingwall, std::vector<Walls>& frontwall, std::vector<Walls>& backwall) {

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

	if (WallComp(w1, dividingwall) == 0) {

		frontwall.push_back(w1);
		backwall.push_back(w2);
	}
	else if (WallComp(w1, dividingwall) == 1) {
		frontwall.push_back(w2);
		backwall.push_back(w1);
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
	int m = (root->ssec.p2.second - root->ssec.p1.second) / (root->ssec.p2.first - root->ssec.p1.first);
	int res1 = v.second - root->ssec.p2.second - (m * v.first) + (m * root->ssec.p2.first);
	if (res1 < 0) {
		if(root->back != nullptr) traverse(root->back, v, maptree);
		maptree.push_back(root->ssec);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->front != nullptr) traverse(root->front, v, maptree);
	}

	if (res1 > 0) {
		if (root->front != nullptr) traverse(root->front, v, maptree);
		maptree.push_back(root->ssec);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->back != nullptr) traverse(root->back, v, maptree);
	}

	//TODO: same plane handling

	if (res1 == 0) {
		if (root->front != nullptr) traverse(root->front, v, maptree);
		//std::cout << "(" << root->ssec.p1.first << root->ssec.p1.second << "), (" << root->ssec.p2.first << root->ssec.p2.second << ")" << std::endl;
		if (root->back != nullptr) traverse(root->back, v, maptree);
	}
}

void trees::print(std::pair<float, float> v, std::vector<Walls>& maptree) {
	traverse(root, v, maptree);
}