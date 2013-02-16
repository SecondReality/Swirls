#pragma once

#include "Timer.h"
#include "Curl.h"
#include "tree.hh"

#include <vector>
#include "Vector.h"

#include <random>
#include <functional>

class Game
{
public:
	Game(void);
	~Game(void);
	void update();

private:
	typedef tree<PositionedCurl> CurlTree;
	void recursiveDraw(CurlTree::sibling_iterator& it);

	smr::WorldTimer timer_;
	std::vector<Curl> curls_;
	std::vector<smr::Vector3f> curlPositions_;

	CurlTree tree_;

	void recursivePopulate(CurlTree::iterator & i);
};