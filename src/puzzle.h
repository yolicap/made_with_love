// puzzle.h

#ifndef PUZZLE_H
#define PUZZLE_H

class Puzzle {
private:
	bool complete;
public:
    virtual void Draw(olc::Draw& draw, float fElapsedTime) = 0;
	virtual void Update(float fElapsedTime) = 0;
	virtual bool isComplete() = 0;

private:
	virtual void checkComplete() = 0;
};

#endif