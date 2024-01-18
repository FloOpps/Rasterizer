//!
//! \brief     Defines class Color related functions.
//!
//! \file      color.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "color.h"

using namespace minwin;

//! A string representation of the color.
std::string minwin::to_string( const Color & color )
{
	return std::to_string( color.r ) + ", "
       + std::to_string( color.g ) + ", "
       + std::to_string( color.b ) + ", "
       + std::to_string( color.a );
}

//! Equality test.
bool operator==( const Color & color1, const Color & color2 )
{
  return color1.r == color2.r
     and color1.g == color2.g
     and color1.b == color2.b
     and color1.a == color2.a;
}
