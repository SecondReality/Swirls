#include "stdafx.h"
#include "Game.h"
#include "Constants.h"

#include <gl\gl.h>
#include <gl\glu.h>

std::uniform_real_distribution<float> distribution(0, 1);
std::random_device rd;
std::mt19937 engine(rd()); // Mersenne twister MT19937

void DrawSquare(float x, float y, float size)
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f(x, y+size, 0.0f);					// Top Left
		glVertex3f( x+size, y+size, 0.0f);					// Top Right
		glVertex3f( x+size, y, 0.0f);					// Bottom Right
		glVertex3f(x, y, 0.0f);					// Bottom Left
	glEnd();	

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Game::recursivePopulate(CurlTree::iterator & it)
{
	if(tree_.depth(it)>4)
	{
		return;
	}

	for(int i=0; i<3; i++)
	{
		if(rand() % 5 == 0)
		{
			continue;
		}

		float randomAngle = distribution(engine);
		PositionedCurl p(randomAngle*0.9f, /*rand() % 2 == 0 ? */Curl::Direction::Clockwise/* :Curl::Direction::AntiClockwise*/);
		CurlTree::iterator ci = tree_.append_child(it, p);
		recursivePopulate(ci);
	}
}

Game::Game(void)
{
	// Build the tree:
	auto top=tree_.begin();

	PositionedCurl child;

	// Add the root node:
	auto root = tree_.insert(top, child);

	// Add children:
	recursivePopulate(root);
	/*
	auto child2 = tree_.append_child(root, child);
	auto child3 = tree_.append_child(child2, child);
	auto child4 = tree_.append_child(child3, child);
	*/
}

Game::~Game(void)
{
}

void Game::recursiveDraw(CurlTree::sibling_iterator& it)
{
	glPushMatrix();

	// Get the position angle:
	float currentAngle = it->getAngleForAttachmentPoint();

	float scale = it->fractionComplete() * 0.6f;

	// Set our position based on our attachment angle:
	smr::Vector3f offset = it->getPositionAtLength(currentAngle);
	smr::Vector3f baseCorrection = offset;
	baseCorrection.normalize();
	baseCorrection*= (it->baseWidth() * scale);

	offset-=baseCorrection;

	glTranslatef(offset.x, offset.y, 0.0f);	

	// Convert currentAngle to degrees:
	currentAngle*=radsToDegs;

	glRotatef(currentAngle+180, 0.0f, 0.0f, -1.0f);

	glScalef(scale, scale, 1);
	glTranslatef(0, -1.0f, 0.0f);	


	for(CurlTree::sibling_iterator node = tree_.begin(it); node!=tree_.end(it); node++)
	{
		recursiveDraw(node);
	}

	glBegin(GL_POINTS);
	it->draw(timer_.delta(), false);

	/*
	glVertex3f(0, 0, 0.0f);
	glVertex3f(0.0, 0.2, 0.0f);
	glVertex3f(0.1, 0, 0.0f);
	glVertex3f(0, 1.1, 0.0f);
	*/

	glEnd();
		//DrawSquare(-0.2, -0.2, 0.4);

	glPopMatrix();	
}

void Game::update()
{
	timer_.frame();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(-1.5f, 0.0f, -6.0f);						// Move Left 1.5 Units And Into The Screen 6.0

	//recursiveDraw(tree_.begin(tree_.begin()));

	glBegin(GL_POINTS);
	tree_.begin()->draw(timer_.delta());
	glEnd();

	recursiveDraw(tree_.begin().begin());

	for(auto node = tree_.begin(); node!=tree_.end(); node++)
	{

	}

/*
	if(rand()%10000 == 0)
	{	
		//curlPositions.push_back(smr::Vector3f(generator(), generator(), 0.0f)); // Doesnt' generate random values for some reason?
		Curl curl(rand() % 2 == 0 ? Curl::Direction::Clockwise : Curl::Direction::AntiClockwise);
		curls_.push_back(curl);

		auto generator = std::bind(distribution, engine);
		curlPositions_.push_back(smr::Vector3f(distribution(engine), distribution(engine), 0.0f));
	}
	
	for(int i=0; i<curls_.size(); i++)
	{
		glPushMatrix();

		DrawSquare(-1,-2, 4);

		Curl& curl = curls_[i];
		smr::Vector3f& position = curlPositions_[i];

		glTranslatef(position.x, position.y, -6.0f);	

		glBegin(GL_POINTS);
		curl.draw(timer_.delta());//0.0001f);
		glEnd();

		glPopMatrix();		
	}
	*/
}

		/*
	glBegin(GL_TRIANGLES);								// Drawing Using Triangles
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	glEnd();											// Finished Drawing The Triangle
	glTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
	glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	glEnd();						
	*/