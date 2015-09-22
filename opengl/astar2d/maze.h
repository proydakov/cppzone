/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <chrono>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/graph/grid_graph.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

boost::mt19937 random_generator;

#define GRID_RANK 2

typedef int distance_t;
typedef boost::grid_graph<GRID_RANK> grid;
typedef boost::graph_traits<grid>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<grid>::vertices_size_type vertices_size_type;

std::ostream& operator<<(std::ostream& os, const vertex_descriptor& vertex)
{
    os << "( " << vertex[0] << ", " << vertex[1] << " )";
    return os;
}

// A hash function for vertices.
struct vertex_hash : std::unary_function<vertex_descriptor, std::size_t>
{
    std::size_t operator()(vertex_descriptor const& u) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, u[0]);
        boost::hash_combine(seed, u[1]);
        return seed;
    }
};

typedef std::vector<vertex_descriptor> vertex_vector;
typedef boost::unordered_set<vertex_descriptor, vertex_hash> vertex_set;
typedef boost::vertex_subset_complement_filter<grid, vertex_set>::type filtered_grid;

//
// Euclidean heuristic for a grid
//
// This calculates the Euclidean distance between a vertex and a goal
// vertex.
//
class euclidean_heuristic : public boost::astar_heuristic<filtered_grid, double>
{
public:
    euclidean_heuristic(const vertex_descriptor& goal) : m_goal(goal) {};

    double operator() (vertex_descriptor v) {
        double x = (m_goal[0] - v[0]) * (m_goal[0] - v[0]);
        double y = (m_goal[1] - v[1]) * (m_goal[1] - v[1]);
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

private:
    vertex_descriptor m_goal;
};

// Exception thrown when the goal vertex is found
struct found_goal {};

// Visitor that terminates when we find the goal vertex
struct astar_goal_visitor : public boost::default_astar_visitor
{
    astar_goal_visitor(const vertex_descriptor& goal, vertex_vector& way) : m_goal(goal), m_way(way) { m_way.clear(); };

    void examine_vertex(const vertex_descriptor& u, const filtered_grid&) {
        if (u == m_goal) {
            throw found_goal();
        }
        m_way.push_back(u);
    }

private:
    vertex_descriptor m_goal;
    vertex_vector& m_way;
};

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
    enum maze_type {
        fixed,
        empty,
        random
    };

public:
    friend std::ostream& operator<<(std::ostream&, const maze&);

    friend boost::shared_ptr<maze> random_maze(std::size_t x, std::size_t y);
    friend boost::shared_ptr<maze> empty_maze (std::size_t x, std::size_t y);
    friend boost::shared_ptr<maze> fixed();

    maze() : m_grid(create_grid(0, 0)), m_barrier_grid(create_barrier_grid()) {};
    maze(std::size_t x, std::size_t y) : m_grid(create_grid(x, y)),
        m_barrier_grid(create_barrier_grid()) {};

    vertices_size_type length(std::size_t d) const {
        return m_grid.length(d);
    }
    vertex_descriptor source() const {
        return m_soure;
    }
    vertex_descriptor goal() const {
        return m_goal;
    }

    bool solve();
    bool solved() const {
        return !m_solution.empty();
    }
    const vertex_vector& get_way() const {
        return m_way;
    }
    const vertex_vector& get_solution() const {
        return m_solution;
    }
    const vertex_set& get_barriers() const {
        return m_barriers;
    }
    bool has_barrier(vertex_descriptor u) const {
        return m_barriers.find(u) != m_barriers.end();
    }

private:
    grid create_grid(std::size_t x, std::size_t y) {
        boost::array<std::size_t, GRID_RANK> lengths = { {x, y} };
        return grid(lengths);
    }

    filtered_grid create_barrier_grid() {
        return boost::make_vertex_subset_complement_filter(m_grid, m_barriers);
    }

    grid m_grid;
    filtered_grid m_barrier_grid;
    vertex_set m_barriers;
    std::vector<vertex_descriptor> m_solution;
    std::vector<vertex_descriptor> m_way;
    vertex_descriptor m_soure;
    vertex_descriptor m_goal;
};

