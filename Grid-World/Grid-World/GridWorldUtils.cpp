#include "GridWorldUtils.h"

bool next(std::vector<std::vector<Cell>>& stage, Agent& agent)
{
    int rowMax = stage.size();
    int colMax = stage[0].size();
    int r = agent.m_row;
    int c = agent.m_col;
    if (stage[r][c].isRewardCell() == true) {
        return true; // Finish
    }

    int move_dir = stage[r][c].getMaxDir();
    int nr = r + dr[move_dir];
    int nc = c + dc[move_dir];

    float Q = stage[nr][nc].getReward() + stage[nr][nc].getMaxValue();
    stage[r][c].setExpectedValue(move_dir, Q);
    agent.m_row = nr;
    agent.m_col = nc;
    return false; // Still running
}

bool train(std::vector<std::vector<Cell>>& stage, Agent& agent)
{
    int rowMax = stage.size();
    int colMax = stage[0].size();
    int r = agent.m_row;
    int c = agent.m_col;
    if (stage[r][c].isRewardCell() == true) {
        return true; // Finish
    }

    int move_dir = stage[r][c].getRandDir();
    int nr = r + dr[move_dir];
    int nc = c + dc[move_dir];

    float Q = stage[nr][nc].getReward() + stage[nr][nc].getMaxValue();
    //std::cout << Q << " = " << stage[nr][nc].getReward() << " + " << stage[nr][nc].getMaxValue() << std::endl;
    stage[r][c].setExpectedValue(move_dir, Q);
    agent.m_row = nr;
    agent.m_col = nc;
    return false; // Still running
}

void disp(std::vector<std::vector<Cell>>& stage, Agent& agent, bool isTraining, int step)
{
    int stage_row = stage.size();
    int stage_col = stage[0].size();
    cv::Mat display(10 + 160 * stage_row + 100, 10 + 160 * stage_col, CV_8UC3);

    {
        for (int i = 0; i < stage_row; i++) {
            for (int j = 0; j < stage_col; j++) {
                int offsetY = 10 + (160 * i);
                int offsetX = 10 + (160 * j);
                cv::rectangle(display, cv::Rect(cv::Point(offsetX, offsetY), cv::Point(offsetX + 150, offsetY + 150)), cv::Scalar(0, 0, 0), 2);

                if (stage[i][j].isRewardCell()) {
                    std::stringstream sstr;
                    sstr << "Reward: " << std::fixed << std::setprecision(1) << stage[i][j].getReward();
                    cv::putText(display, sstr.str(), cv::Point(offsetX + 35, offsetY + 80), 2, 0.5, cv::Scalar(0, 0, 0), 1);
                }
                else {
                    cv::line(display, cv::Point(offsetX, offsetY + 150), cv::Point(offsetX + 150, offsetY), cv::Scalar(0, 0, 0), 2);
                    cv::line(display, cv::Point(offsetX, offsetY), cv::Point(offsetX + 150, offsetY + 150), cv::Scalar(0, 0, 0), 2);
                    for (int k = 0; k < 4; k++) {
                        std::stringstream sstr;
                        sstr << std::fixed << std::setprecision(1) << stage[i][j].getExpectedValue(k);
                        int px = offsetX;
                        int py = offsetY;
                        if (k == dir_up) { px += 70; py += 30; }
                        if (k == dir_down) { px += 70; py += 130; }
                        if (k == dir_left) { px += 20; py += 80; }
                        if (k == dir_right) { px += 110; py += 80; }
                        if(stage[i][j].isBoundary(k))
                            cv::putText(display, sstr.str(), cv::Point(px, py), 2, 0.5, cv::Scalar(0, 0, 255), 1);
                        else
                            cv::putText(display, sstr.str(), cv::Point(px, py), 2, 0.5, cv::Scalar(0, 0, 0), 1);
                    }
                }
                if (i == agent.m_row && j == agent.m_col) {
                    cv::circle(display, cv::Point(offsetX + 75, offsetY + 75), 15, cv::Scalar(255, 0, 0), 3);
                }
            }
        }
        if (isTraining) {
            std::stringstream sstr;
            sstr << "Training... , Step: " << step;
            cv::putText(display, sstr.str(), cv::Point(50, 10 + 160 * stage_row + 50), 2, 1, cv::Scalar(0, 0, 255));
        }
        else {
            cv::putText(display, "Running", cv::Point(10 + 160 * stage_row + 50, 10 + 160 * stage_col), 2, 1, cv::Scalar(255, 0, 0));
        }
    }


    cv::imshow("display", display);
    cv::waitKey(1);
}