/*****************************************************************************
 *   Copyright (C) 2004-2014 The PaGMO development team,                     *
 *   Advanced Concepts Team (ACT), European Space Agency (ESA)               *
 *   http://apps.sourceforge.net/mediawiki/pagmo                             *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers  *
 *   http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits     *
 *   act@esa.int                                                             *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#ifndef PAGMO_PROBLEM_BASE_TSP_H
#define PAGMO_PROBLEM_BASE_TSP_H

#include "base.h"
#include <vector>
#include <boost/graph/graphviz.hpp>
//#include "graph_helper.hpp"

namespace pagmo{ namespace problem {

/// Boost graph type (could also be added to types.h, for now it only has scope here.
/**
 * std::vector (vecS) are fastest for iterators: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/using_adjacency_list.html
 * External properties can also be added: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/quick_tour.html
 * for hashmaps or other types, which are better for other operations.
 */
/* Graph Internal Properties */    
typedef boost::property<boost::vertex_index_t, int 
    /*, boost::property<boost::vertex_name_t, std::string> */> tsp_vertex_properties;
typedef boost::property<boost::edge_index_t, int, 
        boost::property<boost::edge_weight_t, double> > tsp_edge_properties;
/* Graph Type, internal containers for edges and lists */
typedef boost::adjacency_list<  
            boost::setS, // disallow parallel edges
            boost::listS, // vertex container
            boost::directedS, // directed graph
            tsp_vertex_properties, 
            tsp_edge_properties
        > tsp_graph;
/* Graph specific typedefs */
typedef boost::graph_traits<tsp_graph>::vertex_descriptor tsp_vertex;
typedef boost::graph_traits<tsp_graph>::edge_descriptor tsp_edge;

typedef boost::property_map<tsp_graph, boost::vertex_index_t>::type tsp_vertex_map_index;
typedef boost::property_map<tsp_graph, boost::edge_weight_t>::type tsp_edge_map_weight;
typedef boost::property_map<tsp_graph, boost::vertex_index_t>::const_type tsp_vertex_map_const_index;
typedef boost::property_map<tsp_graph, boost::edge_weight_t>::const_type tsp_edge_map_const_weight;

typedef boost::graph_traits<tsp_graph>::vertex_iterator tsp_vertex_iter;
typedef boost::graph_traits<tsp_graph>::edge_iterator tsp_edge_iter;
typedef boost::graph_traits<tsp_graph>::adjacency_iterator tsp_adjacency_iter;
typedef boost::graph_traits<tsp_graph>::out_edge_iterator tsp_out_edge_iter;
typedef boost::graph_traits<tsp_graph>::degree_size_type tsp_degree_t;

typedef std::pair<tsp_edge, tsp_edge> tsp_edge_pair;
typedef std::pair<tsp_adjacency_iter, tsp_adjacency_iter> tsp_adjacency_vertex_range_t;
typedef std::pair<tsp_out_edge_iter, tsp_out_edge_iter> tsp_out_edge_range_t;
typedef std::pair<tsp_vertex_iter, tsp_vertex_iter> tsp_vertex_range_t;
typedef std::pair<tsp_edge_iter, tsp_edge_iter> edge_range_t;


/// Shortened version of the 2D vector type
template <typename T>
using vector2D = typename std::vector<std::vector<T> >;

/// Base TSP.
/**
 * This is a base class for Traveling Salesman Problems.
 * m_graph is of type tsp_graph, defined above.
 * It is an adjacency list representing symmetric or assymmetric graphs, with edges representing cost/weight between vertices.
 * The internal properties of the boost graph are defined above in the tsp_edge_properties.
 * The only internal property for a vertice is its index.
 * All problems are considered to be directed graphs, since for undirected it's just a matter of storage space.
 * Most of the TSPLIB problems are dense (e.g. fully connected graphs).
 *
 * @author Florin Schimbinschi (florinsch@gmail.com)
 */

class __PAGMO_VISIBLE base_tsp : public base
{
    public:
            /**
             * Constructor with the dimension of the problem (number of vertices)
             * @param[in] number of vertices
             */
            base_tsp(int);
            /**
             * Getter for the m_graph
             * @return reference to the m_graph of type tsp_graph
             */
            tsp_graph const& get_graph() const;
            /**
             * Setter for the m_graph
             * @param[in] tsp_graph
             */
            void set_graph(tsp_graph const&);
            /**
             * Setter for the m_graph
             * @param[in] 2D vector of doubles
             */
            void set_graph(vector2D<double> const&);

    protected:
            /**
             * Converts a 2D vector to a boost graph type tsp_graph
             * @param[in] the 2D vector of doubles
             * @param[out] the tsp_graph adjacency list
             */
            void vector2D_to_graph(vector2D<double> const&, tsp_graph&);
            /**
             * Checks the maximum dimensions for both width and height.
             * The 'matrix' might not be square for asymmetric problems.
             * Returns the number of vertices in the 2D vector,
             * which is either max(row, col) for sparse matrices.
             * @return number of vertices
             */
            int get_no_vertices(vector2D<double> const&) const;
            /**
             * The boost graph, an adjacency list
             * derived classes inherit this property
             */
            tsp_graph m_graph;
                
    private:                
            friend class boost::serialization::access;
            template <class Archive>
            void serialize(Archive &ar, const unsigned int)
            {
                    ar & boost::serialization::base_object<base>(*this);
                    ar & m_graph;
            }
    
};

}} //namespaces

#endif // PAGMO_PROBLEM_BASE_TSP_H