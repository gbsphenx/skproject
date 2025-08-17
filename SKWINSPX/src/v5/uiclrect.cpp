#include <skxrect.h>
#include <dm2data.h>
#include <skalloc.h>
#include <uitmouse.h>
#include <uiclrect.h>

#include <dm2debug.h>

c_clickrectnode table1d32d8[18];

const c_clickrectnode _table1d32d8[18] =
{
  { 0x000b, 0x00, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x005c, 0x01, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x00a1, 0x01, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x00a2, 0x01, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x00a3, 0x01, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x00a4, 0x01, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x0009, 0x00, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x0007, 0x01, 0x0f, 0x00, 0x00, 0x00000000 },
  { 0x0007, 0x01, 0x0f, 0x00, 0x60, 0x00000000 },
  { (i16)0x823b, 0x00, 0x0f, 0x00, 0x00, 0x00000000 },
  { (i16)0x81ee, 0x00, 0x0f, 0x00, 0x00, 0x00000000 },
  { 0x002f, 0x02, 0x0f, 0x00, 0x40, 0x00000000 },
  { 0x000b, 0x00, 0x01, 0x00, 0x00, 0x00000000 },
  { 0x009c, 0x00, 0x02, 0x00, 0x00, 0x00000000 },
  { 0x009d, 0x00, 0x02, 0x00, 0x00, 0x00000000 },
  { 0x009e, 0x00, 0x02, 0x00, 0x00, 0x00000000 },
  { 0x009f, 0x00, 0x02, 0x00, 0x00, 0x00000000 },
  { (i16)0x8267, 0x00, 0x0f, 0x00, 0x00, 0x00000000 }
};

void init_clickrecttable(void)
{
  SPX_DEBUG_PRINT("CLICKRECTABLE:INIT\n");

  for (i16 i=0; i<18; i++)
    table1d32d8[i] = _table1d32d8[i];
}

// the clickrectangle-list is unfortunately available to all threads.
// I would prefer it, that the common thread just gets pure mouseinfos,
// and decides itself what to do. now, the current way, the common
// thread has to provide the informations to the event-thread, then
// just reads them.

// was DM2_443c_00a9, TODO: tell SPX new name
void c_clickrectnode::DM2_SET_CLICKRECTDATAS(i16 x0, i16 y0, i16 x1, i16 y1)
{
  Tmouse.rectlist1 = this;
  Tmouse.mouse_rx0 = x0;
  Tmouse.mouse_rx1 = x1;
  Tmouse.mouse_ry0 = y0;
  Tmouse.mouse_ry1 = y1;
  Tmouse.DRV_xmouserectptr->rc.x = x0;
  Tmouse.DRV_xmouserectptr->rc.y = y0;
  Tmouse.DRV_xmouserectptr->rc.w = x1 - x0 + 1;
  Tmouse.DRV_xmouserectptr->rc.h = y1 - y0 + 1;
  Tmouse.DRV_xmouserectptr->b = 0x20;

  send_command(4); // grab datas
}

// was SKW_443c_07d5, TODO: tell SPX new name
void c_clickrectnode::DM2_REFRESH_CLICKRECTLINK_1(void)
{
  if ((b_03 & 0x80) == 0)
    return;
  b_03 &= 0x7f;

  c_clickrectnode** rg2 = &Tmouse.rectlist2;
  c_clickrectnode* rg4;
  //m_4CDE:
  for (rg4 = Tmouse.rectlist2; rg4 != NULL && rg4 != this; rg4 = *rg2)
    rg2 = &rg4->node->next;

  //m_4CED:
  Tmouse.driver_blockmouseinput();
  *rg2 = node->next;
  Tmouse.driver_unblockmouseinput();
  DM2_REFRESHMOUSE();
}

// was DM2_443c_06b4, TODO: tell SPX new name
void c_clickrectnode::DM2_REFRESH_CLICKRECTLINK_2(void)
{
  if ((b_03 & 0x80) != 0)
    return;
  b_03 |= 0x80;

  c_clickrectnode** rg4 = &Tmouse.rectlist2;
  c_clickrectnode* rg2 = Tmouse.rectlist2;
  //m_4C25:
  while (rg2 != NULL && ((rg2->b_03 & 0xf) > (b_03 & 0xf)))
  {
    rg4 = &rg2->node->next;
    rg2 = *rg4;
  }
  //m_4C4A:
  Tmouse.driver_blockmouseinput();
  node->next = rg2;
  *rg4 = this;
  c_rect* r = &node->r;
  DM2_QUERY_EXPANDED_RECT(w_00 & 0x3fff, r);
  if ((w_00 & 0x8000) != 0)
  {
    r->x += ddat.v1d26fc;
    r->y += ddat.v1d26fe;
  }
  if (b_05 != 0)
    DM2_SET_CLICKRECTDATAS(r->x, r->y, r->x + r->w - 1, r->y + r->h - 1);
  Tmouse.driver_unblockmouseinput();
  DM2_REFRESHMOUSE();
}

// was SKW_443c_067a, TODO: tell SPX new name
void c_clickrectnode::DM2_ALLOC_CLICKRECTDATA(void)
{
  if ((b_03 & 0x40) == 0)
  {
    b_03 |= 0x40;
    node = UPCAST(c_clickrectdata, dm2_dballochandler.DM2_ALLOC_MEMORY_RAM(sizeof(c_clickrectdata), MASK_200, FREEPOOL)); // TODO: allocating with non-standard mask
  }
}
