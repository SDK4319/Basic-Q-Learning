#pragma once
#include<algorithm>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<vector>
#include<opencv2/opencv.hpp>

const auto dir_up = 0;
const auto dir_down = 1;
const auto dir_left = 2;
const auto dir_right = 3;

const int dr[4] = { -1,1,0,0 };
const int dc[4] = { 0,0,-1,1 };

class Cell
{
public:

	Cell(float reward,bool isRewardCell = false) : m_Reward(reward), m_isRewardCell(isRewardCell) {}
	
	bool isRewardCell() { return m_isRewardCell; }

	float getMaxValue() {
		return std::max(std::max(m_Exp[0], m_Exp[1]), std::max(m_Exp[2], m_Exp[3]));
	}
	int getMaxDir() {
		int dir = 0;
		while (m_isBoundary[dir]) { dir += 1; }
		for (int i = 0; i < 4; i++) {
			if (m_isBoundary[i] == false && m_Exp[dir] <= m_Exp[i])
				dir = i;
		}
		return dir;
	}
	void updateBoundary(int curRow, int curCol, int maxRow, int maxCol) {
		if (curRow == 0)
			m_isBoundary[dir_up] = true;
		if (curCol == 0)
			m_isBoundary[dir_left] = true;
		if (curRow == maxRow - 1)
			m_isBoundary[dir_down] = true;
		if (curCol == maxCol - 1)
			m_isBoundary[dir_right] = true;
	}
	float getReward() { return m_Reward; }
	void setExpectedValue(int dir, float value) {
		m_Exp[dir] = value;
	}
	float getExpectedValue(int dir) {
		return m_Exp[dir];
	}
	bool isBoundary(int dir) { return m_isBoundary[dir]; }
	int getRandDir() { //Unused
		int dir = rand() % 4;
		while (m_isBoundary[dir]) { dir = rand() % 4; }
		return dir;
	}
private:
	float m_Exp[4] = { 0.0f,0.0f,0.0f,0.0f };
	float m_Reward = 0.0f;
	bool m_isRewardCell = false;
	bool m_isBoundary[4] = { 0,0,0,0 };
};

class Agent {
public:
	Agent(int startRow, int startCol) : m_row(startRow), m_col(startCol), m_totalReward(0.0f) {}
	void reset(int startRow, int startCol) { m_row = startRow; m_col = startCol; m_totalReward = 0.0f; }
	int m_row;
	int m_col;
	float m_totalReward;
};

void disp(std::vector<std::vector<Cell>>& stage, Agent& agent, bool isTraining, int step);
bool next(std::vector<std::vector<Cell>>& stage, Agent& agent);
bool train(std::vector<std::vector<Cell>>& stage, Agent& agent);
