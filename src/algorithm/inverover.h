/*****************************************************************************
 *   Copyright (C) 2004-2013 The PaGMO development team,                     *
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

#ifndef PAGMO_ALGORITHM_INVEROVER_H
#define PAGMO_ALGORITHM_INVEROVER_H

#include <algorithm>

#include "../config.h"
#include "../serialization.h"
#include "../population.h"
#include "../problem/base_tsp.h"
#include "base.h"

namespace pagmo { namespace algorithm {

/// Inver-Over Algorithm (IO)
/**
 * The Inver-Over algorithm is a state-of-the-art genetic algorithm for the Travelling Salesman Problem.
 * It was designed by G. Tao and Z. Michalewicz in 1998.
 * 
 * Note: The algorithm was sightly changed (choice of the next city in a series of inverisons)
 * since with this choice better performance (tour length, computational time) was observed.
 *
 * Note2: The value for the population size is advised to be no smaller than 20.
 * To not have premature convergence, values around 100 are observed to work well.
 *
 * Note3: The inversion sequence for cases where city1 is later in the tour than city2 is
 * chosen as in the original paper (city1 -> city2). Some papers invert the complementary part of the tour
 * (city2 -> city1).
 *
 * @author Ingmar Getzner (ingmar.getzner@gmail.com)
 */
class __PAGMO_VISIBLE inverover: public base
{
public:
	enum initialization_type
	{
		random = 0,
		nn = 1
	};
	inverover(int gen = 10000, double ri = 0.05, initialization_type ini_type = nn);
        base_ptr clone() const;
        void evolve(population &) const;
        std::string get_name() const;
protected:
	// void problem::tsp::nn_tsp::evolve(population &) const

private:
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int)
        {
                ar & boost::serialization::base_object<base>(*this);
		ar & const_cast<int &>(m_gen);
		ar & const_cast<double &>(m_ri);
		ar & m_ini_type;
        }
	//Number of generations
        const int m_gen;
	//Mutation probalility
        const double m_ri;
	//Methode for initialization
	initialization_type m_ini_type;

};

}} //namespaces

BOOST_CLASS_EXPORT_KEY(pagmo::algorithm::inverover)

#endif // PAGMO_ALGORITHM_INVEROVER_H
