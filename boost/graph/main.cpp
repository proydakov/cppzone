/*
 *  Copyright (c) 2015 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <cmath>
#include <chrono>
#include <fstream>
#include <iostream>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef float distance_t;

struct vertex_property
{
    size_t x;
    size_t y;
};

typedef boost::property<boost::edge_weight_t, distance_t> edge_property;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_property, edge_property> grid;

typedef boost::graph_traits<grid>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<grid>::vertices_size_type vertices_size_type;

class vertex_hash;

typedef std::vector<vertex_descriptor> vertex_vector;
typedef boost::unordered_set<vertex_descriptor, vertex_hash> vertex_set;

typedef std::vector<std::string> name_vector;

// Exception thrown when the goal vertex is found
struct found_goal {};

///////////////////////////////////////////////////////////////////////////////

// A hash function for vertices.
class vertex_hash
{
public:
    vertex_hash(const grid& graph) : m_graph(graph) {}

    std::size_t operator()(const vertex_descriptor& v) const
    {   
        std::size_t seed = 0;
        boost::hash_combine(seed, m_graph[v].x);
        boost::hash_combine(seed, m_graph[v].y);
        return seed;
    }

private:
    const grid& m_graph;
};

///////////////////////////////////////////////////////////////////////////////

//
// Euclidean heuristic for a grid
//
// This calculates the Euclidean distance between a vertex and a goal
// vertex.
//
class euclidean_heuristic : public boost::astar_heuristic<grid, distance_t>
{
public:
    euclidean_heuristic(const grid& graph, const vertex_descriptor& goal) : m_graph(graph), m_goal(goal) {}

    distance_t operator() (vertex_descriptor v) {
        distance_t x = static_cast<distance_t>(std::fabs(m_graph[m_goal].x - m_graph[v].x));
        distance_t y = static_cast<distance_t>(std::fabs(m_graph[m_goal].y - m_graph[v].y));
        return static_cast<distance_t>(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
    }

private:
    const grid& m_graph;
    vertex_descriptor m_goal;
};

///////////////////////////////////////////////////////////////////////////////

// Visitor that terminates when we find the goal vertex
struct astar_goal_visitor : public boost::default_astar_visitor
{
    astar_goal_visitor(const vertex_descriptor& goal) : m_goal(goal) {}

    void examine_vertex(const vertex_descriptor& u, const grid&) {
        if (u == m_goal) {
            throw found_goal();
        }
    }

private:
    vertex_descriptor m_goal;
};

///////////////////////////////////////////////////////////////////////////////

class grid_map
{
public:
    grid_map(size_t width, size_t height) : m_width(width), m_height(height)
    {
        const size_t size = width * height;
        m_name.resize(size);

        // init horizontal edge
        const distance_t horizontal_size = 1;
        for(size_t x = 0; x < width - 1; x++) {
            for(size_t y = 0; y < height; y++) {
                const size_t index = calc_vertex_index(x, y);
                boost::add_edge(index, index + 1, horizontal_size, m_grid);
            }
        }

        // init vertical edge
        const distance_t vertical_size = 1;
        for(size_t x = 0; x < width; x++) {
            for(size_t y = 0; y < height - 1; y++) {
                const size_t index = calc_vertex_index(x, y);
                boost::add_edge(index, index + width, vertical_size, m_grid);
            }
        }

        // init diagonal edge
        const distance_t diagonal_size = static_cast<distance_t>(std::sqrt(2));
        for(size_t x = 0; x < width - 1; x++) {
            for(size_t y = 0; y < height - 1; y++) {
                const size_t index = calc_vertex_index(x, y);
                boost::add_edge(index, index + width + 1, diagonal_size, m_grid);
                boost::add_edge(index + 1, index + width, diagonal_size, m_grid);
            }
        }

        // fill name and vertex descriptin
        for(size_t x = 0; x < width; x++) {
            for(size_t y = 0; y < height; y++) {
                const size_t index = calc_vertex_index(x, y);
                vertex_descriptor descriptor(index);
                m_grid[descriptor].x = x;
                m_grid[descriptor].y = y;
                std::stringstream sstream;
                sstream << "(" << x << ", " << y << ")";
                m_name[index] = sstream.str();
            }
        }
    }

    size_t calc_vertex_index(size_t x, size_t y)
    {
        return x + y * m_width;
    }

    const grid::vertex_bundled& get_vertex_property(vertex_descriptor vertex) const
    {
        return m_grid[vertex];
    }

    bool search_path(const vertex_descriptor& source, const vertex_descriptor& goal, vertex_vector& solution)
    {
        const size_t solution_assessment_size = m_width + m_height;
        vertex_hash hash(m_grid);

        // The predecessor map is a vertex-to-vertex mapping.
        typedef boost::unordered_map<vertex_descriptor, vertex_descriptor, vertex_hash> pred_map;
        pred_map predecessor(solution_assessment_size, hash);
        boost::associative_property_map<pred_map> pred_pmap(predecessor);

        // The distance map is a vertex-to-distance mapping.
        typedef boost::unordered_map<vertex_descriptor, distance_t, vertex_hash> dist_map;
        dist_map distance(solution_assessment_size, hash);
        boost::associative_property_map<dist_map> dist_pmap(distance);

        euclidean_heuristic heuristic(m_grid, goal);
        astar_goal_visitor  visitor  (goal);

        bool result = false;
        const int iters = 10;

        std::chrono::high_resolution_clock clock;
        auto start = clock.now();

        for(int i = 0; i < iters; i++) {
            solution.clear();
            predecessor.clear();
            distance.clear();
            try {
                boost::astar_search(m_grid, source, heuristic,
                                    boost::visitor(visitor).
                                    predecessor_map(pred_pmap).
                                    distance_map(dist_pmap)
                                    );
            }
            catch(const found_goal&) {
                // Walk backwards from the goal through the predecessor chain adding
                // vertices to the solution path.
                for (vertex_descriptor u = goal; u != source; u = predecessor[u]) {
                    solution.push_back(u);
                }
                solution.push_back(source);
                std::reverse(solution.begin(), solution.end());
                result = true;
            }
        }
        auto end = clock.now();

        distance_t dist = distance[goal];
        std::cout << "solve. result: " << result << " size: " << dist << " process time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iters << " us"
                  << std::endl;

        return result;
    }

    void dump_to_dot()
    {
        std::ofstream dot("graph.dot");
        boost::write_graphviz(dot, m_grid, boost::make_label_writer(&m_name[0]));
        dot.close();
    }

private:
    grid        m_grid;
    name_vector m_name;

    size_t m_width;
    size_t m_height;
};

///////////////////////////////////////////////////////////////////////////////

int main()
{
    const size_t w = 5;
    const size_t h = 5;

    grid_map map(w, h);

    vertex_descriptor source = map.calc_vertex_index(0, 0);
    vertex_descriptor goal = map.calc_vertex_index(w - 1, h - 1);

    auto property_source = map.get_vertex_property(source);
    std::cout << "search from: (" << property_source.x << ", " << property_source.y << ")";

    auto property_goal = map.get_vertex_property(goal);
    std::cout << " to: (" << property_goal.x << ", " << property_goal.y << ")" << std::endl;

    vertex_vector solution;
    map.search_path(source, goal, solution);

    map.dump_to_dot();

    return 0;
}
