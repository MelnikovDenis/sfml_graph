#include "Graph.h"

void Graph::snapEdgesToVertices() {
	int counter = 0;
	for (int i = 0; i < adjMatrix.getVerticesCount(); ++i) {
		for (int j = i + 1; j < adjMatrix.getVerticesCount(); ++j) {
			if (adjMatrix.matrix[i][j] != NOVALUE || adjMatrix.matrix[j][i] != NOVALUE) {
				edges[counter].setPosition(vertices[i].getCenter(), vertices[j].getCenter());
				counter++;
			}
		}
	}
}

Graph::Graph(std::vector<std::vector<int>>& matrix, const std::vector<std::wstring>& names, const Font& font, const int& textSize = 15, 
	const Vector2f& SPREAD_CENTER = Vector2f(260.f, 260.f), const int& SPREAD_RADIUS = 260) {
	this->adjMatrix = AdjacencyMatrix(matrix);
	vertices = std::vector<gVertex>(adjMatrix.getVerticesCount());
	for (int i = 0; i < adjMatrix.getVerticesCount(); ++i) {
		vertices[i] = gVertex(names[i] , font, textSize);
	}
	gVertex::getPolygonSpread(vertices, SPREAD_CENTER, SPREAD_RADIUS);
	for (int i = 0; i < adjMatrix.getVerticesCount(); ++i) {
		for (int j = i + 1; j < adjMatrix.getVerticesCount(); ++j) {
			if (adjMatrix.matrix[i][j] != NOVALUE && adjMatrix.matrix[j][i] != NOVALUE) {
				edges.push_back(gEdge(adjMatrix.matrix[i][j], adjMatrix.matrix[j][i], font, textSize));
				edges.back().setPosition(vertices[i].getCenter(), vertices[j].getCenter());
			}
			else if (adjMatrix.matrix[i][j] = NOVALUE && adjMatrix.matrix[j][i] != NOVALUE) {
				edges.push_back(gEdge(NOVALUE, adjMatrix.matrix[j][i], font, textSize));
				edges.back().setPosition(vertices[i].getCenter(), vertices[j].getCenter());
			}
			else if (adjMatrix.matrix[i][j] != NOVALUE && adjMatrix.matrix[j][i] == NOVALUE) {
				edges.push_back(gEdge(adjMatrix.matrix[i][j], NOVALUE, font, textSize));
				edges.back().setPosition(vertices[i].getCenter(), vertices[j].getCenter());
			}
			
		}
	}
}