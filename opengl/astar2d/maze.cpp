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

#include "maze.h"
#include <fstream>
#include <boost/graph/graphviz.hpp>

boost::mt19937 random_generator = boost::mt19937();

// Exception thrown when the goal vertex is found
struct found_goal {};

///////////////////////////////////////////////////////////////////////////////

distance_t euclidean_distance(distance_t dx, distance_t dy)
{
	return std::sqrt(dx * dx + dy * dy);
}

distance_t euclidean_squared_distance(distance_t dx, distance_t dy)
{
	return dx * dx + dy * dy;
}

distance_t manhattan_distance(distance_t dx, distance_t dy)
{
	return dx + dy;
}

distance_t diagonal_distance(distance_t dx, distance_t dy)
{
	return dx + dy + (14 - 20) / 10.0 * std::min(dx, dy);
}

///////////////////////////////////////////////////////////////////////////////

//
// Heuristic for a grid
//
// This calculates the Euclidean distance between a vertex and a goal
// vertex.
//
template<class graph_t, class heuristic_impl>
class euclidean_heuristic : public boost::astar_heuristic<graph_t, distance_t>
{
public:
    euclidean_heuristic(const graph_t& graph, const heuristic_impl& impl, vertex_vector& tested, const vertex_descriptor& goal) :
		m_graph(graph),
		m_impl(impl),
		m_tested(tested),
		m_goal(goal)
	{
	}

    distance_t operator() (vertex_descriptor v) {
		m_tested.push_back(v);

        distance_t dx = std::abs(m_graph[m_goal].x - m_graph[v].x) * 10;
        distance_t dy = std::abs(m_graph[m_goal].y - m_graph[v].y) * 10;

		distance_t result = m_impl(dx, dy);
        return result;
    }

private:
    const graph_t& m_graph;
	const heuristic_impl m_impl;
	vertex_vector& m_tested;
    vertex_descriptor m_goal;
};

///////////////////////////////////////////////////////////////////////////////

// Visitor that terminates when we find the goal vertex
template<class graph_t>
struct astar_goal_visitor : public boost::default_astar_visitor
{
    astar_goal_visitor(const vertex_descriptor& goal) :
		m_goal(goal)
	{
	}

    void examine_vertex(const vertex_descriptor& u, const graph_t& graph) {
        if (u == m_goal) {
            throw found_goal();
        }
    }

private:
    vertex_descriptor m_goal;
};

///////////////////////////////////////////////////////////////////////////////

maze::maze(std::size_t width, std::size_t height) :
	m_heuristic_type(heuristic_type::euclidean),
    m_width(width),
    m_height(height),
    m_barriers(width * height, vertex_hash(m_grid)),
    m_barrier_grid(create_barrier_grid())
{
    const size_t size = width * height;
    m_name.resize(size);

    // init horizontal edge
    const distance_t horizontal_size = 10;
    for(size_t x = 0; x < width - 1; x++) {
        for(size_t y = 0; y < height; y++) {
            const size_t index = calc_vertex_index(x, y);
            boost::add_edge(index, index + 1, horizontal_size, m_grid);
        }
    }

    // init vertical edge
    const distance_t vertical_size = 10;
    for(size_t x = 0; x < width; x++) {
        for(size_t y = 0; y < height - 1; y++) {
            const size_t index = calc_vertex_index(x, y);
            boost::add_edge(index, index + width, vertical_size, m_grid);
        }
    }

    // init diagonal edge
    const distance_t diagonal_size = 14;
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

	set_heuristic(m_heuristic_type);
}

void maze::set_heuristic(heuristic_type h)
{
	heuristic_t heuristic;

	switch (h) {
	case heuristic_type::euclidean:
		heuristic = heuristic_t(std::bind(&euclidean_distance, std::placeholders::_1, std::placeholders::_2));
		break;

	case heuristic_type::euclidean_squared:
		heuristic = heuristic_t(std::bind(&euclidean_squared_distance, std::placeholders::_1, std::placeholders::_2));
		break;

	case heuristic_type::manhattan:
		heuristic = heuristic_t(std::bind(&manhattan_distance, std::placeholders::_1, std::placeholders::_2));
		break;

	case heuristic_type::diagonal:
		heuristic = heuristic_t(std::bind(&diagonal_distance, std::placeholders::_1, std::placeholders::_2));
		break;

	default:
		assert(false);
		break;
	}
	if (heuristic) {
		m_heuristic = heuristic;
		m_heuristic_type = h;
	}
}

vertices_size_type maze::length(std::size_t d) const
{
    vertices_size_type size = 0;

    switch(d) {
        case 0:
            size = m_width;
            break;

        case 1:
            size = m_height;
            break;

        default:
            assert(false);
            break;
    }
    return size;
}

vertex_descriptor maze::source_vertex() const
{
    return m_soure;
}

vertex_descriptor maze::goal_vertex() const
{
    return m_goal;
}

