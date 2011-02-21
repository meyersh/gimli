#ifndef __MATRIX_UTILS__HPP__
#define __MATRIX_UTILS__HPP__
/******************************************************************************
 * matrix_utils.hpp - BREADTH FIRST SEARCH functions
 * Written by: Shaun Meyer, Feb 2011
 * 
 * DESCRIPTION:
 * Calculate a path in a graph between two points using the breadth first 
 * algorithm. This has the happy side effect of giving us the shortest path
 * while it's at it.
 *****************************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/scoped_array.hpp>

#include "adjmatrix/adjmatrix.hpp"
#include "stackorqueue/stackorqueue.hpp"
#include "shaun.hpp"

/*
 * Constants
 */

#define GRAPH_PATH "/home/meyersh/public/graphs/"

/*
 * Prototypes
 */

adjMatrix  &load_matrix(const char *filename, std::string &name);

int              save_matrix(const char *filename, adjMatrix &matrix, 
			     std::string name="default");

std::vector<int> bfs_get_path(adjMatrix &matrix, int start_node, int end_node);
std::vector<int> dfs_get_path(adjMatrix &matrix, int start_node, int end_node);

int         print_matrix_as_dot(const char *filename, bool is_directed=false);

#endif
