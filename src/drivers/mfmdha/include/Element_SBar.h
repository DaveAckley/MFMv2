#ifndef ELEMENT_SBAR_H   /* -*- C++ -*- */
#define ELEMENT_SBAR_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "Element_Empty.h"
#include "Element_Res.h"
#include "itype.h"
#include "FXP.h"
#include "ColorMap.h"

namespace MFM
{

  // Forward
  template <class CC> class Element_DBar ;

  /**
    A straight bar-forming class, with symmetry.

    Bar grid positions in an event window

       -4 -3 -2 -1  0  1  2  3  4
    -4  .  .  .  .  x  .  .  .  .
    -3  .  .  .  .  .  .  .  .  .
    -2  .  .  x  .  x  .  x  .  .
    -1  .  .  .  .  .  .  .  .  .
     0  x  .  x  .  C  .  x  .  x
     1  .  .  .  .  .  .  .  .  .
     2  .  .  x  .  x  .  x  .  .
     3  .  .  .  .  .  .  .  .  .
     4  .  .  .  .  x  .  .  .  .

    Each bar atom knows how big the grid is supposed to be
    (GetMax()), where within that grid it is supposed to be
    (GetPos()), and what symmetry it is using (GetSym()).

    Class:
    (C1) Name is Element_SBar
    (C2) Type is 0x5ba2
   */


  template <class EC>
  class Element_SBar : public Element<EC>
  {
    // Extract short names for parameter types
    typedef typename EC::ATOM_CONFIG AC;
    typedef typename AC::ATOM_TYPE T;
    enum { R = EC::EVENT_WINDOW_RADIUS };

    template <u32 TVBITS>
    static u32 toSignMag(s32 value) {
      const u32 SIGN_BIT = 1<<(TVBITS-1);
      const u32 MAX = SIGN_BIT-1;
      u32 sign = 0;
      if (value < 0) {
        sign = SIGN_BIT;
        value = -value;
      }
      if (value > MAX)
        value = MAX;
      u32 val = (u32) value;
      return sign|val;
    }

    template <u32 TVBITS>
    static u32 toMag(u32 value) {
      const u32 MAX = (1<<TVBITS)-1;
      if (value < 0) {
        FAIL(ILLEGAL_ARGUMENT);
      }
      if (value > MAX)
        value = MAX;
      u32 val = value;
      return val;
    }

    template <u32 TVBITS>
    static s32 fromSignMag(const u32 value) {
      const u32 SIGN_BIT = 1<<(TVBITS-1);
      const u32 MASK = SIGN_BIT-1;
      FXP16 val = value&MASK;
      if (value & SIGN_BIT)
        val = -val;
      return val;
    }

    template <u32 TVBITS>
    static u32 fromMag(const u32 value) {
      const u32 MASK = (1<<TVBITS)-1;
      u32 val = value&MASK;
      return val;
    }

    template <u32 TXBITS, u32 TYBITS>
    static u32 toUTiny(const UPoint & v) {
      u32 x = toMag<TXBITS>(v.GetX());
      u32 y = toMag<TYBITS>(v.GetY());
      return (x<<TYBITS)|y;
    }

    template <u32 TXBITS, u32 TYBITS>
    static UPoint toUPoint(const u32 bits) {

      u32 x = fromMag<TXBITS>(bits>>TYBITS);
      u32 y = fromMag<TYBITS>(bits);
      return UPoint(x,y);
    }

  public:

    static Element_SBar THE_INSTANCE;
    static const u32 TYPE() {
      return THE_INSTANCE.GetType();
    }

    static const u32 TYPE_BITS = 15;

    static bool IsOurType(u32 type) {
      return type==TYPE();
    }

    static const u32 BITS_WIDE = 5;
    static const u32 BITS_HIGH = 7;
    static const u32 BITS_SYMI = 3;

    static const u32 BITS_BAR_COORD_LEN = BITS_WIDE + BITS_HIGH;

    static const u32 STATE_SIZE_IDX = 0;
    static const u32 STATE_SIZE_LEN = BITS_BAR_COORD_LEN;
    static const u32 STATE_POS_IDX = STATE_SIZE_IDX + STATE_SIZE_LEN;
    static const u32 STATE_POS_LEN = BITS_BAR_COORD_LEN;
    static const u32 STATE_SYMI_IDX = STATE_POS_IDX + STATE_POS_LEN;
    static const u32 STATE_SYMI_LEN = BITS_SYMI;
    static const u32 STATE_BITS = STATE_SYMI_IDX + STATE_SYMI_LEN;

