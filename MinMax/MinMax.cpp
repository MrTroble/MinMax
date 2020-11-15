#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <string>

std::array<std::array<uint8_t, 3>, 3> board;

struct ScoreStruct {
	uint8_t xpos;
	uint8_t ypos;
	uint8_t score;
	uint8_t reserved = 0; // alignment

	constexpr bool operator<(ScoreStruct scs) { return score < scs.score; }
	constexpr bool operator>(ScoreStruct scs) { return score > scs.score; }
};

inline const uint8_t winnner() {
	for (size_t i = 0; i < 3; i++) {
		const uint8_t firstRow = board[0][i];
		if (firstRow != 1 && firstRow == board[1][i] && firstRow == board[2][i])
			return firstRow;

		const auto& row = board[i];
		const uint8_t first = row[0];
		if (first != 1 && first == row[1] && first == row[2])
			return first;
	}

	const uint8_t lefttop = board[0][0];
	if (lefttop != 1 && lefttop == board[1][1] && lefttop == board[2][2])
		return lefttop;

	const uint8_t righttop = board[0][2];
	if (righttop != 1 && righttop == board[1][1] && righttop == board[2][0])
		return righttop;

	return 1;
}

inline ScoreStruct minmax(bool turn) {
	const uint8_t win = winnner();
	if (win != 1)
		return ScoreStruct{ 0, 0, win, 1 };

	std::vector<ScoreStruct> scores;
	scores.reserve(9);
	const uint8_t currentPlayer = 2 * turn;
	for (size_t x = 0; x < board.size(); x++) {
		for (size_t y = 0; y < board[0].size(); y++) {
			if (board[x][y] == 1) {
				board[x][y] = currentPlayer;
				scores.push_back({ (uint8_t)x, (uint8_t)y, minmax(!turn).score, 0 });
				board[x][y] = 1;
			}
		}
	}

	if (scores.empty())
		return ScoreStruct{ 0, 0, 1, 1 };

	return turn ? *std::max_element(scores.begin(), scores.end()) : *std::min_element(scores.begin(), scores.end());
}

inline void printField() {
	std::cout << std::endl;
	for (auto& row : board) {
		for (auto id : row)
			std::cout << '|' << (uint32_t)id;
		std::cout << '|' << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	for (auto& arr : board)
		std::fill(arr.begin(), arr.end(), (uint8_t)1);

	do {
		int posx = -1, posy = -1;
		while (posx < 0 || posx > 2 || posy < 0 || posy > 2 || board[posx][posy] != 1) {
			if(std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore();
			}
			std::cin >> posx >> posy;
		}
		board[posx][posy] = 0;
		printField();

		ScoreStruct score = minmax(true);
		if (score.reserved == 1)
			break;
		board[score.xpos][score.ypos] = 2;
		printField();
	} while (true);
}
