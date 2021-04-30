#ifndef TYPES_H
  #define TYPES_H

  // #################
  // compiler specific

  //#define for if (0) ; else for   // VC60 - workaround

  #pragma warning (disable:4101) // unreferenced local

  // avoid truncated warnings:
  #pragma warning (disable:4244)
  #pragma warning (disable:4309)
  #pragma warning (disable:4311)
  #pragma warning (disable:4312)
  #pragma warning (disable:4996)

  // #########################

  #define IO // in&out, for references
  #define OO // out only, for references

  // char
  typedef signed _int8 i8;
  typedef unsigned _int8 ui8;
  // (short) int
  typedef signed _int16 i16;
  typedef unsigned _int16 ui16;
  // long (int)
  typedef signed _int32 i32;
  typedef unsigned _int32 ui32;
  // long long
  typedef signed _int64 i64;
  typedef unsigned _int64 ui64;

  typedef char t_text;
  typedef char te_text; // []-texts
  typedef ui16 t_rotate; // 0-3

  extern i16 test; // TODO DEBUG

  i8 bcon(i8 bn);
  i16 wcon(i16 wn);
  i32 lcon(i32 ln);
  ui8 ubcon(i8 bn);
  ui16 uwcon(i16 wn);
  ui32 ulcon(i32 ln);

  i16 signedword(i8);
  i32 signedlong(i16);

  ui16 unsignedword(i8);
  ui32 unsignedlong(i8);
  ui32 unsignedlong(ui8);
  ui32 unsignedlong(i16);
  ui32 unsignedlong(ui16);
  ui32 unsignedlong(t_text);

  i16 CUTX16(i32);
  i8 CUTLX8(i32);
  i8 CUTX8(i16);
  ui8 CUTUX8(i16);

  #define VCAST(z)    reinterpret_cast<void*>(z)

  //#################
  // pointer-things
  //#################

// temporary helpers:
  typedef i8 unk; // USE ONLY FOR POINTERS TO UNKNOWN TYPES!!!
  typedef unk* t_uniptr; // TODO DM2_QUERY_GDAT_ENTRY_DATA_PTR returns different types

  #define DOWNCAST(sot, z) ((i8*)(z))
  #define UPCAST(sot, z) ((sot*)(z))
// in UPCAST z has to be an i8*, else use CHGCAST
  #define CHGCAST(sot, z) ((sot*)(z))
// from/to XXX*
  #define XUPCAST(sot, z) ((sot*)(z))
  #define XDOWNCAST(sot, z) ((unk*)(z))

  #define ADRESSOF(sot, z1) DOWNCAST(sot, z1)

  i8 mkb(i32 ln);
  ui8 mkub(i32 ln);
  i16 mkw(i32 ln);
  ui16 mkuw(i32 ln);
  i32 mkl(i32 ln);
  ui32 mkul(i32 ln);

  #define pointer_to_long(x) (i32(x))
  #define is_odd_pointer(x) ((pointer_to_long(x) & 0x1) != 0)
  #define is_neg_pointer(x) ((pointer_to_long(x) & 0x80000000) != 0)

  //###################
  // function-pointer
  //###################

  #define INDCALL(x)  (*(x))

  typedef i32 (*FPTR_L_WPWP)(i16*, i16*);
  typedef bool (*FboolPTR_L_W)(i16);
  typedef i32 (*FPTR_L_LL)(i32, i32);
  typedef unk* (*FPTR_XP_Lb)(i32, bool);

  typedef i16 t_alphamask;
  // in the blit-routines two alphavalues are used.
  // this one is the one that can be negative,
  // while the other alpha is usually the unsigned lowbyte of it.
  // the reason is obviously that the upper-part is just
  // a bool to use alpha or not.
  #define NOALPHA (0xffff)
  #define ALPHA_4 (0x4)
  #define ALPHA_A (0xa)
  #define ALPHA_C (0xc)

  enum e_blitmode
  {
    BLITMODE0 = 0,
    BLITMODE1,
    BLITMODE2,
    BLITMODE3
  };

  enum e_cursoridx
  {
    NOCURSOR = -1,
    CURSOR0,
    CURSOR1,
    CURSOR2,
    CURSOR3
  };

  // TODO old-style-casting used. search for (e_blitmode)

  class c_evententry // was c_mousebufentry/c_mousedata/c_mouseentry
  {
    public:
      i16 b; // a virtual button that can also mark a keyboardinfo or smth similar.
      i16 x; // can be a keycode as well or another data too
      i16 y;

    public:
      void init(void) { b = x = y = 0; }
      c_evententry(void) { init(); }
  };

  enum e_hero
  {
    E_NOHERO = -1,
    E_HERO0,
    E_HERO1,
    E_HERO2,
    E_HERO3
  };

  #define CONTAINERSIZE (8)

// SPX-import #1 vector-type
 // map/z: range 6 bits 10-15 dec: 0-63
 // y:     range 5 bits 5-9   dec: 0-31
 // x:     range 5 bits 0-4   dec: 0-31

	class SPX_Ax3 // size: 0x2
  {
    protected:
  		ui16 w0;

    public:
      ui16 get(void) const { return w0; } // TODO
      void set(ui16 v) { w0 = v; } // TODO
      void mask(void) { w0 &= 0xffe0; }

// prepared:
		  ui16 GetX(void) const { return (w0) & 0x1f; }
		  ui16 GetY(void) const { return (w0 >> 5) & 0x1f; } // was << 6 with >> 11 unsigned
		  ui16 GetMap(void) const { return (w0 >> 10) & 0x3f; }

		  void SetX(ui16 val);
		  void SetY(ui16 val);
		  void SetMap(ui16 val);

		  static const SPX_Ax3 Invalid;

		  bool Is0() const { return w0 == 0; }
		  bool IsInvalid() const { return w0 == -1; }
		  bool IsValid() const { return !IsInvalid(); }

		  SPX_Ax3() { }

		  static SPX_Ax3 Frm(i16 w)
      {
			  SPX_Ax3 a;
			  a.w0 = w;
			  return a;
		  }
	};

	struct ObjectID
  {
		// FEDCBA98 76543210
		// -----------------
		// DDTTTTii iiiiiiii
		//
		//         DD -> direction. 0=North, 1=E, 2=S, 3=W
		//       TTTT -> record type. 0=door, 1=tele, 2=text, 3=actu, ...
		// iiiiiiiiii -> record index. 0 to 1023.
		ui16 w_00;
  };

#endif