    enum { SBAR_VERSION = 1 };

    Element_SBar() :
      Element<EC>(MFM_UUID_FOR("SBar", SBAR_VERSION))
    {
      Element<EC>::SetAtomicSymbol("Sb");
    }

    u32 GetSymI(const T &atom) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      return atom.GetStateField(STATE_SYMI_IDX,STATE_SYMI_LEN);
    }

    SPoint GetMax(const T &atom) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      return MakeSigned(toUPoint<BITS_WIDE,BITS_HIGH>(atom.GetStateField(STATE_SIZE_IDX,STATE_SIZE_LEN)));
    }

    SPoint GetPos(const T &atom) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      return MakeSigned(toUPoint<BITS_WIDE,BITS_HIGH>(atom.GetStateField(STATE_POS_IDX,STATE_POS_LEN)));
    }

    bool FitsInRep(const SPoint & v) const {
      return v.BoundedBy(SPoint(0,0),SPoint((1<<BITS_WIDE)-1,(1<<BITS_HIGH)-1));
    }

    void SetSize(T &atom, const SPoint & v) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      if (!FitsInRep(v))
        FAIL(ILLEGAL_ARGUMENT);
      atom.SetStateField(STATE_SIZE_IDX,STATE_SIZE_LEN,toUTiny<BITS_WIDE,BITS_HIGH>(MakeUnsigned(v)));
    }

    void SetPos(T &atom, const SPoint v) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      if (!FitsInRep(v))
        FAIL(ILLEGAL_ARGUMENT);
      atom.SetStateField(STATE_POS_IDX,STATE_POS_LEN,toUTiny<BITS_WIDE,BITS_HIGH>(MakeUnsigned(v)));
    }

    void SetSymI(T &atom, const u32 sym) const {
      if (!IsOurType(atom.GetType()))
        FAIL(ILLEGAL_STATE);
      if (sym >= PSYM_SYMMETRY_COUNT)
        FAIL(ILLEGAL_ARGUMENT);
      atom.SetStateField(STATE_SYMI_IDX,STATE_SYMI_LEN, sym);
    }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,STATE_BITS);
      return defaultAtom;
    }

    T GetAtom(const SPoint & size, const SPoint & pos) const
    {
      T atom = GetDefaultAtom();
      SetSize(atom,size);
      SetPos(atom,pos);

      return atom;
    }

    virtual u32 GetElementColor() const
    {
      return 0xffffff00;
    }

    virtual u32 PercentMovable(const T& you, const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual u32 GetAtomColor(const T & atom, u32 selector) const
    {
      switch (selector) {
      case 1: {
        SPoint barMax = GetMax(atom);
        SPoint myPos = GetPos(atom);
        return ColorMap_SEQ6_PuRd::THE_INSTANCE.
          GetInterpolatedColor(myPos.GetEuclideanLength(),
                               0,barMax.GetEuclideanLength(),
                               0xffff0000);
      }
      default:
        return Element<EC>::GetElementColor();
      }
    }

    /**
       We do not diffuse
     */
    virtual u32 Diffusability(EventWindow<EC> & ew, SPoint nowAt, SPoint maybeAt) const {
      return this->NoDiffusability(ew, nowAt, maybeAt);
    }

    virtual void Behavior(EventWindow<EC>& window) const
    {
      // Get self, sanity check
      T self = window.GetCenterAtomSym();
      const u32 selfType = self.GetType();
      if (!IsOurType(selfType)) FAIL(ILLEGAL_STATE);

      // Establish our symmetry before non-self access through window
      u32 symi = GetSymI(self);
      window.SetSymmetry((PointSymmetry) symi);

      Random & random = window.GetRandom();

      SPoint barMax = GetMax(self);
      SPoint myPos = GetPos(self);
      //      myPos.Print(stderr);

      const MDist<R> & md = MDist<R>::get();

      SPoint anInconsistent;
      u32 inconsistentCount = 0;
      T unmakeGuy;

      u32 consistentCount = 0;

      SPoint toMake;
      T makeGuy;
      u32 makeCount = 0;

      SPoint toEat;
      u32 eatCount = 0;

      // Scan event window outside self
      for (u32 idx = md.GetFirstIndex(1); idx <= md.GetLastIndex(R); ++idx) {
        const SPoint sp = md.GetPoint(idx);

        // First question: A 'bar grid' site?
        bool onGrid = (sp.GetX()&1)==0 && (sp.GetY()&1)==0;

        if (onGrid) {
          // Next question: Site within the bounds of the bar?

          const SPoint theirBarPos = sp+myPos;
          bool inBar = theirBarPos.BoundedBy(SPoint(0,0),barMax);

          if (inBar) {
            // Next question: Site empty?

            const T other = window.GetRelativeAtomSym(sp);
            const u32 otherType = other.GetType();

            bool isEmpty = Element_Empty<EC>::THE_INSTANCE.IsType(otherType);

            if (isEmpty) {

              if (random.OneIn(++makeCount)) {
                toMake = sp;
                makeGuy = self; // Start with us
                SetPos(makeGuy,theirBarPos);  // Update position
                //window.SetRelativeAtom(sp,them);  // Store
              }

            } else {

              // Next question: Are they also a bar?
              if (IsOurType(otherType)) {

                // Next question: Are they consistent with us?

                SPoint otherBarMax = GetMax(other);
                SPoint otherPos = GetPos(other);

                SPoint otherPosMapped = otherPos-sp;
                if (otherBarMax==barMax && otherPosMapped == myPos) {
                  ++consistentCount;
                } else if (random.OneIn(++inconsistentCount)) {
                  anInconsistent = sp;
                  // Inconsistent Bars decay to Res
                  unmakeGuy = Element_Res<EC>::THE_INSTANCE.GetDefaultAtom();
                }
              }

            }
          }
        } else {
          // This is a non-grid site.  We'd kind of like it to be Res,
          // and if not that, empty, but we'd really like it not to
          // have Bar's in it

            const T other = window.GetRelativeAtomSym(sp);
            const u32 otherType = other.GetType();

            bool isRes = otherType == Element_Res<EC>::TYPE();
            if (isRes) {
              ++consistentCount;
              if (random.OneIn(++eatCount)) {
                toEat = sp;
              }
            }
            else {

              bool isEmpty = Element_Empty<EC>::THE_INSTANCE.IsType(otherType);

              if (isEmpty) ++consistentCount;
              else {
                bool isBar = IsOurType(otherType);
                if (isBar) {
                  if (random.OneIn(++inconsistentCount)) {
                    anInconsistent = sp;
                    unmakeGuy = Element_Empty<EC>::THE_INSTANCE.GetDefaultAtom();
                  }
                }
              }
            }
        }
      }

      // Scan finished.  Let's decide what to do.

      // First question: Are we inconsistent with anybody?
      if (inconsistentCount > 0) {
        // Next question: Are we much more consistent than inconsistent?
        if (consistentCount > 3*inconsistentCount) {
          // Yes.  Punish selected loser
          window.SetRelativeAtomSym(anInconsistent, unmakeGuy);
        } else if (inconsistentCount > 3*consistentCount) {
          // If we're way inconsistent, let's res out and let them have us
          window.SetCenterAtomSym(Element_Res<EC>::THE_INSTANCE.GetDefaultAtom());
        }
      } else {
        // No inconsistencies.  Do we have something to make, and eat?
        if (makeCount > 0 && eatCount > 0) {
          window.SetRelativeAtomSym(toMake, makeGuy);
          window.SetRelativeAtomSym(toEat, Element_Empty<EC>::THE_INSTANCE.GetDefaultAtom());
        }
        else {
          // Super-special case: Are we the max corner and all consistent?
          if (myPos == barMax-SPoint(1,1)) {
            // Is there an empty off end to us?
            SPoint offset(0,2);
            T offEnd = window.GetRelativeAtomSym(offset);
            const u32 offType = offEnd.GetType();
            if (Element_Empty<EC>::THE_INSTANCE.IsType(offType) && eatCount > 0) {
              //              T corner = self;
              T corner = Element_DBar<EC>::THE_INSTANCE.GetAtom(barMax,SPoint(0,0));
              u32 symi = GetSymI(self);
              symi = (symi+0)%PSYM_SYMMETRY_COUNT;
              Element_DBar<EC>::THE_INSTANCE.SetSymI(corner,symi);
              Element_DBar<EC>::THE_INSTANCE.SetPos(corner,SPoint(0,0));
              window.SetRelativeAtomSym(offset, corner);
              window.SetRelativeAtomSym(toEat, Element_Empty<EC>::THE_INSTANCE.GetDefaultAtom());
            }
          }
        }
      }

    }

  };

  template <class EC>
  Element_SBar<EC> Element_SBar<EC>::THE_INSTANCE;

}

#endif /* ELEMENT_SBAR_H */
