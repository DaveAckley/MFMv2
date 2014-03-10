#ifndef ATOM_H      /* -*- C++ -*- */
#define ATOM_H

#include "itype.h"
#include <stdio.h>  /* For FILE */

namespace MFM {

  template <class T, u32 R> class Element; // Forward declaration

  template <class T, u32 R>
    class Atom
    {
    public:
      bool IsType(u32 type) const
      {
        return GetType()==type;
      }

      bool IsSameType(const Atom & other) const
      {
        return GetType()==other.GetType();
      }

      //      const Element<T,R> * GetElement() const ;

      virtual u32 GetType() const = 0;

      virtual void Print(FILE* ostream) const = 0;

    };
} /* namespace MFM */

#include "atom.tcc"

#endif /*ATOM_H*/

