#include"GridWorldUtils.h"

const auto agent_start_row = 0;
const auto agent_start_col = 0;
const auto stage_row = 7;
const auto stage_col = 7;
const auto delay = 10;
const auto move_cost = -0.1;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout.tie(0);
	srand((unsigned int)time(NULL));
	std::vector<std::vector<Cell>> stage(stage_row);
	
	float reward_map[stage_row][stage_col];
	for (int i = 0; i < stage_row; i++)
		for (int j = 0; j < stage_col; j++)
			reward_map[i][j] = 0.0f;
	reward_map[0][3] = -2.0;
	reward_map[3][4] = -3.0;
	reward_map[1][2] = -2.0;
	reward_map[3][1] = -3.0;
	reward_map[5][4] = -1.5;
	reward_map[4][2] = -2.0;
	reward_map[6][5] = -4.0;
	reward_map[6][6] = 5.0;

	for (int i = 0; i < stage_row; i++) {
		for (int j = 0; j < stage_col; j++) {
			if(reward_map[i][j] == 0.0)
				stage[i].emplace_back(move_cost);
			else
				stage[i].emplace_back(reward_map[i][j], true);
		}
	}

	for (int i = 0; i < stage_row; i++) {
		for (int j = 0; j < stage_col; j++) {
			stage[i][j].updateBoundary(i, j, stage_row, stage_col);
		}
	}

	Agent agent(agent_start_row, agent_start_col);
	disp(stage, agent, true, 0);
	cv::waitKey(0);

	int step = 0;

	while(step < 3000 ){
		
		bool finished = train(stage, agent);
		disp(stage, agent, true, step);
		cv::waitKey(delay / 2);
		if (finished == true) {
			step++;
			agent.reset(agent_start_row, agent_start_col);
			disp(stage, agent, true, step);
			cv::waitKey(delay / 2);
		}
	}

	while (true) {
		bool finished = next(stage, agent);
		disp(stage, agent, false, 0);
		cv::waitKey(delay * 5);
		if (finished == true) {
			agent.reset(agent_start_row, agent_start_col);
			disp(stage, agent, false, 0);
			cv::waitKey(delay * 5);
		}
	}

	return 0;
}