// Solve the maze using A-star search.  Return true if a solution was found.
bool maze::solve()
{
    boost::static_property_map<distance_t> weight(1);

    // The predecessor map is a vertex-to-vertex mapping.
    typedef boost::unordered_map<vertex_descriptor, vertex_descriptor, vertex_hash> pred_map;
    pred_map predecessor;
    boost::associative_property_map<pred_map> pred_pmap(predecessor);

    // The distance map is a vertex-to-distance mapping.
    typedef boost::unordered_map<vertex_descriptor, distance_t, vertex_hash> dist_map;
    dist_map distance;
    boost::associative_property_map<dist_map> dist_pmap(distance);

    vertex_descriptor s = source();
    vertex_descriptor g = goal();
    euclidean_heuristic heuristic(g);
    astar_goal_visitor visitor(g, m_way);

    bool result = false;
    const int iters = 10;

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    for(int i = 0; i < iters; i++) {
        try {
            boost::astar_search(m_barrier_grid, s, heuristic,
                                boost::visitor(visitor).
                                weight_map(weight).
                                predecessor_map(pred_pmap).
                                distance_map(dist_pmap)
                                );
        }
        catch(found_goal fg) {
            // Walk backwards from the goal through the predecessor chain adding
            // vertices to the solution path.
            for (vertex_descriptor u = g; u != s; u = predecessor[u]) {
                m_solution.push_back(u);
            }
            m_solution.push_back(s);
            std::reverse(m_solution.begin(), m_solution.end());
            result = true;
        }
    }
    auto end = clock.now();
    std::cout << "solve. result: " << result << " process time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iters << " us" << std::endl;

    return result;
}

// Return a random integer in the interval [a, b].
std::size_t random_int(std::size_t a, std::size_t b)
{
    if (b < a) {
        b = a;
    }
    boost::uniform_int<> dist(a, b);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
            generate(random_generator, dist);
    return generate();
}

// Generate a maze with a random assignment of barriers.
boost::shared_ptr<maze> random_maze(std::size_t x, std::size_t y)
{
    boost::shared_ptr<maze> m = boost::shared_ptr<maze>(new maze(x, y));

    vertex_descriptor s = {{ 0, 0 }};
    vertex_descriptor g = {{ x - 1, y -1 }};
    m->m_soure = s;
    m->m_goal  = g;

    vertices_size_type n = num_vertices(m->m_grid);

    // One quarter of the cells in the maze should be barriers.
    int barriers = pow(n, 0.75);
    while (barriers > 0) {
        // Choose horizontal or vertical direction.
        std::size_t direction = random_int(0, 1);
        // Walls range up to one quarter the dimension length in this direction.
        vertices_size_type wall = random_int(1, m->length(direction) / 4);
        // Create the wall while decrementing the total barrier count.
        vertex_descriptor u = vertex(random_int(0, n - 1), m->m_grid);
        while (wall) {
            // Start and goal spaces should never be barriers.
            if (u != s && u != g) {
                wall--;
                if (!m->has_barrier(u)) {
                    m->m_barriers.insert(u);
                    barriers--;
                }
            }
            vertex_descriptor v = m->m_grid.next(u, direction);
            // Stop creating this wall if we reached the maze's edge.
            if (u == v) {
                break;
            }
            u = v;
        }
    }
    return m;
}

boost::shared_ptr<maze> empty_maze(std::size_t x, std::size_t y)
{
    boost::shared_ptr<maze> m = boost::shared_ptr<maze>(new maze(x, y));

    vertex_descriptor s = {{ 0, 0 }};
    vertex_descriptor g = {{ x - 1, y -1 }};
    m->m_soure = s;
    m->m_goal  = g;

    return m;
}

boost::shared_ptr<maze> fixed()
{
    boost::shared_ptr<maze> m = boost::shared_ptr<maze>(new maze(9, 9));

    vertex_descriptor u1 = {{4, 2}};
    vertex_descriptor u2 = {{4, 3}};
    vertex_descriptor u3 = {{4, 4}};
    vertex_descriptor u4 = {{4, 5}};

    m->m_barriers.insert(u1);
    m->m_barriers.insert(u2);
    m->m_barriers.insert(u3);
    m->m_barriers.insert(u4);

    vertex_descriptor source = {{2, 4}};
    m->m_soure = source;

    vertex_descriptor goal = {{6, 4}};
    m->m_goal = goal;

    return m;
}
