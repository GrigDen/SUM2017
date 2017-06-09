НАЧАЛО РЕАЛИЗАЦИИ:

DEF.H
  MemHandle
  UINT64 INT64 (DBL FLT <-- VEC.H)

MAIN.C - WinMain + WinFunc (работа с WinAPI)
!!! все реализуется с префиксом ИФК (VG4)
    AG4 DG5 AK5 NK5 AH5
ANIM.C
  VG4_Anim***
  VG4_AnimInit( HWND hWnd );
  VG4_AnimClose( VOID );
  VG4_AnimResize( INT W, INT H );
  VG4_AnimCopyFrame( HDC hDC );
  VG4_AnimRender( VOID );
  VG4_AnimAddUnit( vg4UNIT *Uni );
  VG4_AnimFlipFullScreen( VOID );

ANIM.H:
+ VG4_Anim - глобальная переменная - структура контекста анимации типа vg4ANIM:
typedef struct tagvg4ANIM
{
  ...
} vg4ANIM;
правильно:
typedef struct tagvg4ANIM vg4ANIM; -> в начале файла
typedef struct tagvg4UNIT vg4UNIT; -> в начале файла

struct tagvg4ANIM
{
  HWND hWnd;                     - окно
  HDC hDC;                       - контекст в памяти
  INT W, H;                      - размер окна
  HBITMAP hFrame;                - картинка кадра
  vg4UNIT *Units[VG4_MAX_UNITS]; - массив объектов анимации
  INT NumOfUnits;                - текущее количество объектов анимации
};

struct tagvg4UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( vg4UNIT *Uni, vg4ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( vg4UNIT *Uni, vg4ANIM *Ani );
};

UNITS.C - вся базовая работа с объектами анимации

--- функции - "заглушки" - нужны для начальной инициализации любого объекта анимации по умолчанию

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitClose( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitResponse( vg4UNIT *Uni, vg4ANIM *Ani );
{
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_UnitRender' function */

- основная функция создания объекта анимации по умолчанию:

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size )
{
  vg4UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(vg4UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = VG4_UnitInit;
  Uni->Close = VG4_UnitClose;
  Uni->Response = VG4_UnitResponse;
  Uni->Render = VG4_UnitRender;
  return Uni;
} /* End of 'VG4_AnimUnitCreate' function */

пример своего объекта анимации:

U_BBALL.C

#include "anim.h"

typedef struct
{
  VG4_UNIT_BASE_FIELDS;
  VEC Pos;
} vg4UNIT_BALL;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitInit( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(0, 1, 0);
} /* End of 'VG4_UnitInit' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT_BALL *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID VG4_UnitRender( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  DrawSphere(Uni->Pos, 111);
} /* End of 'VG4_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateBall( VOID )
{
  vg4UNIT_BALL *Uni;

  if ((Uni = (vg4UNIT_BALL *)VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = VG4_UnitInit;
  Uni->Render = VG4_UnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitCreateBall' function */

UNITS.H
-- функции-конструкторы примеров:
/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
vg4UNIT * VG4_UnitCreateBall( VOID );

#include "units.h"
WinMain:
. . .
  VG4_AnimAddUnit(VG4_UnitCreateBall());

VG4_AnimRender:
  . . . timer
  . . . опросили все (kbd, mouse, joystick)
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  . . . очищаем кадр
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    . . . можно сбросить все кисти и перья
    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
  . . .
}
