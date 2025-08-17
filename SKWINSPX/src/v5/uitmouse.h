#ifndef C_TMOUSE_H
  #define C_TMOUSE_H

  #ifndef C_GFX_PIXEL_H
    #include <gfxpixel.h>
  #endif

  #ifndef C_RECT_H
    #include <skrect.h>
  #endif

  #ifndef C_CLICKRECT_H
    #include <uiclrect.h>
  #endif

  #define MOUSE_QUEUE_LENGTH (10)

  class c_mousequeue // it's a fifo - for buffering inputs, not events!
  {
    protected:
      i16 counter; // was DRVW_mousequeuecounter
      i16 idx_in; // was DRVW_mousequeueindex_in
      i16 idx_out; // was DRVW_mousequeueindex_out
      c_evententry queue[MOUSE_QUEUE_LENGTH];

    public:
      c_mousequeue(void) { init(); }
      void init(void);
      bool push(c_evententry me);
      bool pop(OO c_evententry& me);
  };

  #define NOMASK (0xff)

  // Note: c_mblitter always blits the mousecursor into gfxsys.dm2mscreen
  // nothing else

  class c_mblitter // mouse-blitfunctions
  {
    protected:
      void blit_hline(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcofs, ui16 destofs, ui16 numpixels);
      void blit_hline_masked(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcofs, ui16 destofs, ui16 upix, c_pixel umask);
      void blit(c_pixel256* srcmap, c_pixel256* destmap, c_rect* rectp, i16 sx, i16 sy, i16 sw, i16 dw, i8 masked, ui8 bmask);
      void blit_hline_stretched(c_pixel256* srcgfx, c_pixel256* destgfx, ui16 srcx, ui16 destx);

      friend class c_Tmouse;
      friend void _specialblit(c_pixel256* dest, c_pixel256* srcmap, c_rect* r, i16 stretched); // TODO
  };

  // from driver
  class c_xmouserect
  {
    public:
      c_rect rc;
      i16 b; // buttons

      void init(void);
  };

  class c_Tmouse
  {
    protected:
      c_evententry entry;

      e_cursoridx DRVW_cursoridx;
      i16 lastx; // was DRVW_show_mx
      i16 lasty; // was DRVW_show_my

      i16 DRVW_blockmouseinputcounter;

      i16 DRVW_mousecapturedcounter;
      c_mousequeue mousequeue;

      c_mblitter mblitter;

      bool mouse_setrect;
      i16 mouse_rx0;
      i16 mouse_rx1;
      i16 mouse_ry0;
      i16 mouse_ry1;

      c_rect DRVR_mouserect;
      c_rect mdummyrect; // was mouse_rect

      c_xmouserect* DRV_xmouserectptr;
      c_xmouserect DRVR_rect1;
      c_xmouserect DRVR_rect2;
      bool DRVb_use_rect2;

      c_clickrectnode* rectlist1;
      c_clickrectnode* rectlist2;

    public:
      i16 mouse_invisible; // TODO public not good, but else problems with DM2_SHOW_MOUSE()

    //private:
	public: // SPX for easier access during Debug
      e_cursoridx T1_queue_0x20(i16 x, i16 y); // was T1_443c_00f8
      e_cursoridx T1_queue_event(i16 x, i16 y, i16 b); // was T1_execmouseclick
      void T1_drawmouse_sub(c_pixel256* screen, c_pixel256* srcmap, c_rect* rectp, i16 x, i16 y, i16 ww, i8 bmask, bool restorebackground);
	public: // SPX, force use of it
      void T1_drawmouse(void);

    //protected:
	public:
      bool T1_driver_mouseint(c_evententry me);
      void command_interpreter(void);
	protected:

      friend void event_loop_T1(void);
      friend class c_servercommand;
      friend class c_clickrectnode;
      friend void DM2_GET_MOUSE_ENTRY_DATA(OO i16& mx, OO i16& my, OO i16& mb); // TODO
      friend void _specialblit(c_pixel256* dest, c_pixel256* srcmap, c_rect* r, i16 stretched); // TODO

    public:
      void init(void);
      bool is_visible(void) const { return mouse_invisible == 0; }
      void hide(void) { mouse_invisible = 1; } // hides mouse immediately and primary
      void driver_blockmouseinput(void);
      void driver_unblockmouseinput(void);
  };

  extern c_Tmouse Tmouse;

  class c_commandqueue; // forward

  #define COMMAND_QUEUE_LENGTH  (10)

  class c_servercommand
  {
    protected:
      i16 command;

    public:
      c_servercommand(void) { command = 0; }
      c_servercommand(i16 c) { command = c; }

      friend class c_commandqueue;
      friend void c_Tmouse::command_interpreter(void);
  };

  void send_command(i16 c);

  class c_commandqueue // it's a fifo
  {
    protected:
      i16 idx_in;
      i16 idx_out;
      c_servercommand queue[COMMAND_QUEUE_LENGTH];

    public:
      c_commandqueue(void) { flush(); }
      void flush(void) { idx_in = idx_out = 0; }
      bool push(c_servercommand sc);
      bool pop(OO c_servercommand& sc);
  };

  extern c_commandqueue cqueue;

  void DM2_HIDE_MOUSE(void);
  void DM2_SHOW_MOUSE(void);
  void DM2_MOUSE_SET_CAPTURE(void);
  void DM2_MOUSE_RELEASE_CAPTURE(void);
  void DM2_REFRESHMOUSE(void);
  void DM2_CHOOSE_CURSOR3(void);
  void DM2_RELEASE_MOUSE_CAPTURES(void);
  void DM2_GET_MOUSE_ENTRY_DATA(OO i16& mx, OO i16& my, OO i16& mb);

#endif
