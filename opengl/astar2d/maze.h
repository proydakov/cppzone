/*
 *  Copyright (c) 2012-2015 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <memory>
#include <chrono>
#include <iostream>
#include <functional>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

typedef int distance_t;

struct vertex_property
{
    distance_t x;
    distance_t y;
};

typedef boost::property<boost::edge_weight_t, distance_t> edge_property;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_property, edge_property> grid;

typedef boost::graph_traits<grid>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<grid>::vertices_size_type vertices_size_type;

class vertex_hash;

typedef std::vector<vertex_descriptor> vertex_vector;
typedef boost::unordered_set<vertex_descriptor, vertex_hash> vertex_set;
typedef boost::vertex_subset_complement_filter<grid, vertex_set>::type filtered_grid;

typedef std::vector<std::string> name_vector;

///////////////////////////////////////////////////////////////////////////////

// A hash function for vertices.
class vertex_hash : std::unary_function<vertex_descriptor, std::size_t>
{
public:
    vertex_hash(const grid& graph);
    std::size_t operator()(const vertex_descriptor& v) const;

private:
    const grid& m_graph;
};

///////////////////////////////////////////////////////////////////////////////

//
// A searchable maze
//
// The maze is grid of locations which can either be empty or contain a
// barrier.  You can move to an adjacent location in the grid by going up,
// down, left and right.  Moving onto a barrier is not allowed.  The maze can
// be solved by finding a path from the lower-left-hand corner to the
// upper-right-hand corner.  If no open path exists between these two
// locations, the maze is unsolvable.
//
// The maze is implemented as a filtered grid graph where locations are
// vertices.  Barrier vertices are filtered out of the graph.
//
// A-star search is used to find a path through the maze. Each edge has a
// weight of one, so the total path length is equal to the number of edges
// traversed.
//
class maze
{
public:
    enum class maze_type {
        fixed,
        empty,
        random
    };

    enum class heuristic_type {
        euclidean,
        euclidean_squared,
        manhattan,
        diagonal
    };

public:
    friend std::shared_ptr<maze> random_maze(std::size_t x, std::size_t y);
    friend std::shared_ptr<maze> empty_maze (std::size_t x, std::size_t y);
    friend std::shared_ptr<maze> fixed();

    maze(std::size_t width, std::size_t height);

    void set_heuristic(heuristic_type h);

    bool solve();
    bool solved() const;

    bool has_barrier(vertex_descriptor u) const;

    vertex_descriptor source_vertex() const;
    vertex_descriptor goal_vertex() const;

    const vertex_vector& get_heuristic_tested() const;
    const vertex_vector& get_solution() const;
    const vertex_set& get_barriers() const;

    const grid::vertex_bundled& get_vertex_property(vertex_descriptor vertex) const;

    vertices_size_type length(std::size_t direction) const;
    vertex_descriptor next(vertex_descriptor u, std::size_t direction);

    void dump_to_dot();

private:
    std::size_t random_int(std::size_t a, std::size_t b);
    filtered_grid create_barrier_grid();
    size_t calc_vertex_index(size_t x, size_t y);
    std::string heuristic_type_2_string(heuristic_type h) const;

private:
    typedef std::function<distance_t(distance_t, distance_t)> heuristic_t;

    boost::mt19937 m_random_generator;

    size_t m_width;
    size_t m_height;

    grid            m_grid;
    heuristic_type  m_heuristic_type;
    heuristic_t     m_heuristic;
    name_vector     m_name;
    vertex_set      m_barriers;
    filtered_grid   m_barrier_grid;

    vertex_vector m_solution;
    vertex_vector m_tested;
    vertex_descriptor m_soure;
    vertex_descriptor m_goal;
};

// Generate a maze with a random assignment of barriers.
std::shared_ptr<maze> random_maze(std::size_t x, std::size_t y);
std::shared_ptr<maze> empty_maze(std::size_t x, std::size_t y);
std::shared_ptr<maze> fixed();