// Solve the maze using A-star search.  Return true if a solution was found.
bool maze::solve()
{
    const vertex_descriptor& source = source_vertex();
    const vertex_descriptor& goal = goal_vertex();
    vertex_vector& solution = m_solution;

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

    euclidean_heuristic<grid, heuristic_t> heuristic(m_grid, m_heuristic, m_tested, goal);
    astar_goal_visitor<filtered_grid>      visitor  (goal);

    bool result = false;
    const int iters = 1;

	std::cout << "astar search... ";

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    for(int i = 0; i < iters; i++) {
		m_tested.clear();
        solution.clear();
        predecessor.clear();
        distance.clear();
        try {
            boost::astar_search(m_barrier_grid, source, heuristic,
                                boost::visitor(visitor).
                                predecessor_map(pred_pmap).
                                distance_map(dist_pmap)
                                );
        }
        catch(const found_goal& fg) {
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
    std::cout << "solve. result: " << result << " size: " << dist 
		<< " heuristic: " << heuristic_type_2_string(m_heuristic_type)
		<< " process time: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iters << " us"
		<< std::endl;

    dump_to_dot();

    return result;
}

bool maze::solved() const
{
    return !m_solution.empty();
}

const vertex_vector& maze::get_heuristic_tested() const
{
    return m_tested;
}

const vertex_vector& maze::get_solution() const
{
    return m_solution;
}

const vertex_set& maze::get_barriers() const
{
    return m_barriers;
}

const grid::vertex_bundled& maze::get_vertex_property(vertex_descriptor vertex) const
{
    return m_grid[vertex];
}

bool maze::has_barrier(vertex_descriptor u) const
{
    return m_barriers.find(u) != m_barriers.end();
}

vertex_descriptor maze::next(vertex_descriptor u, std::size_t direction)
{
    size_t delta_x = 0;
    size_t delta_y = 0;

    switch(direction) {
        case 0:
            delta_x = 1;
            break;
            
        case 1:
            delta_y = 1;
            break;
            
        default:
            assert(false);
            break;
    }
    const size_t x = m_grid[u].x;
    const size_t y = m_grid[u].y;

    const size_t new_x = x + delta_x;
    const size_t new_y = y + delta_y;

    if(new_x >= m_width || new_y >= m_height) {
        return u;
    }
    vertex_descriptor new_u = calc_vertex_index(new_x, new_y);
    return new_u;
}

void maze::dump_to_dot()
{
    std::ofstream dot("astar.dot");
    boost::write_graphviz(dot, m_grid, boost::make_label_writer(&m_name[0]));
    dot.close();
}

filtered_grid maze::create_barrier_grid()
{
    return boost::make_vertex_subset_complement_filter(m_grid, m_barriers);
}

size_t maze::calc_vertex_index(size_t x, size_t y)
{
    return x + y * m_width;
}

std::string maze::heuristic_type_2_string(heuristic_type h) const
{
	std::string text;
	switch (h) {
	case heuristic_type::euclidean:
		text = "euclidean_distance";
		break;

	case heuristic_type::euclidean_squared:
		text = "euclidean_squared_distance";
		break;

	case heuristic_type::manhattan:
		text = "manhattan_distance";
		break;

	case heuristic_type::diagonal:
		text = "diagonal_distance";
		break;

	default:
		assert(false);
		break;
	}
	return text;
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
    
    vertex_descriptor s = m->calc_vertex_index( 0, 0 );
    vertex_descriptor g = m->calc_vertex_index( x - 1, y -1 );
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
            vertex_descriptor v = m->next(u, direction);
            // Stop creating this wall if we reached the maze's edge.
            if (u == v) {
                break;
            }
            u = v;
        }
    }
    return m;
}

boost::shared_ptr<maze> empty_maze(std::size_t width, std::size_t height)
{
    boost::shared_ptr<maze> m = boost::shared_ptr<maze>(new maze(width, height));
    
    vertex_descriptor s = m->calc_vertex_index( 0, 0 );
    vertex_descriptor g = m->calc_vertex_index( width - 1, height -1 );
    m->m_soure = s;
    m->m_goal  = g;
    
    return m;
}

boost::shared_ptr<maze> fixed()
{
    boost::shared_ptr<maze> m = boost::shared_ptr<maze>(new maze(9, 9));
    
    vertex_descriptor u1 = m->calc_vertex_index(4, 2);
    vertex_descriptor u2 = m->calc_vertex_index(4, 3);
    vertex_descriptor u3 = m->calc_vertex_index(4, 4);
    vertex_descriptor u4 = m->calc_vertex_index(4, 5);
    
    m->m_barriers.insert(u1);
    m->m_barriers.insert(u2);
    m->m_barriers.insert(u3);
    m->m_barriers.insert(u4);
    
    vertex_descriptor source = m->calc_vertex_index(2, 4);
    m->m_soure = source;
    
    vertex_descriptor goal = m->calc_vertex_index(6, 4);
    m->m_goal = goal;
    
    return m;
}
