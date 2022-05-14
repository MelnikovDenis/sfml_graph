#include "Graph.h"
bool AdjacencyMatrix::isNoEmpty(std::vector<std::vector<int>> matrix) {
	bool isFind = false;
	for (unsigned int i = 0; i < matrix.size() && !isFind; ++i) {
		for (unsigned int j = 0; j < matrix.size() && !isFind; ++j) {
			if (matrix[i][j] != NOVALUE) isFind = true;
		}
	}
	return isFind;
}
std::vector<AdjacencyMatrix::Tuple<int, int>> AdjacencyMatrix::getWayParts() const {
	//массив для записи в него результатов (результатом будет пара индксов откуда-куда)
	std::vector<Tuple<int, int>> wayParts;

	//копия матрицы смежности
	std::vector<std::vector<int>> replica(getVerticesCount());
	for (unsigned int i = 0; i < getVerticesCount(); ++i)
		copy(matrix[i].begin(), matrix[i].end(), back_inserter(replica[i]));

	while (isNoEmpty(replica)) {
		//дополнительный столбец, в котором хранятся минимумы строк
		std::vector<int> minColumn(getVerticesCount());
		for (unsigned int i = 0; i < getVerticesCount(); ++i)
			minColumn[i] = *min_element(replica[i].begin(), replica[i].end());

		//редукция строк
		for (unsigned int i = 0; i < getVerticesCount(); ++i) for_each(replica[i].begin(), replica[i].end(), [&i, &minColumn](int& num)->void {
			if (num != NOVALUE) num -= minColumn[i];
		});

		//дополнительная строка, в которой хранятся минимумы столбцов
		std::vector<int> minRow(getVerticesCount());
		for (unsigned int i = 0; i < getVerticesCount(); ++i) {
			int min = NOVALUE;
			for (unsigned int j = 0; j < getVerticesCount(); ++j) {
				if (replica[j][i] < min) min = replica[j][i];
			}
			minRow[i] = min;
		}

		//реукция столбцов
		for (unsigned int i = 0; i < getVerticesCount(); ++i) {
			for (unsigned int j = 0; j < getVerticesCount(); ++j) {
				if (replica[j][i] != NOVALUE) replica[j][i] -= minRow[i];
			}
		}

		//производим оценку каждого 0 (оценка будет записываться отрицательным числом)
		for (unsigned int i = 0; i < getVerticesCount(); ++i) {
			for (unsigned int j = 0; j < getVerticesCount(); ++j) {
				if (replica[i][j] <= 0) {
					int rmin = NOVALUE;
					int cmin = NOVALUE;
					for (unsigned int k = 0; k < getVerticesCount(); ++k) {
						if (replica[i][k] < 0) rmin = 0;
						else  if (replica[i][k] < rmin && k != j) rmin = replica[i][k];

						if (replica[k][j] < 0) cmin = 0;
						else if (replica[k][j] < cmin && k != i) cmin = replica[k][j];
					}
					replica[i][j] = -1 * (rmin + cmin);
				}
			}
		}

		//поиск наибольшей оценки (т.к. у нас отрицательные числа, то наименьшей)
		int maxGrade = NOVALUE; int maxGrade_i = NOVALUE; int maxGrade_j = NOVALUE;
		for (unsigned int i = 0; i < getVerticesCount(); ++i) {
			for (unsigned int j = 0; j < getVerticesCount(); ++j) {
				if (maxGrade > replica[i][j]) {
					maxGrade = replica[i][j];
					maxGrade_i = i;
					maxGrade_j = j;
				}
			}
		}

		//запись найденного пути в итоговый массив - wayParts
		wayParts.push_back(Tuple<int, int>{maxGrade_i, maxGrade_j});

		//очистка матрицы от оценок и столбца, строки и противоположного элемента найденного пути 
		for (int i = 0; i < getVerticesCount(); ++i) {
			for (int j = 0; j < getVerticesCount(); ++j) {
				if (i == maxGrade_i) replica[i][j] = NOVALUE;
				if (j == maxGrade_j) replica[i][j] = NOVALUE;
				if (replica[i][j] < 0) replica[i][j] = 0;
			}
		}
		replica[maxGrade_j][maxGrade_i] = NOVALUE;
	}
	return wayParts;
}
unsigned int AdjacencyMatrix::getVerticesCount() const {
	return matrix.size();
}
int AdjacencyMatrix::getWayLength(std::vector<Tuple<int, int>>& wayParts)const {
	int length = 0;
	for (int i = 0; i < wayParts.size(); ++i) {
		length += matrix[wayParts[i].item1][wayParts[i].item2];
	}
	return length;
}
std::vector<int> AdjacencyMatrix::getWay(int startPoint)const {
	if (startPoint >= 0 && startPoint < matrix.size()) {
		std::vector<Tuple<int, int>> wayParts = getWayParts();
		static std::vector<int> way(matrix.size() + 1);
		bool wayFind;
		int currPoint = startPoint;

		for (int i = 0; i < way.size(); ++i) {
			wayFind = false;
			for (int j = 0; j < wayParts.size() && !wayFind; ++j) {
				if (wayParts[j].item1 == currPoint) {
					way[i] = currPoint;
					currPoint = wayParts[j].item2;					
					wayFind = true;
				}
			}
			if (!wayFind) { 
				throw std::invalid_argument("Не удаётся найти решение задачи коммивояжера для этой точки!"); 
			}
		}
		return way;
	}
	else throw std::invalid_argument("Такой точки не существует!");
}
AdjacencyMatrix::AdjacencyMatrix() {
}
AdjacencyMatrix& AdjacencyMatrix::operator=(const AdjacencyMatrix& adjMatrix) {
	this->matrix = std::vector<std::vector<int>>(adjMatrix.getVerticesCount());
	for (unsigned int i = 0; i < adjMatrix.getVerticesCount(); ++i) {
		copy(adjMatrix.matrix[i].begin(), adjMatrix.matrix[i].end(), back_inserter(matrix[i]));
	}
	return *this;
}
AdjacencyMatrix::AdjacencyMatrix(std::vector<std::vector<int>>& matrix) {
	this->matrix = std::vector<std::vector<int>>(matrix.size());
	for (unsigned int i = 0; i < getVerticesCount(); ++i) copy(matrix[i].begin(), matrix[i].end(), back_inserter(this->matrix[i]));
	for (int i = 0; i < getVerticesCount(); ++i) this->matrix[i][i] = NOVALUE;
}
AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix& adjMatrix) {
	this->matrix = std::vector<std::vector<int>>(adjMatrix.getVerticesCount());
	for (unsigned int i = 0; i < adjMatrix.getVerticesCount(); ++i)
		copy(adjMatrix.matrix[i].begin(), adjMatrix.matrix[i].end(), back_inserter(matrix[i]));
}